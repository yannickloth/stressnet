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
#include "c_eth.hh"
#include "c_ip.hh"
#include "c_udp.hh"
#include "c_ptx_raw.hh"
#include "s_stamp.hh"  // for ucTypeMeasurement

#define USE_THREAD

void *threadPacketGenerator(void *pvArgument)
{
  // convert the void argument to a c_ptx object pointer
  c_ptx *poPtx = (c_ptx *) pvArgument; 
 
  // transmit a given number of packets through this c_ptx object
  poPtx->transmit();

  // ready
  return NULL;
}

int main(int argc, char **argv)
{
  c_ptx_raw cPtx;
  unsigned long ulNumberOfPacketsToCreate = 1;
  unsigned long ulDataSize = 1500 - 8 - 20;
  char acDestinationMacAddress[50] = {0};
  int iRandomDestinationMacAddresses = 0;
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
  while	((c = getopt(argc, argv, "c:s:i:b:l:t:d:r")) != -1)
  {
    switch (c) 
    {
      case 'c':
      { ulNumberOfPacketsToCreate = atol(optarg);
      } break;
      case 's':
      { cPtx.setNumberOfPacketsToTransmit(atol(optarg));
      } break;
      case 'i':
      { cPtx.setInterface(optarg);
      } break;
      case 'b':
      { cPtx.setBitrate(atol(optarg));
      } break;
      case 'l':
      { ulDataSize = atol(optarg);
      } break;
      case 't':
      { uiMessageType = atoi(optarg);
      } break;
      case 'd':
      { strncpy(acDestinationMacAddress, optarg, 40);
      } break;
      case 'r':
      { iRandomDestinationMacAddresses = 1; 
      } break;
      case '?':
      { cerr << "ERROR: parameter \"" << optarg << "\" not implemented." << endl;
        exit(0);
      } break;
    }
  }

  // create a prototype packet
  c_data oData(ulDataSize);

  c_udp oUdp(&oData);
  oUdp.srcePort(140);
  oUdp.destPort(144);
  oUdp.checksum(0);

  c_ip oIp(&oUdp);
  oIp.srceAddr()->set(10, 20, 30, 41);
  oIp.destAddr()->set(10, 20, 30, 42);
//  oIp.fragmentOffset(1234);
  oIp.identification(140);
  oIp.timeToLive(128);
  oIp.protocol(17);  // UDP
  oIp.checksum(oIp.computeChecksum());

// do NOT set the UDP checksum since we will index and time stamp
// the packet later so the checksum we compute will no longer be valid.
//  oUdp.checksum(oUdp.computeChecksum(&oIp));

  c_eth oEth(&oIp);
//  oEth.destAddr()->setBroadcast();
  if (acDestinationMacAddress[0])
  { oEth.destAddr()->set(acDestinationMacAddress); }
  else
  { oEth.destAddr()->set(0x00, 0xCA, 0xFE, 0x00, 0x00, 0x00); }
  oEth.srceAddr()->set(0x00, 0xCA, 0xFE, 0x00, 0x00, 0x00);
  oEth.type(0x0800);  // IP

  // create the requested number of packets
  for ( unsigned long i=0 ; i<ulNumberOfPacketsToCreate ; i++ )
  {

    if (iRandomDestinationMacAddresses)
    { oEth.srceAddr()->randomize();
      oEth.destAddr()->randomize(); 
    }
  
    unsigned char packet[1514]={0};
    oEth.assemble(packet, 1514);

    cPtx.add(packet, oEth.pduLength(), uiMessageType);
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

