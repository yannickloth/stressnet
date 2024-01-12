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

#ifndef C_TCP_HH
#define C_TCP_HH

#include "c_pdu.hh"
#include "c_ip.hh"

class c_tcp
    : public c_pdu
{
  /* *****
   * * Member variables
   * *****
   */

  protected:

  uint16   *puiSrcePort;
  uint16   *puiDestPort;
  uint32   *puiSequenceNumber;
  uint32   *puiAcknowledgementNumber;
  uint16   *puiWindowSize;
  uint16   *puiLength;
  uint16   *puiChecksum;

  /* *****
   * * Member functions
   * *****
   */

  public:

  /* The constructor and destructor
   */
  c_tcp(c_pdu *poPayload);
  
  /* Get and set the source and destination port.
   */
  uint16 srcePort();
  void   srcePort(uint16 uiNewSrcePort);
  uint16 destPort();
  void   destPort(uint16 uiNewDestPort);

  /* Get and set the length.
   */
  uint16 length();
  void   length(uint16 uiNewLength);

  /* Get, set or compute the checksum.
   */
  uint16 checksum();
  void   checksum(uint16 uiNewChecksum);
  uint16 computeChecksum(c_ip *poIp);
};

#endif

