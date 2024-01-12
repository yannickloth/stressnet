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

#include "c_ipaddr.hh"

c_ipaddr::c_ipaddr(uint8 *pucAddr)
        : c_addr(pucAddr, 4)
{ }

void c_ipaddr::set(uint8 byte0, uint8 byte1, uint8 byte2, uint8 byte3)
{
  byte(0, byte0);
  byte(1, byte1);
  byte(2, byte2);
  byte(3, byte3);
}

