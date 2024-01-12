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

#ifndef C_ADDR_HH
#define C_ADDR_HH

#include "types.hh"

class c_addr
{
  /* *****
   * * Member variables
   * *****
   */

  protected:

  uint8 *pucAddr;
  unsigned long ulSize;

  /* *****
   * * Member functions
   * *****
   */

  public:

  /* The constructor  
   */
  c_addr(uint8 *pucAddr, unsigned long ulSize);

  /* Get and set the address byte by byte.
   */
  uint8 byte(unsigned long ulIndex);
  void  byte(unsigned long ulIndex, uint8 uiValue);

  /* Get and set the address from a sequence of hexadecimal
     or decimal values, separated by a non-digit character.
   */
  void  set(const char *pcString, int iBase);
};

#endif

