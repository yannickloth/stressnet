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

#include "c_eth.hh"

c_eth::c_eth(c_pdu *poPayload)
      :c_pdu(this)
{
  // allocate space for the Ethernet header
  allocateHeader(14);

  // define the fields in the header
  poDestAddr = new c_macaddr((uint8 *) &pucHeaderData[0]);
  assert(poDestAddr);
  poSrceAddr = new c_macaddr((uint8 *) &pucHeaderData[6]);
  assert(poSrceAddr);
  puiType    = (uint16 *) &pucHeaderData[12];

  // store the pointer to the payload
  // (this pointer may be zero)
  this->poPayload = poPayload;
}

c_eth::~c_eth()
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

c_macaddr *c_eth::destAddr()
{
  return poDestAddr;
}

c_macaddr *c_eth::srceAddr()
{
  return poSrceAddr;
}

uint16 c_eth::type()
{
  return ntohs(*puiType);
}

void c_eth::type(uint16 uiNewType)
{
  *puiType = htons(uiNewType);
}

unsigned long c_eth::pduLength()
{
  // pre-condition assertions
  assert(poPayload);

  // return the sum of the header length and the payload length,
  // with a minimum payload length of 46 bytes.
  unsigned long ulLengthPayload = poPayload->pduLength();
  if (ulLengthPayload <46)
    return ulHeaderLength + 46;
  else
    return ulHeaderLength + ulLengthPayload;
}

