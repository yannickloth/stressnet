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

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

extern "C" 
{ 
#include <pcap.h> 
}

#include <string.h>  // for memcpy()
#include <iostream>
#include <fstream>  // for ofstream
#include <assert.h>  // for assert()

using namespace std;

#include "c_prx_raw.hh"

c_prx_raw::c_prx_raw()
          :c_prx(ulStampOffsetRaw)
{
  // initialize the member variables
  pcDevice     = "eth0";  // default capture device is first Ethernet interface
  pcFilterString = NULL;  // no default bpf filter
  iSnapLength  =  1514;  // max Ethernet size (without FCS which is not captured anyway)
  iPromiscuous =  1;     // place interface in promiscuous mode
  iTimeout     =  1000;  // (milliseconds)
}

c_prx_raw::~c_prx_raw()
{
}

void c_prx_raw::setInterface(char *pcDevice)
{
  this->pcDevice = pcDevice;
}

void c_prx_raw::setFilterString(char *pcFilterString)
{
  this->pcFilterString = pcFilterString;
}

void c_prx_raw::setSnapLength(int iSnapLength)
{
  this->iSnapLength = iSnapLength;
}

void ProcessPacket (u_char *pucUserData, const struct pcap_pkthdr *psPacketHeader, const u_char *pucData)
{
  // convert userdata pointer to a pointer to the c_prx object
  // that launched this capture.
  c_prx *poPrx = (c_prx *) pucUserData;
  
  // process this packet
  // (note: we pass "->len" instead of "->caplen" since we only
  //  copy the stamp and store the length of the full packet on
  //  the wire; we don't care about the fact that we didn't
  //  capture the complete length of the packet)
  poPrx->add(pucData, psPacketHeader->len, &(psPacketHeader->ts));
}

void c_prx_raw::start()
{
  int  iResult;
  char acErrorMessage [PCAP_ERRBUF_SIZE] ={0};

  // select the device to sniff on
  if (pcDevice == NULL)
  {
    // find a valid device to sniff on
    pcDevice = pcap_lookupdev (acErrorMessage);
    assert(pcDevice != NULL);
  }

  // get the network address and the network mask for this device
  bpf_u_int32 uiNetworkAddress;  
  bpf_u_int32 uiNetworkMask;  
  iResult = pcap_lookupnet (pcDevice, &uiNetworkAddress, &uiNetworkMask, acErrorMessage);
  assert(iResult>=0);

  // open the device for sniffing
  pcap_t *psDescriptor = pcap_open_live (pcDevice, iSnapLength, iPromiscuous, iTimeout, acErrorMessage);
  assert(psDescriptor != NULL);

  // define a capture filter
  struct bpf_program sFilterProgram;
  int                iFlagOptimize  =1;
  iResult = pcap_compile (psDescriptor, &sFilterProgram, pcFilterString, iFlagOptimize, uiNetworkMask);
  assert(iResult>=0);
  iResult = pcap_setfilter (psDescriptor, &sFilterProgram);
  assert(iResult>=0);

  // start the capture
  pcap_handler   printer      = ProcessPacket;
  unsigned char *pucUserData  = (unsigned char *) this;
  iResult = pcap_loop (psDescriptor, iPacketCount, printer, pucUserData);
  assert(iResult>=0);
} 

