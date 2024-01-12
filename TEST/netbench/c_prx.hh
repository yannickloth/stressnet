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

#ifndef C_PRX_HH
#define C_PRX_HH

#include "s_stamp.hh"

struct s_prx_entry;
struct s_prx_entry
{
  // pointer to the next entry in the linked list
  s_prx_entry *psNextEntry;

  // stamp written by the packet generator when the packet was sent
  s_stamp sStamp;  

  // time stamp indicating when the packet was received 
  struct timeval sTimeval;  

  // length of the packet
  unsigned long ulLength;  
};

class c_prx
{
  /* *****
   * * Member variables
   * *****
   */

  protected:
  
  // a pool of pre-allocated entries
  s_prx_entry *psPoolOfEntries;    // pointer to the pool 
  unsigned long ulSizeOfPool;      // number of entries available in the pool
  unsigned long ulUsedInPool;      // number of entries in the pool that have been used

  // the linked list of packets 
  s_prx_entry *psFirstEntry;  // pointer to the first link
  s_prx_entry *psLastEntry;   // pointer to the last  link
  
  char *pcOutputFilename;  // filename for the file in which to write the results
  int   iPacketCount;      // number of packets to capture

  unsigned long ulStampOffset;  // offset of the stamp within the packet

  /* *****
   * * Member functions
   * *****
   */

  public:

  /* The constructor and destructor.
   */
  c_prx(unsigned long ulStampOffset);
  virtual ~c_prx();

  /* Set certain parameters.
   */
  void setOutputFilename(char *pcOutputFilename);
  void setPacketCount(int iPacketCount);
  void preAllocate(unsigned long ulSizeOfPool);

  /* Start and process a capture.
   */
  virtual void start() = NULL;
  unsigned long process();

  /* Add a packet to the list of received packets.
   */
  void add(const unsigned char *pucPacketData, unsigned long ulPacketLength, const struct timeval *psTimeval);
};

#endif

