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

#include <string.h>  // for strncpy()
#include <assert.h>  // for assert()

#include "c_ptx_udp.hh"
#include "s_stamp.hh"

c_ptx_udp::c_ptx_udp()
          :c_ptx(ulStampOffsetUdp)
{
  // initialize the member variables
  strncpy(acDestAddress, "127.0.0.1", (size_t) ADDRESS_LENGTH);
  uiDestPort = 12345;
}

c_ptx_udp::~c_ptx_udp()
{
}

void c_ptx_udp::setDestAddress(char *pcDestAddress)
{
  strncpy(acDestAddress, pcDestAddress, (size_t) ADDRESS_LENGTH);
}

void c_ptx_udp::setDestPort(uint16 uiDestPort)
{
  this->uiDestPort = uiDestPort;
}

#include <unistd.h>  // for close()
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>  // for htons()
#include <arpa/inet.h>  // for inet_aton()

void c_ptx_udp::transmit()
{
  int iResult = 0;

  // create a udp socket
  int iSocketId = socket(PF_INET, SOCK_DGRAM, 0);

  // bind it to the given port
  struct sockaddr_in sAddressBind;
  bzero(&sAddressBind, sizeof(sAddressBind));
  sAddressBind.sin_family      = AF_INET;
  sAddressBind.sin_port        = htons(uiDestPort);
  inet_aton(acDestAddress, &sAddressBind.sin_addr);
  iResult = connect(iSocketId, (sockaddr *) &sAddressBind, sizeof(sAddressBind));
  assert(iResult==0);

  // callibrate gettimeofday()
  unsigned long ulCounterMax = 1000000;  // callibrate with 1 million samples
  struct timeval sTimeBegin ;  
  struct timeval sTimeEnd ;  
  iResult = gettimeofday(&sTimeBegin, NULL);
  assert(iResult==0);
  for ( unsigned long ulCounter=1 ; ulCounter<ulCounterMax ; ulCounter++ )
  {
    iResult = gettimeofday(&sTimeEnd, NULL);
    assert(iResult==0);
  }
  double dTimeBegin = sTimeBegin.tv_sec + sTimeBegin.tv_usec /1.0e6;
  double dTimeEnd   = sTimeEnd.tv_sec   + sTimeEnd.tv_usec   /1.0e6;
// cout << "duration = " << (dTimeEnd - dTimeBegin) / (double) ulCounterMax << endl;
// result: 4.4e-7s
  double dHalfDurationGetTime = (dTimeEnd - dTimeBegin) / (double) ulCounterMax / 2.0;

  // pump the packets out
  s_stamp *psStamp = NULL;
  unsigned long ulCounter = 0;
  double dTimePreviousPacket = 0.0;
  s_ptx_entry *psEntry = psFirstEntry;
  for ( unsigned long i=0 ; i<ulNumberOfPacketsToTransmit ; i++ )
  {
    // point to the stamp position in the packet
    psStamp = (s_stamp *) &psEntry->aucPacketData[ulStampOffset];

    // fill in the counter of the stamp
    psStamp->ulCounter = ulCounter++;

    // if the bitrate has been restricted, compute the duration
    // of the "time slot" for a packet of the given size at the
    // given bitrate, and wait until the end of this "timeslot"
    if (dBitrate > 0.0)
    {
      // compute the end of the timeslot
      double dTimeslotEnd = dTimePreviousPacket + psEntry->ulPacketLength *8 / dBitrate - dHalfDurationGetTime;

      // wait until this time
      double dTimeNow = 0.0;
      do
      {
        iResult = gettimeofday(&psStamp->sTimeval, NULL);
        assert(iResult==0);
        dTimeNow = psStamp->sTimeval.tv_sec + psStamp->sTimeval.tv_usec /1.0e6;
      } while (dTimeNow < dTimeslotEnd);
 
      // store the transmission time of this packet so we can compute 
      // the timeslot end time for the next packet in the next iteration
      dTimePreviousPacket = dTimeNow;
    }
    else
    {
      // fill in the time stamp of the packet
      // (must be the last operation before writing the packet to the socket)
      iResult = gettimeofday(&psStamp->sTimeval, NULL);
      assert(iResult==0);
    }

    // send out the packet
    iResult = send(iSocketId, psEntry->aucPacketData, psEntry->ulPacketLength, 0);
    assert(iResult>0);

    // step to the next packet if there is one,
    // otherwise start over at the first one
    if (psEntry->psNextEntry)
    {
      psEntry = psEntry->psNextEntry;
    }
    else
    {
      psEntry = psFirstEntry;
    }

  }

  // finished
  iResult = close(iSocketId);
  assert(iResult==0);
}

