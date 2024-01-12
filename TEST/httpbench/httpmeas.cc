/*
 * httpbench
 * Copyright (C) 2005   Wim Mees <Wim.Mees@rma.ac.be>
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

#include <stdlib.h>  // for exit()
#include <unistd.h>  // for getopt()
#include <assert.h>  // for assert()

#include <netinet/ip.h> 
#include <netinet/tcp.h>

extern "C" 
{ 
#include <pcap.h> 
}

#include <iostream>
#include <fstream>  // for ofstream
using namespace std;

// some default values
char *pcInputFile    = NULL;   // no default input file
char *pcFilterString = NULL;   // no default bpf filter
char *pcOutputFilename = "/dev/stdout";  // show results on stdout
int iShowUrl = 0;  // default is not to show the URLs
ofstream fout;

#define MAX_REQUEST_SIZE 512
typedef struct
{
  unsigned int uiClientIp;
  unsigned int uiServerIp;
  unsigned int uiClientPort;
  unsigned int uiServerPort;
  char acRequest[MAX_REQUEST_SIZE+1];
  unsigned int uiSizeOfReply;
  unsigned long ulTimeStart;
  unsigned long ulTimeEnd;
} sFlowInfo;

#define MAX_FLOWS 100000
sFlowInfo asFlowInfo[MAX_FLOWS];
long lNrOfFlows = 0;

int IsHttpPort(unsigned int uiPort)
{
  return ((uiPort==80) || (uiPort==8080));
}

void ProcessPacket (u_char *pucUserData, const struct pcap_pkthdr *psPacketHeader, const u_char *pucData)
{
  // verify whether we have IP at layer 3 
  unsigned char *psEthernet = (unsigned char *) pucData;
  unsigned int uiEtherType = pucData[12] << 8 + pucData[13];
  if (uiEtherType != 0x0800)  return;

  // verify whether we have TCP at layer 4
  struct ip *psIp = (struct ip *) &psEthernet[14];
  assert(psIp->ip_v==0x04);
  unsigned int uiSrcIp = psIp->ip_src.s_addr;
  unsigned int uiDstIp = psIp->ip_dst.s_addr;
  if (psIp->ip_p != 0x06)  return;

  // verify whether either source or destination port is a HTTP port
  struct tcphdr *psTcp = (struct tcphdr *) ((char *)psIp + psIp->ip_hl*4);
  unsigned int uiSrcPort = ntohs(psTcp->source);
  unsigned int uiDstPort = ntohs(psTcp->dest);
  if ((! IsHttpPort(uiSrcPort)) && (! IsHttpPort(uiDstPort)))  return;

  // look at the HTTP part
  unsigned char *psApplication = (unsigned char *)psTcp + psTcp->doff*4;

  // is it a new client-to-server request ?
  if (IsHttpPort(uiDstPort))
  {
    char *psHit = strstr((const char *) psApplication, "GET");
    if (! psHit)
          psHit = strstr((const char *) psApplication, "POST");
    if (! psHit)
          psHit = strstr((const char *) psApplication, "HEAD");
 
    if (psHit)
    {
      // it's a new request, so create a new entry
      asFlowInfo[lNrOfFlows].uiClientIp    = uiSrcIp;
      asFlowInfo[lNrOfFlows].uiServerIp    = uiDstIp;
      asFlowInfo[lNrOfFlows].uiClientPort  = uiSrcPort;
      asFlowInfo[lNrOfFlows].uiServerPort  = uiDstPort;
      asFlowInfo[lNrOfFlows].uiSizeOfReply = 0;

      // skip the GET/POST/HEAD to the URL
      char *psUrl = strstr(psHit, " ") +1;

      // terminate the string behind the URL
      if (strstr(psUrl, " "))
      { psUrl[strstr(psUrl, " ") - psUrl] = 0; }

      // copy the URL to the new entry
      strncpy((char *) asFlowInfo[lNrOfFlows].acRequest, psUrl, MAX_REQUEST_SIZE);

      // set the start and end time to the time of this packet
      asFlowInfo[lNrOfFlows].ulTimeStart = psPacketHeader->ts.tv_sec * 1000000L + psPacketHeader->ts.tv_usec;
      asFlowInfo[lNrOfFlows].ulTimeEnd   = asFlowInfo[lNrOfFlows].ulTimeStart;

      // increase the "nr of flows" counter
      lNrOfFlows++;
      assert(lNrOfFlows<MAX_FLOWS);
    }
  }

  // is it server-to-client data for an existing connection ?
  if ((IsHttpPort(uiSrcPort)) && (lNrOfFlows>0))
  {
    // find the corresponding flow entry
    for ( long i=lNrOfFlows-1 ; i>=0 ; i-- )
    {
      if (    (asFlowInfo[i].uiClientIp   == uiDstIp)
           && (asFlowInfo[i].uiServerIp   == uiSrcIp)
           && (asFlowInfo[i].uiClientPort == uiDstPort)
           && (asFlowInfo[i].uiServerPort == uiSrcPort)
         )
        {
          asFlowInfo[i].uiSizeOfReply += psPacketHeader->len -14 -psIp->ip_hl*4 - psTcp->doff*4;
          asFlowInfo[i].ulTimeEnd      = psPacketHeader->ts.tv_sec * 1000000L + psPacketHeader->ts.tv_usec;
          break;
        }
    }
  }
}

int ParseFile()
{
  // start the capture
  int  iResult;
  char acErrorMessage [PCAP_ERRBUF_SIZE] ={0};

  // open the file for parsing
  pcap_t *psDescriptor = pcap_open_offline (pcInputFile, acErrorMessage);
  assert(psDescriptor != NULL);

  // define a capture filter
  struct bpf_program sFilterProgram;
  int                iFlagOptimize  =1;
  iResult = pcap_compile (psDescriptor, &sFilterProgram, pcFilterString, iFlagOptimize, 0);
  assert(iResult>=0);
  iResult = pcap_setfilter (psDescriptor, &sFilterProgram);
  assert(iResult>=0);

  // start the capture
  pcap_handler   printer      = ProcessPacket;
  unsigned char *pucUserData  = (unsigned char *) NULL;
  iResult = pcap_loop (psDescriptor, 0, printer, pucUserData);
  assert(iResult>=0);

  // ready
  return 0;
}

int ParseFlows()
{
  // dump all the flows in a format that can be used by httpclient
  for ( long i=0 ; i<lNrOfFlows ; i++ )
  {
    fout << (asFlowInfo[i].ulTimeStart - asFlowInfo[0].ulTimeStart) / 1.0e6 << " "
         <<  asFlowInfo[i].uiSizeOfReply << " "
         << (asFlowInfo[i].ulTimeEnd   - asFlowInfo[i].ulTimeStart) / 1.0e6;

    if (iShowUrl)
    { fout << " " << asFlowInfo[i].acRequest; }

    fout << endl;
  }

  // ready
  return 0;
}

int main(int argc, char **argv)
{
  // parse the command-line options
  int c;
  extern char *optarg;
  while	((c = getopt(argc, argv, "i:f:o:uh")) != -1)
  {
    switch (c) 
    {
      case 'i':
      { pcInputFile = optarg;
      } break;
      case 'f':
      { pcFilterString = optarg;
      } break;
      case 'o':
      { pcOutputFilename = optarg;
      } break;
      case 'u':
      { iShowUrl = 1;
      } break;
      default:
      { cout << "Usage: " << argv[0] << " [options]" << endl;
        cout << "Options:" << endl;
        cout << "  -i input-file      (required)" << endl;
        cout << "  -f filter-string   (default is none)" << endl;
        cout << "  -o output-file     (default is stdout)" << endl;
        cout << "  -u                 (default is not to show URLs)" << endl;
        exit(0);
      } break;
    }
  }

  // open the output stream
  fout.open(pcOutputFilename, ios::out);
  assert(! fout.bad());

  // set precision (important !)
  // (so we store the timestamps up to ms precision)
  fout.precision(14);

  // parse the file      
  int iResult = ParseFile();
  assert(iResult==0);

  // process the TCP/HTTP flows
  iResult = ParseFlows();
  assert(iResult==0);

  // clean up 
  fout.close();

  // the end
  return 0;
}

