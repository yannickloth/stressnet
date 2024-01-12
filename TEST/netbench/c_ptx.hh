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

#ifndef C_PTX_HH
#define C_PTX_HH

#include "types.hh"

struct s_ptx_entry;
struct s_ptx_entry
{
  // pointer to the next entry in the linked list
  s_ptx_entry *psNextEntry;

  // length of the packet that follows
  unsigned long ulPacketLength;

  // the packet data
  // (must be the last entry in the structure!)
  unsigned char aucPacketData[];  
};

class c_ptx
{
  /* *****
   * * Member variables
   * *****
   */

  protected:
  
  s_ptx_entry *psFirstEntry;  // pointer to the first link
  s_ptx_entry *psLastEntry;   // pointer to the last  link

  char *pcDevice;  // the network interface to send packets on
  double dBitrate;  // the bitrate at which to send data
  unsigned long ulNumberOfPacketsToTransmit;  // number of packets to send out
 
  unsigned long ulStampOffset;
 
  /* *****
   * * Member functions
   * *****
   */

  public:

  /* The constructor and destructor.
   */
  c_ptx(unsigned long ulStampOffset);
  virtual ~c_ptx();

  /* Set certain parameters.
   */
  void setInterface(char *pcDevice);
  void setBitrate(double dBitrate);
  void setNumberOfPacketsToTransmit (unsigned long ulNumberOfPacketsToTransmit);

  /* Add a packet to the list of packets to transmit.
   */
  void add(unsigned char *pucNewPacketData, unsigned long ulNewPacketLength, uint16 uiType);

  /* Transmit a given number of packets, 
   * while looping over the linked list
   * with available packets.
   */
  virtual void transmit() = NULL;
};

#endif

