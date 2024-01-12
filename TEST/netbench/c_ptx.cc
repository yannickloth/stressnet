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

#include "types.hh"
#include "c_ptx.hh"
#include "s_stamp.hh"

c_ptx::c_ptx(unsigned long ulStampOffset)
{
  // initialize the member variables
  psFirstEntry = NULL;
  psLastEntry  = NULL;
  pcDevice = "eth0";
  dBitrate = 0.0;  // 0 means "no restriction"
  ulNumberOfPacketsToTransmit = 1;

  this->ulStampOffset = ulStampOffset;
}

c_ptx::~c_ptx()
{
  // remove all entries from the linked list
  // and free the memory that was allocated for them.
  while (psFirstEntry)
  {
    // keep the pointer to the next entry
    // (necessary because we will free psFirstEntry below)
    s_ptx_entry *psNextEntry = psFirstEntry->psNextEntry;

    // free the current entry
    delete psFirstEntry;

    // step to the next entry
    psFirstEntry = psNextEntry;
  }
  psLastEntry = NULL;
}

void c_ptx::setInterface(char *pcDevice)
{
  this->pcDevice = pcDevice;
}

void c_ptx::setBitrate(double dBitrate)
{
  this->dBitrate = dBitrate;
}

void c_ptx::setNumberOfPacketsToTransmit (unsigned long ulNumberOfPacketsToTransmit)
{
  this->ulNumberOfPacketsToTransmit = ulNumberOfPacketsToTransmit;
}

void c_ptx::add(unsigned char *pucNewPacketData, unsigned long ulNewPacketLength, uint16 uiType)
{
  // pre-conditions
  assert(pucNewPacketData!=NULL);
  assert(ulNewPacketLength>0);

  // allocate memory for a new entry in the linked list
  unsigned char *pucNewEntry = new unsigned char [sizeof(s_ptx_entry) + ulNewPacketLength];
  assert(pucNewEntry!=NULL);
  s_ptx_entry *psNewEntry = (s_ptx_entry *) pucNewEntry;

  // fill in the fields of this new entry
  psNewEntry->psNextEntry = NULL;
  psNewEntry->ulPacketLength = ulNewPacketLength;
  memcpy(psNewEntry->aucPacketData, pucNewPacketData, ulNewPacketLength);

  // point to the stamp position in the packet
  s_stamp *psStamp = (s_stamp *) &psNewEntry->aucPacketData[ulStampOffset];

  // fill in the fixed signature and the type field of the stamp
  psStamp->uiSignature = uiStampSignature;
  psStamp->uiType = uiType;

  // add it to the end of the list
  if (psFirstEntry == NULL)
  {
    psFirstEntry = psNewEntry;
    psLastEntry  = psNewEntry;
  }
  else
  {
    psLastEntry->psNextEntry = psNewEntry;
    psLastEntry = psNewEntry;
  }
}

