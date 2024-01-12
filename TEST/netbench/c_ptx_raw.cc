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

#include <assert.h>  // for assert()

#include "c_ptx_raw.hh"
#include "s_stamp.hh"

c_ptx_raw::c_ptx_raw()
          :c_ptx(ulStampOffsetRaw)
{
  // initialize the member variables
  pcDevice = "eth0";
}

c_ptx_raw::~c_ptx_raw()
{
}

void c_ptx_raw::setInterface(char *pcDevice)
{
  this->pcDevice = pcDevice;
}

#include <sys/time.h>    // for gettimeofday(), ...
#include <string.h>      // for strcpy()
#include <sys/socket.h>  // for socket(),  ...
#include <netinet/in.h>  // for htons()
#include <net/if.h>      // for if_nametoindex()
#include <unistd.h>      // for write(), close(), ...

#include <features.h>    /* for the glibc version number */
#if __GLIBC__ >= 2 && __GLIBC_MINOR >= 1
#include <netpacket/packet.h>
#include <net/ethernet.h>     /* the L2 protocols */
#else
#include <asm/types.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>   /* The L2 protocols */
#endif

void c_ptx_raw::transmit()
{
  int iResult;

  // select the interface
  struct sockaddr sa;
  sa.sa_family = AF_INET;
  strcpy (sa.sa_data, pcDevice);

  // prepare a socket for transmission
  int iSocketId = socket (PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
  assert(iSocketId>=0);

  // bind the socket
  struct sockaddr_ll sSockAddr;
  sSockAddr.sll_family = AF_PACKET;
  sSockAddr.sll_protocol = htons(ETH_P_ALL);
  sSockAddr.sll_ifindex = if_nametoindex(pcDevice);
  iResult = bind( iSocketId, (struct sockaddr *) &sSockAddr, sizeof(sSockAddr));
  assert(iSocketId>=0);

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
    iResult = write(iSocketId, psEntry->aucPacketData, psEntry->ulPacketLength);
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

