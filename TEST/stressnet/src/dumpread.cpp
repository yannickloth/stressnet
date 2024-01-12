/***************************************************************************
*   Copyright (C) 2005 by Yannick Loth                                    *
*   yannick@loth.be                                                       *
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
#include "dumpread.h"
void readDumpfileStats( char * const file, int quantity, int * const length )
{
	/*Locals*/
	pcap_t * descr;
	char errbuf[ PCAP_ERRBUF_SIZE ];
	int i;
	struct pcap_pkthdr *packetHeader;
	const u_char * packetData; //the value pointed to is constant, not the pointer!
	int bytes;
	int result;

	/*Initialise buffers and variables*/
	memset( errbuf, 0, PCAP_ERRBUF_SIZE ); //initialise the buffer to all zeros
	i = 0;
	bytes = 0;
	result = 0;

	/*Begin reading the file*/
	descr = pcap_open_offline( file, errbuf );
	if ( descr == NULL )
	{
		printf( "%s\n", errbuf );
		exit( 1 );
	}

	for ( i = 0;i < quantity;++i )
	{
		result = pcap_next_ex ( descr, &packetHeader, &packetData );
		if ( ( result != -2 ) && ( result != -1 ) )
			bytes += packetHeader->caplen;
		else
		{
			pcap_close( descr );
			descr = pcap_open_offline( file, errbuf );
			if ( descr == NULL )
			{
				printf( "%s\n", errbuf );
				exit( 1 );
			}
			result = pcap_next_ex ( descr, &packetHeader, &packetData );
			if ( ( result != -2 ) && ( result != -1 ) )
				bytes += packetHeader->caplen;
			else
			{
				printf( "Error obtaining packets from the file.\n" );
				exit( 1 );
			}
		}
	}
	( *length ) = bytes;
	pcap_close( descr );
}
void readDumpfilePackets( char * const fileM, char * const fileN, int totalQuantity,
                          int quantityN, int ratio, char * const dataBuffer, struct timeval * const timingArray, int * const lengthArray )
{
	/*Locals*/
	pcap_t *descrM;
	pcap_t *descrN;
	char errbuf[ PCAP_ERRBUF_SIZE ];
	int i;
	struct pcap_pkthdr *packetHeader;
	const u_char *packetData;
	int bytes;
	int result;
	int counter;
	div_t divisionResult;
	div_t divisionResult10th;
	/*Initialise buffers and variables*/
	memset( errbuf, 0, PCAP_ERRBUF_SIZE ); //initialise the buffer to all zeros
	i = 0;
	bytes = 0;
	result = 0;
	counter = 0;
	descrM = NULL;
	descrN = NULL;
	
	/*divide totalQuantity/10 to print dots each time 10% of packets have been processed*/
	divisionResult10th=div(totalQuantity,10);
	
	/*Printing a message to indicate the the files are being read*/
	if(quantityN!=0)
		printf("The files %s and %s are being processed,\nthis may take a while.\n",fileM, fileN);
	else
		printf("The file %s is being processed,\nthis may take a while.\n",fileM);
	
	/*Begin reading the file*/
	descrM = pcap_open_offline( fileM, errbuf );
	if ( descrM == NULL )
	{
		printf( "%s\n", errbuf );
		exit( 1 );
	}
	if ( quantityN != 0 )
	{
		descrN = pcap_open_offline( fileN, errbuf );
		if ( descrN == NULL )
		{
			printf( "%s\n", errbuf );
			exit( 1 );
		}
	}
	else descrN = NULL;

	while ( counter < totalQuantity )
	{
		for ( i = 0;i < ratio;++i )  //read ratio packets from M
		{
			result = pcap_next_ex ( descrM, &packetHeader, &packetData );
			if ( ( result != -2 ) && ( result != -1 ) )
			{
				memcpy( &dataBuffer[ bytes ], packetData, packetHeader->caplen );
				bytes += packetHeader->caplen;
				memcpy( &timingArray[ counter ], &( packetHeader->ts ), sizeof( struct timeval ) );
				lengthArray[ counter ] = packetHeader->caplen;
			}
			else
			{
				pcap_close( descrM );
				descrM = pcap_open_offline( fileM, errbuf );
				if ( descrM == NULL )
				{
					printf( "%s\n", errbuf );
					exit( 1 );
				}
				result = pcap_next_ex ( descrM, &packetHeader, &packetData );
				if ( ( result != -2 ) && ( result != -1 ) )
				{
					memcpy( &dataBuffer[ bytes ], packetData, packetHeader->caplen );
					bytes += packetHeader->caplen;
					memcpy( &timingArray[ counter ], &( packetHeader->ts ), sizeof( struct timeval ) );
					lengthArray[ counter ] = packetHeader->caplen;
				}
				else
				{
					printf( "Error obtaining packets from the file.\n" );
					exit( 1 );
				}
			}

			if ( ++counter >= totalQuantity ) break;
		}
		if ( ( quantityN != 0 ) && ( counter < totalQuantity ) ) //read 1 packet from N
		{
			result = pcap_next_ex ( descrN, &packetHeader, &packetData );
			if ( ( result != -2 ) && ( result != -1 ) )
			{
				memcpy( &dataBuffer[ bytes ], packetData, packetHeader->caplen );
				bytes += packetHeader->caplen;
				memcpy( &timingArray[ counter ], &( packetHeader->ts ), sizeof( struct timeval ) );
				lengthArray[ counter ] = packetHeader->caplen;
			}
			else
			{
				pcap_close( descrN );
				descrN = pcap_open_offline( fileN, errbuf );
				if ( descrN == NULL )
				{
					printf( "%s\n", errbuf );
					exit( 1 );
				}
				result = pcap_next_ex ( descrN, &packetHeader, &packetData );
				if ( ( result != -2 ) && ( result != -1 ) )
				{
					memcpy( &dataBuffer[ bytes ], packetData, packetHeader->caplen );
					bytes += packetHeader->caplen;
					memcpy( &timingArray[ counter ], &( packetHeader->ts ), sizeof( struct timeval ) );
					lengthArray[ counter ] = packetHeader->caplen;
				}
				else
				{
					printf( "Error obtaining packets from the file.\n" );
					exit( 1 );
				}
			}
			++counter;
		}
		divisionResult=div(counter,divisionResult10th.quot);
		if (divisionResult.rem==0)
		{
			printf("%d%c ",divisionResult.quot*10,'%');
			fflush(stdout);
		}
	}
	printf("\n");
	pcap_close( descrM );
	if ( quantityN != 0 )
		pcap_close( descrN );
}

