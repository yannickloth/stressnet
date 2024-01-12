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

#ifndef S_STAMP_HH
#define S_STAMP_HH

#include <sys/time.h>  // for gettimeofday(), ...
#include "types.hh"

/* An s_stamp structure is placed in the data part
 * of every packet that is transmitted, just before
 * it is written to the transmission socket.
 * The receiver extracts and interprets the information
 * from the s_stamp.
 */

struct s_stamp
{
  // fixed signature indicating the presence 
  // of a "s_stamp" structure; its value is given
  // below in the form of a constant.
  uint16 uiSignature;  

  // type of message
  uint16 uiType;

  // packet counter containing the index of this 
  // packet in the sequence of packets that is transmitted.
  unsigned long ulCounter; 
 
  // time stamp indicating when the packet was transmitted
  struct timeval sTimeval;  
};

// the value that is used for the fixed signature
// in s_stamp.
const uint16 uiStampSignature = 0xF140;

// some type values
const uint16 uiTypeCalibrate   = 0x00;
const uint16 uiTypeMeasurement = 0x01;


// position of the s_stamp structure in the packet,
// must be at least:
// - Ethernet header: 14 bytes
// -       IP header: 20 bytes
// -      UDP header:  8 bytes
// - -------------------------
// -             sum: 42 bytes
//
// the length of the s_stamp structure is:
// - uint16 uiSignature:       2 bytes
// - uint16 ucType:            2 byte
// - unsigned long ulCounter:  4 bytes
// - struct timeval sTimeval:  8 bytes
// - -------------------------
// -                     sum: 16 bytes
//
// the minimum length of an Ethernet frame
// is 64 bytes, with the last 4 bytes being
// used for the FCS.
// therefore the offset can
// vary between 42 and 60-16 = 44 bytes.
const unsigned long ulStampOffsetRaw = 42;

// position of the s_stamp structure in a UDP packet,
// this time only counting the data part of a UDP packet.
const unsigned long ulStampOffsetUdp = 0;

#endif



