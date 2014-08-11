/***************************************************************************
 *   Copyright (C) 2012 by philippe   *
 *   philippe@localhost.localdomain   *
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
#include <stdio.h>
#include <stdbool.h>

#include "common.h"
#include "decode.h"

/* Constantes et MACROS */

/* Structures */

/* Variables Globales */

/* Fonctions */

// DecodeButton return set the attributes of the object objJS:
// iButtonPressed is the code related to the button pressed
// iTimeStamp is the time information coded in 32 bits.
// return value : 0
int DecodeButton(int EndPortTab[], classJS * objJS)
{
	int iPos;
	
	objJS->iButtonPressed = 0;
	
	// Buttons #1 to 10
	if ((EndPortTab[1] & 0x00FF0000) == 0x00010000)
	{
		objJS->iButtonPressed = ((EndPortTab[1] & 0xFF000000) >> 24)+1;
		objJS->bPressed = (EndPortTab[1] & 0x00000001);
	} 
	// else buttons are Cross (11) or JS left (12) or JS right (13)
	else
	{
		// Get the analogic position of the cross or JS
		iPos = (EndPortTab[1] & 0x0000FFFF);
		objJS->iButtonAnaPos = iPos;
		objJS->bPressed = true;
		
		switch (EndPortTab[1] & 0xFFFF0000)
		{	// Vertical Cross
			case 0x05020000:
				objJS->iButtonPressed = 11;
				switch (iPos)
				{
					case 0x8001:
						objJS->iButtonPos = UP;
						break;	
					case 0x7FFF:
						objJS->iButtonPos = DOWN;
						break;
					case 0x0000:
						objJS->iButtonPos = MIDDLE;
						objJS->bPressed = false;
				}
				break;
			// Horizontal Cross	
			case 0x04020000:
				objJS->iButtonPressed = 11;
				switch (iPos)
				{
					case 0x8001:
						objJS->iButtonPos = LEFT;
						break;	
					case 0x7FFF:
						objJS->iButtonPos = RIGHT;
						break;
					case 0x0000:
						objJS->iButtonPos = MIDDLE;
						objJS->bPressed = false;
				}
				break;
			// Left Horizontal JS	
			case 0x00020000:
				objJS->iButtonPressed = 12;
				switch (iPos)
				{
					case 0x8001:
						objJS->iButtonPos = LEFT;
						break;	
					case 0x7FFF:
						objJS->iButtonPos = RIGHT;
						break;
					case 0x0000:
						objJS->iButtonPos = MIDDLE;
						objJS->bPressed = false;
						break;
				}
				break;
			// Left Vertical JS	
			case 0x01020000:
				objJS->iButtonPressed = 12;
				switch (iPos)
				{
					case 0x8001:
						objJS->iButtonPos = UP;
						break;	
					case 0x7FFF:
						objJS->iButtonPos = DOWN;
						break;
					case 0x0000:
						objJS->iButtonPos = MIDDLE;
						objJS->bPressed = false;
						break;
				}
				break;
			// Right Horizontal JS	
			case 0x03020000:
				objJS->iButtonPressed = 13;
				switch (iPos)
				{
					case 0x8001:
						objJS->iButtonPos = LEFT;
						break;	
					case 0x7FFF:
						objJS->iButtonPos = RIGHT;
						break;
					case 0x0000:
						objJS->iButtonPos = MIDDLE;
						objJS->bPressed = false;
						break;
				}
				break;
			// Right Vertical JS	
			case 0x02020000:
				objJS->iButtonPressed = 13;
				switch (iPos)
				{
					case 0x8001:
						objJS->iButtonPos = UP;
						break;	
					case 0x7FFF:
						objJS->iButtonPos = DOWN;
						break;
					case 0x0000:
						objJS->iButtonPos = MIDDLE;
						objJS->bPressed = false;
						break;
				}
				break;
		}
	}
	objJS->iTimeStamp = EndPortTab[0];
	return (0);
}
