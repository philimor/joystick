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

// Definition of class classJS (joystick)
// iButtonPressed :id of the button currently pressed
// iTimeStamp : first byte read from de device driver
// bPressed: True if the button is pressed, False otherwise
// iButtonAnaPos : analogic position of the joystick
// ibuttonPos : discrete position of the cross or joystick :middel, up, down, left right

typedef struct  {
	int iButtonPressed;
	int iTimeStamp;
	bool bPressed;
	int iButtonAnaPos;
	int iButtonPos;
} classJS;
