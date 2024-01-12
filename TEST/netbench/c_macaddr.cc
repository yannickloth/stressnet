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

#include <stdlib.h>  // for rand()

#include "c_macaddr.hh"

c_macaddr::c_macaddr(uint8 *pucAddr)
         : c_addr(pucAddr, 6)
{ }

void c_macaddr::set(uint8 byte0, uint8 byte1, uint8 byte2, uint8 byte3, uint8 byte4, uint8 byte5)
{
  byte(0, byte0);
  byte(1, byte1);
  byte(2, byte2);
  byte(3, byte3);
  byte(4, byte4);
  byte(5, byte5);
}

void c_macaddr::set(const char *pcString)
{
  c_addr::set(pcString, 16);
}

void c_macaddr::setBroadcast()
{
  set (0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
}

void c_macaddr::randomize()
{
  // (don't touch the first byte since it determine uni/multi/broadcast
  for ( int i=1 ; i<6; i++ )
  { byte (i, (byte(i) + rand()) % 0xFF); }
}


