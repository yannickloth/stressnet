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

#ifndef C_IP_HH
#define C_IP_HH

#include "c_pdu.hh"
#include "c_ipaddr.hh"

class c_ip
    : public c_pdu
{
  /* *****
   * * Class variables
   * *****
   */

  static uint16 uiIdentificationCounter;

  /* *****
   * * Member variables
   * *****
   */

  protected:

  uint16   *puiLength;
  uint16   *puiIdentification;
  uint16   *puiFragmentOffset;
  uint8    *puiTimeToLive;
  uint8    *puiProtocol;
  uint16   *puiChecksum;
  c_ipaddr *poDestAddr;
  c_ipaddr *poSrceAddr;

  /* *****
   * * Member functions
   * *****
   */

  public:

  /* The constructor and destructor
   */
  c_ip(c_pdu *poPayload);
  virtual ~c_ip();

  /* Get and set the length.
   */
  uint16 length();
  void   length(uint16 uiNewLength);

  /* Get and set the identification.
   */
  uint16 identification();
  void   identification(uint16 uiNewIdentification);

  /* Get and set the fragment offset.
   */
  uint16 fragmentOffset();
  void   fragmentOffset(uint16 uiNewFragmentOffset);

  /* Get and set the time to live (TTL).
   */
  uint8 timeToLive();
  void  timeToLive(uint8 uiNewTimeToLive);

  /* Get and set the protocol.
   */
  uint8 protocol();
  void  protocol(uint8 uiNewProtocol);

  /* Get, set or compute the checksum.
   */
  uint16 checksum();
  void   checksum(uint16 uiNewChecksum);
  uint16 computeChecksum();

  /* Provide access to the destination and source IP address.
   */
  c_ipaddr *destAddr();
  c_ipaddr *srceAddr();
};

#endif

