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

#pragma once

#include "CRender.h"
#include "..\Logic\CMap.h"
#include "..\const.h"

void	CRender::RenderAllTiles(CMap *MapPointer)
{
	int	cameraX		=	(int)MapPointer->m_cameraX/32;
	int	cameraY		=	(int)MapPointer->m_cameraY/32;

	int	k =	0, l = 0;

	for (int i = -7; i < 8; i++)
	{
		for (int j = -10; j < 10; j++)
		{
			k = j, l = i;
			k += 10, l += 7;
			RenderImage(k*32,l*32,TYPE_TILE,MapPointer->m_allTilesMap[cameraY+i][cameraX+j]);
		}
	}

	RenderButton("Return",20,WINDOW_HEIGHT,98,0,49);
	RenderImage(230,WINDOW_HEIGHT,TYPE_TILE,MapPointer->m_selectedTile);
}

void	CRender::RenderMapEditor(CMap *MapPointer)
{
	int cameraX = (int)MapPointer->m_cameraX/32;
	int cameraY = (int)MapPointer->m_cameraY/32;

	int k = 0, l = 0;

	for (int i = -7; i < 8; i++)
	{
		for (int j = -10; j < 10; j++)
		{
			k = j, l = i;
			k += 10, l+=7;
			RenderImage(k*32,l*32,TYPE_TILE,MapPointer->m_mapEditor[cameraY+i][cameraX+j]);
			k = 0, l = 0;
		}
	}

	RenderButton("Return",20,WINDOW_HEIGHT,98,0,49);
	RenderButton("All tiles",210,WINDOW_HEIGHT,98,0,49);
	RenderButton("Save map",400,WINDOW_HEIGHT,98,0,49);
}

void	CRender::RenderMap(CMap *MapPointer)
{
	int	cameraX = (int)MapPointer->m_cameraX / 32; // window middle
	int	cameraY = (int)MapPointer->m_cameraY / 32;

	int k = 0, l = 0; // where will the image be drawn if multiplied by 32
	// [cameraY + i][cameraX + j]  which tile needs to be drawn

	for (int i = -7; i < 8; i++)
	{
		for (int j = -10; j < 10; j++)
		{
			k = j, l = i;
			k+=10,l+=7; // we can not use negative numbers
			RenderImage(k*32,l*32,TYPE_TILE,MapPointer->m_map[cameraY+i][cameraX+j]);
			k = 0, l = 0;
		}
	}

	// m_playerX and m_playerY are map locations, we need window locations
	int x = (int)(MapPointer->m_playerX - MapPointer->m_cameraX + WINDOW_WIDTH/2);
	int y = (int)(MapPointer->m_playerY - MapPointer->m_cameraY + WINDOW_HEIGHT/2);

	if (x >= 0 && y >= 0 && x <= WINDOW_WIDTH - TILE_SIZE && y <= WINDOW_HEIGHT - TILE_SIZE) // do not let player be rendered onto bottom screen
		RenderImage(x,y,TYPE_PLAYER,0); // render player

	for (size_t i = 0; i < MapPointer->m_closeEnemyXY.size(); i++)
	{
		x = (int)MapPointer->m_closeEnemyXY[i].x;
		y = (int)MapPointer->m_closeEnemyXY[i].y;
		if (CheckIfInWindow((int)MapPointer->m_cameraX,(int)MapPointer->m_cameraY,x,y))
			RenderImage(x,y,TYPE_ENEMY,MapPointer->m_closeEnemyXY[i].imgID);
			/* You can not use ->m_imageSurface[i] for image that needs to be drawn, because there can be more than one*/
	}

	for (size_t i = 0; i < MapPointer->m_npcXY.size(); i++)
	{
		x = (int)MapPointer->m_npcXY[i].x;
		y = (int)MapPointer->m_npcXY[i].y;
		if (CheckIfInWindow((int)MapPointer->m_cameraX,(int)MapPointer->m_cameraY,x,y))
			RenderImage(x,y,TYPE_NPC,MapPointer->m_npcXY[i].imgID);
	}

	for (size_t i = 0; i < MapPointer->m_spell.size(); i++)
	{
		x = (int)MapPointer->m_spell[i].x;
		y = (int)MapPointer->m_spell[i].y;
		if (CheckIfInWindow((int)MapPointer->m_cameraX,(int)MapPointer->m_cameraY,x,y))
			RenderImage(x,y,TYPE_SPELL,MapPointer->m_spell[i].imgID);
	}
}

void	CRender::RenderHUD(int fps,int hp,int mp)
{
	RenderFPS(fps);
	RenderText("HP:",20,490,98,0,49);
	RenderValue(hp,60,490,98,0,49);
	RenderText("MP:",20,520,98,0,49);
	RenderValue(mp,60,520,98,0,49);
}

void	CRender::SetTextBox(std::string text)
{
	m_text = text;
}

void	CRender::RenderMainMenu()
{
	RenderButton("New game",224,144,25,23,112);
	RenderButton("Map editor",224,208,25,25,112);
	RenderButton("Options",224,272,25,25,112);
	RenderButton("Exit",224,336,25,23,112);
}

bool	CRender::CheckIfInWindow(int cameraX,int cameraY,int &x,int &y)
{
	if (x < (cameraX - WINDOW_WIDTH/2) || x > (cameraX + WINDOW_WIDTH/2) || y < (cameraY - WINDOW_HEIGHT/2) || y > (cameraY + WINDOW_HEIGHT/2))
		return false; // outside
	// ( lets say, 1200X,800Y on map -> and our window has 640,480 )
	// MapX,MapY transform into X and Y for window 
	x-= (cameraX - WINDOW_WIDTH/2);
	y-= (cameraY - WINDOW_HEIGHT/2);
	return true;
}
