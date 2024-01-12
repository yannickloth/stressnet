/*
 * httpbench
 * Copyright (C) 2005   Wim Mees <Wim.Mees@rma.ac.be>
 *                      All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include <stdlib.h>  // for exit()
#include <signal.h>  // for signal()
#include <unistd.h>  // for getopt()
#include <assert.h>  // for assert()

extern "C" 
{ 
#include <pcap.h> 
}

#include <iostream>
#include <fstream>  // for ofstream
using namespace std;

// some default values
char *pcOutputFilename = "/dev/stdout";  // show results on stdout
int  uiAlarmTimeout  =  0;
double dTimeStep     =  1e6;   // (in microseconds !!)
pthread_t sThreadStatisticsDumper;
ofstream fout;

void readProcStat(unsigned long &ulIdle, unsigned long &ulTotal)
{
  // open the /proc/stat virtual file as an input stream
  ifstream fin;
  fin.open("/proc/stat", ios::in);
  assert(! fin.bad());

  // read the cpu data
  char acBuffer[1024];
  fin >> acBuffer;
  unsigned long ulUserMode;
  fin >> ulUserMode;
  unsigned long ulUserModeNice;
  fin >> ulUserModeNice;
  unsigned long ulSystem;
  fin >> ulSystem;

  fin >> ulIdle;
  ulTotal = ulUserMode + ulUserModeNice + ulSystem + ulIdle;

  // close the input stream
  fin.close();
}

void *threadStatisticsDumper(void *pvArgument)
{
  // get the current counter values
  unsigned long ulIdle;
  unsigned long ulTotal;
  readProcStat(ulIdle, ulTotal);
  usleep((unsigned int) dTimeStep);

  // get the current time
  struct timeval sTimeInit;  
  int iResult = gettimeofday(&sTimeInit, NULL);
  assert(iResult==0);
  double dTimeNextDeadline = sTimeInit.tv_sec * 1e6 + sTimeInit.tv_usec;

  while (1)
  { 
    // get the current time
    struct timeval sTimeNow;  
    int iResult = gettimeofday(&sTimeNow, NULL);
    assert(iResult==0);

    // compute current time
    double dCurrentTime = sTimeNow.tv_sec * 1e6 + sTimeNow.tv_usec; 

    // get the load
    unsigned long ulPreviousIdle  = ulIdle;
    unsigned long ulPreviousTotal = ulTotal;
    readProcStat(ulIdle, ulTotal);
    double dLoad;
    if (ulTotal>ulPreviousTotal)
    { dLoad = 1.0 - (double) (ulIdle-ulPreviousIdle) / (double) (ulTotal-ulPreviousTotal); }
    else
    { dLoad = 0.0; }

    // write the results to the output file
    fout   << dCurrentTime
    << " " << dLoad
    << endl;

    // update the next deadline
    dTimeNextDeadline += dTimeStep;

    // get the current time again
    iResult = gettimeofday(&sTimeNow, NULL);
    assert(iResult==0);

    // compute the time until the next deadline
    double dTimeToSleep = dTimeNextDeadline - (sTimeNow.tv_sec * 1e6 + sTimeNow.tv_usec);

    // sleep until the next deadline
    if (dTimeToSleep>0)
    {
      usleep((unsigned int) dTimeToSleep);
    }
  }
}

void terminateHandler(int iSignal)
{
  // ignore new signals of this type
  signal(SIGINT, SIG_IGN);      
  signal(SIGTERM, SIG_IGN);      
  signal(SIGALRM, SIG_IGN);      

  // cancel the recorder and statistics threads
  int iResult;
  iResult = pthread_cancel(sThreadStatisticsDumper);
  assert(iResult==0);

  // ready
  return;
}

int main(int argc, char **argv)
{
  // parse the command-line options
  int c;
  extern char *optarg;
  while	((c = getopt(argc, argv, "i:f:o:s:w:h")) != -1)
  {
    switch (c) 
    {
      case 'o':
      { pcOutputFilename = optarg;
      } break;
      case 's':
      { dTimeStep = atof(optarg) * 1e6;
      } break;
      case 'w':
      { uiAlarmTimeout = atoi(optarg);
      } break;
      default:
      { cout << "Usage: " << argv[0] << " [options]" << endl;
        cout << "Options:" << endl;
        cout << "  -o output-file     (default is stdout)" << endl;
        cout << "  -s time-step       (in seconds, default is 1s)" << endl;
        cout << "  -w duration        (in seconds, default is never stop)" << endl;
        exit(0);
      } break;
    }
  }

  // install signal handlers
  sighandler_t pResult;
  pResult = signal(SIGINT,  terminateHandler);
  assert(pResult!=SIG_ERR);
  pResult = signal(SIGTERM, terminateHandler);
  assert(pResult!=SIG_ERR);
  pResult = signal(SIGALRM, terminateHandler);
  assert(pResult!=SIG_ERR);
  
  // set an alarm 
  if (uiAlarmTimeout >0)
  { alarm(uiAlarmTimeout);         
  }

  // open the output stream
  fout.open(pcOutputFilename, ios::out);
  assert(! fout.bad());

  // set precision (important !)
  // (so we store the timestamps up to ms precision)
  fout.precision(14);

  // set the attributes for the threads
  pthread_attr_t sThreadAttr;
  int iResult;
  iResult = pthread_attr_init(&sThreadAttr);
  assert(iResult==0);
  iResult = pthread_attr_setschedpolicy(&sThreadAttr, SCHED_FIFO); 
  assert(iResult==0);

  // start the statistics thread
  sched_param sSchedParam;
  sSchedParam.sched_priority = 50;
  iResult = pthread_attr_setschedparam(&sThreadAttr, &sSchedParam);
  assert(iResult==0);
  iResult = pthread_create(&sThreadStatisticsDumper, &sThreadAttr, threadStatisticsDumper, NULL);
  assert(iResult==0);

  // cleanup
  iResult = pthread_attr_destroy(&sThreadAttr);
  assert(iResult==0);

  // wait for the threads to finish
  iResult = pthread_join(sThreadStatisticsDumper, NULL);
  assert(iResult==0);

  // the capture was stopped, clean up 
  fout.close();

  // the end
  return 0;
}

