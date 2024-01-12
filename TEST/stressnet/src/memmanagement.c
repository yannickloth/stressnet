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
#include "memmanagement.h"
void *callocBuffer(size_t nbElem, size_t elemSize)
{
	void *ptr;
	ptr=calloc(nbElem,elemSize);
	if(ptr==NULL)
	{
		printf("Memory allocation failed. Try to free more memory or send less\nand/or shorter packets.\n");
		exit(1);
	}
	return(ptr);
}
