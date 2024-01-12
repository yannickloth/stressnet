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
#include <iostream>
#include <fstream>  // for ofstream
#include <assert.h>  // for assert()

using namespace std;

#include "c_prx.hh"

c_prx::c_prx(unsigned long ulStampOffset)
{
  // initialize the member variables
  psFirstEntry = NULL;
  psLastEntry  = NULL;
  pcOutputFilename = "/dev/stdout";  // show results on stdout
  iPacketCount = -1;     // loop "eternally" (meaning "until interrupted by user or by an alarm")
  this->ulStampOffset = ulStampOffset;

  // the variables for pool preallocation
  psPoolOfEntries = NULL;   
  ulSizeOfPool = 0;
  ulUsedInPool = 0;
}

c_prx::~c_prx()
{
  // remove the memory that was allocated for the entries
  if (psPoolOfEntries)
  {
    delete [] psPoolOfEntries;
    psPoolOfEntries = NULL;   
    ulSizeOfPool = 0;
    ulUsedInPool = 0;
  }
  else
  {
    // remove all entries from the linked list
    // and free the memory that was allocated for them.
    while (psFirstEntry)
    {
      // keep the pointer to the next entry
      // (necessary because we will free psFirstEntry below)
      s_prx_entry *psNextEntry = psFirstEntry->psNextEntry;

      // free the current entry
      delete psFirstEntry;

      // step to the next entry
      psFirstEntry = psNextEntry;
    }
    psLastEntry = NULL;
  }
}

void c_prx::setOutputFilename(char *pcOutputFilename)
{
  this->pcOutputFilename = pcOutputFilename;
}

void c_prx::setPacketCount(int iPacketCount)
{
  this->iPacketCount = iPacketCount;
}

void c_prx::preAllocate(unsigned long ulSizeOfPool)
{
  // verify that packets have been added previously
  assert(! psFirstEntry);

  // allocate the pool
  this->ulSizeOfPool = ulSizeOfPool;
  psPoolOfEntries = new s_prx_entry [ulSizeOfPool];   
}

void c_prx::add(const unsigned char *pucPacketData, unsigned long ulPacketLength, const struct timeval *psTimeval)
{
  // get a new entry from the pool or allocate one
  s_prx_entry *psNewEntry;
  if (psPoolOfEntries)
  {
    psNewEntry = &psPoolOfEntries [ulUsedInPool++];
    assert(ulUsedInPool <= ulSizeOfPool);
  }
  else
  {
    psNewEntry = new s_prx_entry;
  }

  // fill in the fields of the new entry
  memcpy(&psNewEntry->sStamp, &pucPacketData[ulStampOffset], sizeof(s_stamp));
  psNewEntry->ulLength = ulPacketLength;
  psNewEntry->sTimeval = *psTimeval;
  psNewEntry->psNextEntry = NULL;

  // verify the signature
  if (psNewEntry->sStamp.uiSignature != uiStampSignature)
  {
    if (psPoolOfEntries)
    {
      ulUsedInPool--;
    }
    else
    {
      delete psNewEntry;
    }
    return;
  }

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

unsigned long c_prx::process()
{
  // open the output stream
  ofstream fout;
  fout.open(pcOutputFilename, ios::out);
  assert(! fout.bad());

  // set precision (important !)
  fout.precision(12);

  // write a title line
  fout   << "tx_counter"
  << " " << "tx_time"
  << " " << "rx_time"
  << " " << "length"
  << " " << "code"
  << " " << "bandwidth"
  << endl;

  // process all entries and write the results to the output stream
  unsigned long ulCounter = 0;
  unsigned long ulTimeOrigin = 0;
  s_prx_entry *psEntry = psFirstEntry;
  while (psEntry)
  {
    // when the "time origin" has not yet been set,
    // set it now to the smallest of two (Rx and Tx) timestamps.
    if (ulTimeOrigin ==0)
    {
      if (psEntry->sStamp.sTimeval.tv_sec < psEntry->sTimeval.tv_sec)
      {
        ulTimeOrigin = psEntry->sStamp.sTimeval.tv_sec;
      }
      else
      {
        ulTimeOrigin = psEntry->sTimeval.tv_sec;
      }
    }

    // compute the latency for this packet
    double dTimeTransmit = (psEntry->sStamp.sTimeval.tv_sec - ulTimeOrigin) + psEntry->sStamp.sTimeval.tv_usec /1.0e6;
    double dTimeReceive  = (psEntry->sTimeval.tv_sec        - ulTimeOrigin) + psEntry->sTimeval.tv_usec        /1.0e6;

    // write the results to the output file
    fout   << psEntry->sStamp.ulCounter
    << " " << dTimeTransmit
    << " " << dTimeReceive
    << " " << psEntry->ulLength 
    << " " << (int) psEntry->sStamp.uiType
    << endl;
  
    // step to the next entry
    ulCounter++;
    psEntry = psEntry->psNextEntry;
  }

  // ready
  fout.close();
  return ulCounter;
}

