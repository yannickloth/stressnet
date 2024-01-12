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

#ifndef C_PTX_RAW_HH
#define C_PTX_RAW_HH

#include "c_ptx.hh"

class c_ptx_raw
    : public c_ptx
{
  /* *****
   * * Member variables
   * *****
   */

  protected:
  
  char *pcDevice;  // the network interface to send packets on
  
  /* *****
   * * Member functions
   * *****
   */

  public:

  /* The constructor and destructor.
   */
  c_ptx_raw();
  virtual ~c_ptx_raw();

  /* Set certain parameters.
   */
  void setInterface(char *pcDevice);

  /* Transmit a given number of packets, 
   * while looping over the linked list
   * with available packets.
   */
  void transmit();
};

#endif

