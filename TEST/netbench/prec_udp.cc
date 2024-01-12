/*
 * netbench
 * Copyright (C) 2004   Wim Mees <Wim.Mees@rma.ac.be>
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
#include <pthread.h> // for pthread_...()
#include <assert.h>  // for assert()

#include <iostream>
using namespace std;

#include "c_prx_udp.hh"

#define USE_THREAD

c_prx_udp cPrx;
pthread_t sThread;

void *threadPacketRecorder(void *pvArgument)
{
  // receive the packets
  cPrx.start();

  // ready
  return NULL;
}

void terminateHandler(int iSignal)
{
  // ignore new signals of this type
  signal(SIGINT, SIG_IGN);      
  signal(SIGTERM, SIG_IGN);      
  signal(SIGALRM, SIG_IGN);      

#ifdef USE_THREAD

  // cancel the recorder thread
  int iResult;
  iResult = pthread_cancel(sThread);
  assert(iResult==0);

#endif

  // ready
  return;
}

int main(int argc, char **argv)
{
  int iResult;

  // show license
  cout << argv[0] << " is free software," << endl;
  cout << "  covered by the GNU General Public License (GPL)."  << endl;
  cout << "  You are welcome to change it and/or distribute" << endl;
  cout << "  copies of it under certain conditions." << endl;
  cout << "  There is absolutely no warranty for this program." << endl;
  cout << "  See the definition of the GPL at" << endl;
  cout << "  http://www.gnu.org/copyleft/gpl.html for details." << endl << endl;

  // parse the command-line options
  unsigned int uiAlarmTimeout = 0;
  int c;
  extern char *optarg;
  while	((c = getopt(argc, argv, "p:o:w:c:a:")) != -1)
  {
    switch (c) 
    {
      case 'p':
      {
        cPrx.setPort(atoi(optarg));
      } break;
      case 'o':
      {
        cPrx.setOutputFilename(optarg);
      } break;
      case 'w':
      {
        uiAlarmTimeout = atoi(optarg);
      } break;
      case 'c':
      {
        cPrx.setPacketCount(atol(optarg));
      } break;
      case 'a':
      {
        cPrx.preAllocate(atol(optarg));
      } break;
      case '?':
      {
        cerr << "ERROR: parameter \"" << optarg << "\" not implemented." << endl;
        exit(0);
      } break;
    }
  }

  // install signal handlers
  sighandler_t pResult;
  pResult = signal(SIGINT, terminateHandler);
  assert(pResult!=SIG_ERR);
  pResult = signal(SIGTERM, terminateHandler);
  assert(pResult!=SIG_ERR);
  pResult = signal(SIGALRM, terminateHandler);
  assert(pResult!=SIG_ERR);

  // set an alarm 
  if (uiAlarmTimeout >0)
  {
    alarm(uiAlarmTimeout);         
  }

#ifdef USE_THREAD

  // set the attributes for the recorder thread
  pthread_attr_t sThreadAttr;
  iResult = pthread_attr_init(&sThreadAttr);
  assert(iResult==0);
  iResult = pthread_attr_setschedpolicy(&sThreadAttr, SCHED_FIFO); 
  assert(iResult==0);
  sched_param sSchedParam;
  sSchedParam.sched_priority = 60;
  iResult = pthread_attr_setschedparam(&sThreadAttr, &sSchedParam);
  assert(iResult==0);

  // start the recording thread
  iResult = pthread_create(&sThread, &sThreadAttr, threadPacketRecorder, NULL);
  assert(iResult==0);

  // cleanup
  iResult = pthread_attr_destroy(&sThreadAttr);
  assert(iResult==0);

  // wait for the thread to finish
  iResult = pthread_join(sThread, NULL);
  assert(iResult==0);

#else

  // start the capture
  cPrx.start();

#endif

  // the capture was stopped, now process the measurements
  cPrx.process();

  // the end
  return 0;
}

