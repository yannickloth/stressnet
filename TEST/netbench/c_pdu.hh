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

#ifndef C_PDU_HH
#define C_PDU_HH

#include "types.hh"

class c_pdu;

class c_pdu
{
  /* *****
   * * Member variables
   * *****
   */

  protected:
  
  unsigned char *pucHeaderData;  // pointer to the header data
  unsigned long ulHeaderLength;  // the length of the header

  c_pdu *poPayload;  // pointer to the payload pdu
  
  /* *****
   * * Member functions
   * *****
   */

  public:

  /* The constructor and destructor.
   */
  c_pdu(c_pdu *poPayload);
  virtual ~c_pdu();

  /* The function "length" returns the length of the assembled pdu.
   */  
  virtual unsigned long pduLength();

  /* The function "assemble" builds the pdu in a buffer "pucBufferData"
   * with size "ulBufferLength".
   */
  virtual void assemble(unsigned char *pucBufferData, unsigned long ulBufferLength);

  protected:

  /* Allocate memory for the header.
   */  
  void allocateHeader(unsigned long ulLength);
};

#endif

