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

#include <stdlib.h>  // for exit()
#include <signal.h>  // for signal()
#include <unistd.h>  // for getopt()
#include <pthread.h> // for pthread_...()
#include <assert.h>  // for assert()

#include <iostream>
using namespace std;

#include "c_prx_raw.hh"

void terminateHandler(int iSignal)
{
  // ignore new signals of this type
  signal(SIGINT, SIG_IGN);      
  signal(SIGTERM, SIG_IGN);      
  signal(SIGALRM, SIG_IGN);      

  // ready
  exit(0);
}

int main(int argc, char **argv)
{
  // the packet receiver
  c_prx_raw cPrx;
  cPrx.setPacketCount(1);

  // show license
  cout << argv[0] << " is free software," << endl;
  cout << "  covered by the GNU General Public License (GPL)."  << endl;
  cout << "  You are welcome to change it and/or distribute" << endl;
  cout << "  copies of it under certain conditions." << endl;
  cout << "  There is absolutely no warranty for this program." << endl;
  cout << "  See the definition of the GPL at" << endl;
  cout << "  http://www.gnu.org/copyleft/gpl.html for details." << endl << endl;

  // parse the command-line options
  unsigned int uiAlarmTimeout = 0;
  int c;
  extern char *optarg;
  while	((c = getopt(argc, argv, "i:f:o:w:c:")) != -1)
  {
    switch (c) 
    {
      case 'i':
      {
        cPrx.setInterface(optarg);
      } break;
      case 'f':
      {
        cPrx.setFilterString(optarg);
      } break;
      case 'w':
      {
        uiAlarmTimeout = atoi(optarg);
      } break;
      case 'c':
      {
        cPrx.setPacketCount(atol(optarg));
      } break;
      case '?':
      {
        cerr << "ERROR: parameter \"" << optarg << "\" not implemented." << endl;
        exit(0);
      } break;
    }
  }

  // install signal handlers
  sighandler_t pResult;
  pResult = signal(SIGINT, terminateHandler);
  assert(pResult!=SIG_ERR);
  pResult = signal(SIGTERM, terminateHandler);
  assert(pResult!=SIG_ERR);
  pResult = signal(SIGALRM, terminateHandler);
  assert(pResult!=SIG_ERR);

  // set an alarm 
  if (uiAlarmTimeout >0)
  {
    alarm(uiAlarmTimeout);         
  }

  // run captures until at least one packet with the 
  // correct signature is received
  do
  {
    cPrx.start();
  } while (cPrx.process() < 1);

  // the end
  return 0;
}

