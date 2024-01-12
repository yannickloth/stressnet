/**************************************************************************
*   Copyright (C) 2005 by Yannick Loth   				                  *
*   yannick@loth.be  							                          *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
***************************************************************************/
//gengetopt command: gengetopt -iinput.txt --conf-parser

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netpacket/packet.h>
#include <unistd.h> //for close()
#include <netinet/in.h>
#include <linux/if_ether.h>
#include <net/ethernet.h>
#include <netinet/ether.h> //for ether_ntoa()
#include <sys/mman.h>
#include <assert.h>
#include <net/if.h>
#include "cmdline.h"
#include "dumpread.h"
#include "licenseinfo.h"
#include "memmanagement.h"
#include "processproperties.h"
#include "timing.h"

int main( int argc, char *argv[] )
{
	int bufferLength;
	int bytesM;
	int bytesN;
	int quantityM;
	int quantityN;
	int i;
	int position;
	int bindResult;
	int sendtoResult;
	int setsockoptResult;
	struct timeval refTime;
	struct timeval now;
	struct timeval startTimeOffset;
	double dRefTime;
	double dNow;
	double gettimeofdayTime;
	struct ether_header *pEtherHeader;
	struct ether_addr *pEtherAddr;
	struct gengetopt_args_info args_info;
	char **packetPointer;
	double *dOffsetArray;
	double *dTimingArray;
	int *lengthArray;
	int *positionArray;
	struct timeval *timingArray;
	int *sent;
	
	/* Process command line arguments */
	//Cmdline parser
	if ( cmdline_parser ( argc, argv, &args_info ) != 0 )
		exit( 1 ) ;
	//Config file parser
	if ( args_info.configfile_arg != NULL )
		if ( cmdline_parser_configfile ( args_info.configfile_arg, &args_info, 1 ) != 0 )
			exit( 1 );
	
	/*Print GPL info*/
	if(!args_info.stats_given)
		printGPLText(2005);
	
	/*Determine for each file the quantity of packets that'll be sent*/
	args_info.quantity_arg = args_info.quantity_arg;
	if ( args_info.packetfileN_given )
	{
		div_t divisionResult;//Integer division faster with div() than with /
		divisionResult=div(args_info.quantity_arg,args_info.ratio_arg+1);
		quantityM = divisionResult.quot * args_info.ratio_arg + divisionResult.rem;
		quantityN = divisionResult.quot;
	}
	else
	{
		quantityM = args_info.quantity_arg;
		quantityN = 0;
	}

	/*Initialise the variables*/
	lengthArray=(int *)callocBuffer(args_info.quantity_arg,sizeof(int));
	timingArray = (struct timeval *)callocBuffer(args_info.quantity_arg,sizeof(struct timeval)); //if int const * or const int *, then the pointed variable is constant -> not what we want!
	dOffsetArray=(double *)callocBuffer(args_info.quantity_arg,sizeof(double));
	dTimingArray = (double *)callocBuffer(args_info.quantity_arg, sizeof(double));
	positionArray = (int *)callocBuffer(args_info.quantity_arg, sizeof(int));
	packetPointer = (char **)callocBuffer(args_info.quantity_arg, sizeof(char *));
	sent=(int *)callocBuffer(args_info.quantity_arg,sizeof(int));
	bufferLength = 0;
	bytesM = 0;
	bytesN = 0;
	dNow=0;
	gettimeofdayTime = measureGettimeofday()/2;

	/*Determine the length of the data buffer*/
	if(!args_info.stats_given)
		printf("Reading stats from file(s). This may take a while.\n");
	readDumpfileStats( args_info.packetfileM_arg, quantityM, &bytesM );
	if ( args_info.packetfileN_given )
		readDumpfileStats( args_info.packetfileN_arg, quantityN, &bytesN );
	bufferLength = bytesM + bytesN;
	if(args_info.stats_given)
	{
		printf("Number of bytes to send: %d\n",bufferLength);
		exit(1);
	}
	printf( "Buffer length: %d\n", bufferLength );
	char * const dataBuffer = new char[ bufferLength ];

	/*Now put the packets in the buffer*/
	readDumpfilePackets( args_info.packetfileM_arg, args_info.packetfileN_arg, args_info.quantity_arg, quantityN, args_info.ratio_arg, dataBuffer, timingArray, lengthArray );
	printf( "Number of packets to send: %d\n", args_info.quantity_arg );
	//Calculate positions of each packet in data-buffer
	packetPointer[ 0 ] = dataBuffer;
	for ( i = 1;i < args_info.quantity_arg;++i )
	{
		positionArray[ i ] = positionArray[ i - 1 ] + lengthArray[ i - 1 ];
		packetPointer[ i ] = dataBuffer + positionArray[ i ];
	}
	
	/*Check wether the addresses in positionArray are within dataBuffer*/
	for ( i = 0;i < args_info.quantity_arg;++i )
	{
		if (packetPointer[ i ] < dataBuffer)
		{
			printf("Packet %d address before buffer!\n",i);
			exit(1);
		}
		else if (packetPointer [ i ] > (dataBuffer + bufferLength - lengthArray[args_info.quantity_arg-1]))
		{
			printf("Packet %d address after buffer!\n",i);
			exit(1);
		}
	}
	
	//Modify ethernet addresses to unique address if any given as argument
	if(args_info.destMAC_given!=0)
		for ( i = 0;i < args_info.quantity_arg;++i )
		{
			pEtherHeader=(ether_header *)packetPointer[i];
			pEtherAddr=ether_aton(args_info.destMAC_arg);
			memcpy(pEtherHeader->ether_dhost,pEtherAddr->ether_addr_octet,ETH_ALEN);
		}
	free(positionArray);//Array not needed any more
	//Calculate timings
	calculateOffset( lengthArray, timingArray, args_info.quantity_arg, args_info.bitrate_arg);
	for ( i = 0 ; i < args_info.quantity_arg ; ++i )
	{
		dOffsetArray[i]=timingArray[i].tv_sec+timingArray[i].tv_usec*1e-6;
	}
	//On modern hardware, 1s should be enough to do all calculations before send
	startTimeOffset.tv_sec = 1;
	startTimeOffset.tv_usec = 0;
	gettimeofday( &refTime, NULL );
	timevalAdd( &refTime, &startTimeOffset );
	dRefTime = refTime.tv_sec + refTime.tv_usec * 1e-6;
	//Calculate sending times
	printf("\tCalculating sending times...\n");
	for ( i = 0;i < args_info.quantity_arg;++i )
	{
		dRefTime += timingArray[ i ].tv_sec + timingArray[ i ].tv_usec * 1e-6;
		dTimingArray[ i ] = dRefTime-gettimeofdayTime;
	}
	/*Lock buffers in physical memory (=disable swapping of these buffers)*/
	mlock( dataBuffer, bufferLength );
	mlock( dTimingArray, args_info.quantity_arg * sizeof( double ) );
	mlock( lengthArray, args_info.quantity_arg * sizeof( int ) );
	mlock( packetPointer, args_info.quantity_arg * sizeof( char * ) );
	mlock( sent, args_info.quantity_arg * sizeof( int ) );
	/*Create the socket and send packets*/
	printf("\tCreating the socket...\n");
	int socketFileDescriptor;
	struct sockaddr to;
	int tolen;
	//Create the SOCK_PACKET socket:
	socketFileDescriptor = socket( PF_PACKET, SOCK_RAW, htons( ETH_P_ALL ) );
	assert( socketFileDescriptor >= 0 );
	printf("\tSocket successfully created...\n");
	//Set socket options:
	int optval;
	optval=(int)2.1*bufferLength; //set enough kernel buffer space
	setsockoptResult=setsockopt(socketFileDescriptor, SOL_SOCKET, SO_SNDBUF, &optval, sizeof(int));
	//Bind the socket to the desired interface(necessary to know where to send the packets):
	struct sockaddr_ll sSockAddr;
	unsigned int sockaddr_llSize;
	sockaddr_llSize=sizeof(struct sockaddr_ll);
	memset( &sSockAddr, '\0', sizeof( struct sockaddr_ll ) );
    sSockAddr.sll_family = AF_PACKET;
	sSockAddr.sll_protocol = htons(ETH_P_ALL);
	sSockAddr.sll_ifindex = if_nametoindex(args_info.interface_arg);
	bindResult = bind( socketFileDescriptor, (struct sockaddr *) &sSockAddr, sizeof( sSockAddr ) );
	assert( bindResult >= 0 );
	printf("\tSocket successfully bound to device %s...\n",args_info.interface_arg);
	//Prepares for sendto()
	memset( &to , '\0', sizeof( to ) );
	to.sa_family = AF_INET;
	strcpy( to.sa_data, args_info.interface_arg );
	tolen = sizeof( to );
	position = 0;
	//Change scheduling policy and priority
	if (args_info.priority_arg>0)
	{
		printf("\tChanging scheduling policy (to FIFO) and priority...\n");
		setSchedulingPolicyFIFO( args_info.priority_arg );
	}
	int count;
	count=0;
	int errorOccured;
	errorOccured=0;
	double dTimePreviousPacket;
	dTimePreviousPacket = 0.0;
	printf("\tSending packets...\n\n");
	if(args_info.bitrate_arg!=0)//0 for maximum speed (no busy-waiting)
	{
		for ( i = 0;i < args_info.quantity_arg;++i )
		{
			/*Timing 'à la Yannick' (absolute, precalculated)*/
			for(;dNow < dTimingArray[ i ];)
			{
				gettimeofday( &now, NULL );
				dNow = now.tv_sec + now.tv_usec * 1e-6;
			}

			/*if (dOffsetArray[i]>gettimeofdayTime)
			{
				//Timing 'à la Wim'...(relative to last sent packet)
				double dTimeslotEnd = dTimePreviousPacket + dOffsetArray[i];
				double dTimeNow = 0.0;
	    		do
	     		{
	        		gettimeofday(&now, NULL);
	        		dTimeNow = now.tv_sec + now.tv_usec/1.0e6;
	      		} while (dTimeNow < ( dTimeslotEnd - gettimeofdayTime ) );
	 			dTimePreviousPacket = dTimeNow;
			}*/
			
			//sendtoResult = sendto( socketFileDescriptor,  packetPointer[ i ], lengthArray[ i ], 0, (struct sockaddr *)&sSockAddr, sockaddr_llSize );
			sendtoResult = write( socketFileDescriptor,  packetPointer[ i ], lengthArray[ i ]);//even if not a connected socket, write works because it works with ANY file descriptor
			if (sendtoResult < 0 && errorOccured==0)
			{
				errorOccured=1;
				perror("sendto() return-value < 0 ");
				printf("\tIf error 'No buffer space available',\n\ttry to put a unique ethernet address");
			}
			else
			{
				++count;
				++sent[i];
			}
		}
	}
	else //0 for maximum speed (no busy-waiting)
	{
		for ( i = 0;i < args_info.quantity_arg;++i )
		{
			//No waiting...
			//sendtoResult = sendto( socketFileDescriptor, packetPointer[ i ], lengthArray[ i ], 0, &to, tolen );
			sendtoResult = write( socketFileDescriptor,  packetPointer[ i ], lengthArray[ i ]);
			//errors might happen frequently because application might send packets to datalink layer faster than the device sends the packets to the network
			if (sendtoResult>=0)
			{
				++count;
				++sent[i];
			}
			else if (errorOccured==0) //only prints the error if the previous packet was sent successfully
			{
				errorOccured=1;
				perror("sendto() return-value < 0 ");
				printf("\tIf error 'No buffer space available',\n\ttry to put a unique ethernet address.\n");
			}
		}
	}
	printf("\nNumber of packets sent: %d\n",count);
	int bytesSent;
	bytesSent=0;
	for(i=0;i<args_info.quantity_arg;++i)
	{
		if (sent[i]!=0)
			bytesSent+=lengthArray[i];
	}
	printf("Number of bytes sent: %d\n",bytesSent);
	setSchedulingPolicyFIFO( 0 );
	munlock( dataBuffer, bufferLength );
	munlock( dTimingArray, args_info.quantity_arg * sizeof( double ) );
	munlock( lengthArray, args_info.quantity_arg * sizeof( int ) );
	munlock( packetPointer, args_info.quantity_arg * sizeof( char * ) );
	munlock( sent, args_info.quantity_arg * sizeof( int ) );
	
	/*Close everything opened and free memory*/
	free(lengthArray);
	free(dTimingArray);
	free(dataBuffer);
	free(packetPointer);
	free(timingArray);
	free(dOffsetArray);
	close( socketFileDescriptor );
	printf("%s exited.\n",argv[0]);
	printf("****************************************\n");
	return EXIT_SUCCESS;
}

