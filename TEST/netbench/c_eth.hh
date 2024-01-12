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

#ifndef C_ETH_HH
#define C_ETH_HH

#include "c_pdu.hh"
#include "c_macaddr.hh"

class c_eth
    : public c_pdu
{
  /* *****
   * * Member variables
   * *****
   */

  protected:

  c_macaddr *poDestAddr;
  c_macaddr *poSrceAddr;
  uint16    *puiType;

  /* *****
   * * Member functions
   * *****
   */

  public:

  /* The constructor and destructor
   */
  c_eth(c_pdu *poPayload);
  virtual ~c_eth();

  /* The function "length" returns the length of the assembled pdu.
   * We override here the definition in c_pdu so that we can guarantee
   * the minimum length of an Ethernet frame.
   */  
  unsigned long pduLength();

  /* Provide access to the destination and source MAC address.
   */
  c_macaddr *destAddr();
  c_macaddr *srceAddr();

  /* Get and set the EtherType.
   */
  uint16 type();
  void   type(uint16 uiNewType);

};

#endif

