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

#include "c_data.hh"

c_data::c_data(unsigned long ulSize)
       :c_pdu(NULL)
{
  // allocate space for the data
  ulLength = ulSize;
  pucData = new unsigned char[ulLength];
  assert(pucData);
}

c_data::~c_data()
{
  if (pucData)
  {
    delete [] pucData;
    pucData = NULL;
  }
  ulLength = 0;
}

void c_data::set(unsigned char *pucNewData, unsigned long ulNewDataLength)
{
  // pre-condition assertions
  assert(pucNewData);
  assert(ulNewDataLength);
  assert(pucData);
  assert(ulLength);

  // verify whether data buffer is large enough to hold the new data
  assert(ulNewDataLength <= ulLength);

  // copy the data
  memcpy(pucData, pucNewData, ulNewDataLength);
}

unsigned long c_data::pduLength()
{
  return ulLength;
}

void c_data::assemble(unsigned char *pucBufferData, unsigned long ulBufferLength)
{
  // pre-condition assertions
  assert(pucBufferData);
  assert(ulBufferLength);

  // verify whether the buffer is large enough
  assert(ulBufferLength >= ulLength);

  // copy the data (if any)
  if (ulLength>0)
  {
    assert(pucData);
    memcpy(pucBufferData, pucData, ulLength);
  }
}


