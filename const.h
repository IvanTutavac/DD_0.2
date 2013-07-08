/*
	(c) Copyright 2013 Ivan Tutavac

	This file is part of DD 0.2

    DD 0.2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    DD 0.2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with DD 0.2.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma	once

static const	char* APP_NAME			=	"DD 0.2";
static	int	WINDOW_WIDTH		=	640;
static	int	WINDOW_HEIGHT		=	480;
const	int	HUD_HEIGHT			=	64;
const	int TILE_NUMBER_FINISH	=	2; // last tile img number
const	int TILE_NUMBER_START	=	0; // first tile img number
const	int	MAP_WIDTH			=	6400;
const	int	MAP_HEIGHT			=	4800;
const	int	TILE_SIZE			=	32;

const	int SPELL_NUMBER		=	1;
	
const	int	TYPE_TILE			=	0;
const	int	TYPE_ENEMY			=	1;
const	int	TYPE_PLAYER			=	2;
const	int	TYPE_MENU			=	3;
const	int	TYPE_NPC			=	4;
const	int	TYPE_SPELL			=	5;