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

#ifndef C_DATA_HH
#define C_DATA_HH

#include "c_pdu.hh"

class c_data
    : public c_pdu
{
  /* *****
   * * Member variables
   * *****
   */

  protected:

  unsigned char *pucData;  // pointer to the data
  unsigned long ulLength;  // the length of the data

  /* *****
   * * Member functions
   * *****
   */

  public:

  /* The constructor and destructor
   */
  c_data(unsigned long ulSize);
  virtual ~c_data();

  /* Set the content of the data buffer.
   */
  void set(unsigned char *pucNewData, unsigned long ulNewDataLength);

  /* We override the following functions from c_pdu because
   * we do not have a header nor a payload object.
   */  
  virtual unsigned long pduLength();
  virtual void assemble(unsigned char *pucBufferData, unsigned long ulBufferLength);

};

#endif

