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

#include <string.h>  // for memcpy()
#include <assert.h>  // for assert()

#include "c_pdu.hh"

c_pdu::c_pdu(c_pdu *poPayload)
{
  pucHeaderData = NULL;
  ulHeaderLength = 0;
  this->poPayload = poPayload;
}

c_pdu::~c_pdu()
{
  if (pucHeaderData)
  {
    delete [] pucHeaderData;
    pucHeaderData = NULL;
  }
  ulHeaderLength = 0;
}

unsigned long c_pdu::pduLength()
{
  // pre-condition assertions
  assert(poPayload);

  // return the sum of the header length and the payload length
  return ulHeaderLength + poPayload->pduLength();
}

void c_pdu::assemble(unsigned char *pucBufferData, unsigned long ulBufferLength)
{
  // pre-condition assertions
  assert(pucBufferData);
  assert(ulBufferLength);
  assert(poPayload);

  // verify whether the buffer is large enough
  assert(ulBufferLength >= ulHeaderLength + poPayload->pduLength());

  // copy the header
  assert(pucHeaderData);
  assert(ulHeaderLength);
  memcpy(pucBufferData, pucHeaderData, ulHeaderLength);

  // delegate assembly of the data part to the payload pdu
  poPayload->assemble(&pucBufferData[ulHeaderLength], ulBufferLength-ulHeaderLength);
}

void c_pdu::allocateHeader(unsigned long ulLength)
{
  ulHeaderLength = ulLength;
  pucHeaderData = new unsigned char[ulHeaderLength];
  assert(pucHeaderData);
}



