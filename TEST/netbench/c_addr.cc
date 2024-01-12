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

#include <stdlib.h>  // for strtol()
#include <string.h>  // for strlen()
#include <assert.h>  // for assert()

#include "c_addr.hh"

c_addr::c_addr(uint8 *pucAddr, unsigned long ulSize)
{
  this->pucAddr = pucAddr;
  this->ulSize = ulSize;
}

uint8 c_addr::byte(unsigned long ulIndex)
{
  assert(ulIndex<ulSize);
  return pucAddr[ulIndex];
}

void c_addr::byte(unsigned long ulIndex, uint8 uiValue)
{
  assert(ulIndex<ulSize);
  pucAddr[ulIndex] = uiValue;
}

void c_addr::set(const char *pcString, int iBase)
{
  char *pcCursor = (char *) pcString;
  unsigned long ulIndex = 0;
  while (strlen(pcCursor) >0)
  {
    // extract next value
    char *pcNewCursor = NULL;
    uint8 uiValue = strtol(pcCursor, &pcNewCursor, iBase);

    // store it in this address object
    assert(ulIndex<ulSize);
    byte(ulIndex++, uiValue);

    // advance to the next value
    while ((*pcNewCursor == ':') || (*pcNewCursor == '.'))
    {
      pcNewCursor++;
    }

    // continue
    pcCursor = pcNewCursor;
  }
}

