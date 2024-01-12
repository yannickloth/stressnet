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

#include <netinet/in.h>  // for htons(), ntohs()
#include <assert.h>  // for assert()

#include "c_ip.hh"

uint16 c_ip::uiIdentificationCounter = 0;

c_ip::c_ip(c_pdu *poPayload)
     :c_pdu(this)
{
  // allocate space for the IP header
  allocateHeader(20);

  // define the fields in the header
  pucHeaderData[0]  = 4 << 4;  // version
  pucHeaderData[0] |= 20/4;  // Internet header length (IHL)
  pucHeaderData[1]  = 0;  // type of service (TOS)
  puiLength         = (uint16 *) &pucHeaderData[2];
  puiIdentification = (uint16 *) &pucHeaderData[4];
  puiFragmentOffset = (uint16 *) &pucHeaderData[6];
  puiTimeToLive     = (uint8  *) &pucHeaderData[8];
  puiProtocol       = (uint8  *) &pucHeaderData[9];
  puiChecksum       = (uint16 *) &pucHeaderData[10];
  poSrceAddr = new c_ipaddr((uint8 *) &pucHeaderData[12]);
  assert(poSrceAddr);
  poDestAddr = new c_ipaddr((uint8 *) &pucHeaderData[16]);
  assert(poDestAddr);

  // some initial values which can be overwritten
  // by the user using member function.
  length(20 +  poPayload->pduLength());
  identification(uiIdentificationCounter++);
  timeToLive(128);

  // store the pointer to the payload
  // (this pointer may be zero)
  this->poPayload = poPayload;
}

c_ip::~c_ip()
{
  if (poDestAddr)  
  {
    delete poDestAddr;
    poDestAddr = NULL;
  }
  if (poSrceAddr)  
  {
    delete poSrceAddr;
    poSrceAddr = NULL;
  }
}

uint16 c_ip::length()
{
  return ntohs(*puiLength);
}

void c_ip::length(uint16 uiNewLength)
{
  *puiLength = htons(uiNewLength);
}

uint16 c_ip::identification()
{
  return ntohs(*puiIdentification);
}

void c_ip::identification(uint16 uiNewIdentification)
{
  *puiIdentification = htons(uiNewIdentification);
}

uint16 c_ip::fragmentOffset()
{
  return ntohs(*puiFragmentOffset) & 0x1FFF;
}

void c_ip::fragmentOffset(uint16 uiNewFragmentOffset)
{
  *puiFragmentOffset = htons((fragmentOffset() & 0xE000) + (uiNewFragmentOffset & 0x1FFF));
}

uint8 c_ip::timeToLive()
{
  return *puiTimeToLive;
}

void c_ip::timeToLive(uint8 uiNewTimeToLive)
{
  *puiTimeToLive = uiNewTimeToLive;
}

uint8 c_ip::protocol()
{
  return *puiProtocol;
}

void c_ip::protocol(uint8 uiNewProtocol)
{
  *puiProtocol = uiNewProtocol;
}

uint16 c_ip::checksum()
{
  return ntohs(*puiChecksum);
}

void c_ip::checksum(uint16 uiNewChecksum)
{
  *puiChecksum = htons(uiNewChecksum);
}

uint16 c_ip::computeChecksum()
{
  // add all 16bit values together
  uint32 accumulator32 = 0;
  for ( int i=0 ; i<20 ; i+=2 )
  {
    accumulator32 += ntohs(*((uint16 *) &pucHeaderData[i]));
  }

  // carry
  uint16 accumulator16 = (accumulator32 >> 16) + (accumulator32 & 0xFFFF);

  // inverse the bits
  accumulator16 = ~accumulator16;

  // ready
  return accumulator16;
}

c_ipaddr *c_ip::srceAddr()
{
  return poSrceAddr;
}

c_ipaddr *c_ip::destAddr()
{
  return poDestAddr;
}


