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

#include <sys/socket.h>
#include <netinet/in.h>  // for htons()
#include <unistd.h>  // for getopt()

#include <string.h>  // for memcpy()
#include <iostream>
#include <fstream>  // for ofstream
#include <assert.h>  // for assert()

using namespace std;

#include "c_prx_udp.hh"

c_prx_udp::c_prx_udp()
          :c_prx(ulStampOffsetUdp)
{
  // initialize the member variables
  uiPort = 12345;
}

c_prx_udp::~c_prx_udp()
{
}

void c_prx_udp::setPort(uint16 uiPort)
{
  this->uiPort = uiPort;
}

void c_prx_udp::start()
{
  int iResult;

  // create a udp socket
  int iSocketId = socket(PF_INET, SOCK_DGRAM, 0);
  
  // bind it to the given port
  struct sockaddr_in sAddressBind;
  bzero(&sAddressBind, sizeof(sAddressBind));
  sAddressBind.sin_family      = AF_INET;
  sAddressBind.sin_port        = htons(uiPort);
  sAddressBind.sin_addr.s_addr = INADDR_ANY;
  iResult = bind(iSocketId, (sockaddr *) &sAddressBind, sizeof(sAddressBind));
  assert(iResult==0);

  // receive packets
  struct sockaddr_in sAddressReceive;
  socklen_t iAddressLength = sizeof(sAddressReceive);
#define BUFFER_LENGTH 1500
  char acBuffer[BUFFER_LENGTH];
  struct timeval sTimestamp;  
  int iContinue = 1;
  unsigned long ulCounter = 0; 
  do
  {
    // receive next packet
    int iLength = recvfrom(iSocketId, (void *) acBuffer, (size_t) BUFFER_LENGTH, 0, (sockaddr *) &sAddressReceive, &iAddressLength);

    // obtain a timestamp
    iResult = gettimeofday(&sTimestamp, NULL);
    assert(iResult==0);

    // add the packet to the list
    add((const unsigned char *) acBuffer, iLength, &sTimestamp);

    // decide whether to continue receiving packets
    if (iPacketCount >0)
    {
      iContinue = (ulCounter++ < (unsigned long) iPacketCount);
    }

  } while (iContinue);

} 

