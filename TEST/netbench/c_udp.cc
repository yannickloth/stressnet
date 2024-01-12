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

#include <stdio.h>


#include <netinet/in.h>  // for htons(), ntohs()
#include <assert.h>  // for assert()

#include "c_udp.hh"

c_udp::c_udp(c_pdu *poPayload)
      :c_pdu(this)
{
  // allocate space for the UDP header
  allocateHeader(8);

  // define the fields in the header
  puiSrcePort = (uint16 *) &pucHeaderData[0];
  puiDestPort = (uint16 *) &pucHeaderData[2];
  puiLength   = (uint16 *) &pucHeaderData[4];
  puiChecksum = (uint16 *) &pucHeaderData[6];

  // some initial values which can be overwritten
  // by the user using member function.
  length(8 + poPayload->pduLength());

  // store the pointer to the payload
  // (this pointer may be zero)
  this->poPayload = poPayload;
}

uint16 c_udp::srcePort()
{
  return ntohs(*puiSrcePort);
}

void c_udp::srcePort(uint16 uiNewSrcePort)
{
  *puiSrcePort = htons(uiNewSrcePort);
}

uint16 c_udp::destPort()
{
  return ntohs(*puiDestPort);
}

void c_udp::destPort(uint16 uiNewDestPort)
{
  *puiDestPort = htons(uiNewDestPort);
}

uint16 c_udp::length()
{
  return ntohs(*puiLength);
}

void c_udp::length(uint16 uiNewLength)
{
  *puiLength = htons(uiNewLength);
}

uint16 c_udp::checksum()
{
  return ntohs(*puiChecksum);
}

void c_udp::checksum(uint16 uiNewChecksum)
{
  *puiChecksum = htons(uiNewChecksum);
}

uint16 c_udp::computeChecksum(c_ip *poIp)
{
  // create the byte sequence over which we need to compute the checksum
  unsigned long ulBufferSize = 12 // pseudo IP header
                             +  8 // UDP header
                             + poPayload->pduLength();
  ulBufferSize = 2 * ((ulBufferSize+1) /2);  // round to higher multiple of 2

  unsigned char aucBufferData[ulBufferSize];
  aucBufferData[ulBufferSize-1] = 0;  // in case last byte is a padding byte

  // fill in pseudo IP header
  for ( unsigned long i=0 ; i<4 ; i++ )
  {
    aucBufferData[i]   = poIp->srceAddr()->byte(i);
    aucBufferData[i+4] = poIp->destAddr()->byte(i);
  }
  aucBufferData[8] = 0;
  aucBufferData[9] = poIp->protocol();
  *((uint16 *) &aucBufferData[10]) = htons(length());
  /* note:
   *   one would expect here "htons(poIp->length())" as the right-value, 
   *   in other words that the length of the IP datagram would be used
   *   in the IP pseudo header, but it is the UDP length we have to use. 
   *   Strange but that's the way it is...
   */

  // fill in UDP header (with zero checksum) and payload
  checksum(0);
  assemble(&aucBufferData[12], ulBufferSize-12);

  // add all 16bit values together
  uint32 accumulator32 = 0;
  for ( unsigned long i=0 ; i<ulBufferSize ; i+=2 )
  {
    accumulator32 += ntohs(*((uint16 *) &aucBufferData[i]));
  }

  // carry
  uint16 accumulator16 = (accumulator32 >> 16) + (accumulator32 & 0xFFFF);

  // inverse the bits
  accumulator16 = ~accumulator16;

  // if the computed checksum is zero, it must be set to 0xFFFF.
  if (accumulator16 ==0)
  {
    accumulator16 = 0xFFFF;
  }

  // ready
  return accumulator16;
}


