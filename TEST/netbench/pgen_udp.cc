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

#include <stdlib.h>  // for atoi()
#include <string.h>  // for strlen()
#include <unistd.h>  // for getopt()
#include <pthread.h> // for pthread_...()
#include <assert.h>  // for assert()

#include <iostream>
using namespace std;

#include "c_data.hh"
#include "c_ptx_udp.hh"
#include "s_stamp.hh"  // for uiTypeMeasurement

#define USE_THREAD

void *threadPacketGenerator(void *pvArgument)
{
  // convert the void argument to a c_ptx object pointer
  c_ptx *poPtx = (c_ptx *) pvArgument; 
 
  // transmit a given number of packets
  poPtx->transmit();

  // ready
  return NULL;
}

int main(int argc, char **argv)
{
  c_ptx_udp cPtx;
  unsigned long ulNumberOfPacketsToCreate = 1;
  unsigned long ulDataSize = 1500 - 8 - 20;
  uint16 uiMessageType = uiTypeMeasurement;
  int iResult = 0;

  // show license
  cout << argv[0] << " is free software," << endl;
  cout << "  covered by the GNU General Public License (GPL)."  << endl;
  cout << "  You are welcome to change it and/or distribute" << endl;
  cout << "  copies of it under certain conditions." << endl;
  cout << "  There is absolutely no warranty for this program." << endl;
  cout << "  See the definition of the GPL at" << endl;
  cout << "  http://www.gnu.org/copyleft/gpl.html for details." << endl << endl;

  // parse the command-line options
  int c;
  extern char *optarg;
  while	((c = getopt(argc, argv, "c:s:b:l:t:D:p:")) != -1)
  {
    switch (c) 
    {
      case 'c':
      {
        ulNumberOfPacketsToCreate = atol(optarg);
      } break;
      case 's':
      {
        cPtx.setNumberOfPacketsToTransmit(atol(optarg));
      } break;
      case 'b':
      {
        cPtx.setBitrate(atol(optarg));
      } break;
      case 'l':
      {
        ulDataSize = atol(optarg);
      } break;
      case 't':
      {
        uiMessageType = atoi(optarg);
      } break;
      case 'D':
      {
        cPtx.setDestAddress(optarg);
      } break;
      case 'p':
      {
        cPtx.setDestPort(atoi(optarg));
      } break;
      case '?':
      {
        cerr << "ERROR: parameter \"" << optarg << "\" not implemented." << endl;
        exit(0);
      } break;
    }
  }

  // verify the minimum data size required for storing the timestamp
  assert(ulDataSize>=16);

  // create a prototype packet
  c_data oData(ulDataSize);

  // create the requested number of packets
  for ( unsigned long i=0 ; i<ulNumberOfPacketsToCreate ; i++ )
  {
    unsigned char packet[1514]={0};
    oData.assemble(packet, 1514);

    cPtx.add(packet, oData.pduLength(), uiMessageType);
  }

#ifdef USE_THREAD

  // set the attributes for the transmission thread
  pthread_attr_t sThreadAttr;
  iResult = pthread_attr_init(&sThreadAttr);
  assert(iResult==0);
  iResult = pthread_attr_setschedpolicy(&sThreadAttr, SCHED_FIFO); 
  assert(iResult==0);
  sched_param sSchedParam;
  sSchedParam.sched_priority = 50;
  iResult = pthread_attr_setschedparam(&sThreadAttr, &sSchedParam);
  assert(iResult==0);

  // start the transmission thread
  pthread_t sThread;
  iResult = pthread_create(&sThread, &sThreadAttr, threadPacketGenerator, (void *) &cPtx);
  assert(iResult==0);

  // wait for the thread to finish
  iResult = pthread_join(sThread, NULL);
  assert(iResult==0);

#else

  // generate the packets
  cPtx.transmit(); 

#endif

  // the end
  return 0;
}

