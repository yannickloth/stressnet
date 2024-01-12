/***************************************************************************
*   Copyright (C) 2005 by Yannick Loth   *
*   yannick@loth.be   *
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
#include <stdlib.h>
#include "timing.h"
#define COUNT 1000000
double measureGettimeofday()
{
	int i;
	struct timeval tv1;
	struct timeval tv2;
	double dTv2;
	gettimeofday( &tv1, NULL );
	for ( i = 0;i < COUNT;++i )
	{
		gettimeofday( &tv2, NULL );
		dTv2 = tv2.tv_sec + tv2.tv_usec * 1e-6;
	}
	timevalSub( &tv2, &tv1 );
	return ( ( double ) ( tv2.tv_sec + tv2.tv_usec * 1e-6 ) / COUNT );//division after other operations to avoid precision loss due to division of small number through big number
}
void timevalSub ( struct timeval *to, struct timeval * const val )
{
	to->tv_sec -= val->tv_sec;
	to->tv_usec -= val->tv_usec;
	//timevalfix
	if ( to->tv_usec < 0 )
	{
		to->tv_sec--;
		to->tv_usec += 1000000;
	}
	if ( to->tv_usec >= 1000000 )
	{
		to->tv_sec++;
		to->tv_usec -= 1000000;
	}
}
void timevalAdd ( struct timeval *to, struct timeval *val )
{
	to->tv_sec += val->tv_sec;
	to->tv_usec += val->tv_usec;

	//  timevalfix
	if ( to->tv_usec < 0 )
	{
		to->tv_sec--;
		to->tv_usec += 1000000;
	}
	if ( to->tv_usec >= 1000000 )
	{
		to->tv_sec++;
		to->tv_usec -= 1000000;
	}
}
void calculateOffset( int *pLengthArray, struct timeval *tArray, int tArrayLength, int bitrate)
{
	double brate;
	double delta;
	double intermediate;
	int i;
	int offset; //offset in microseconds, relative to the previous packet
	brate = ( double ) bitrate;
	intermediate=8*1e3;
	delta = intermediate/brate;//intermediate to avoid loss of precision due to numerical division of small number through big number

	/*Note that we've calculated all constants for the loop out of the loop to gain processing time*/
	div_t divstruct;
	for ( i = 0;i < tArrayLength;++i )
	{
		offset = ( int ) ( ( pLengthArray[ i ] + 4 ) * delta ); //+4 because of FCS/CRC appended to ethernet packet by the datalink layer
		if ( offset >= 1000000 )
		{
			divstruct=div(offset,1000000);
			tArray[ i ].tv_sec = divstruct.quot; //transform the offset in (sec + usec) instead of only too much usec.
			tArray[ i ].tv_usec = divstruct.rem/*offset % 1000000*/;
		}
		else
		{
			tArray[ i ].tv_sec = 0;
			tArray[ i ].tv_usec = offset;
		}
	}
}

