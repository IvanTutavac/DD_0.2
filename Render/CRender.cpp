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
#include "..\Logic\Map\CMap.h"
#include "..\const.h"

void	CRender::RenderAllTiles(CMap *MapPointer)
{
	int cameraX = (int)MapPointer->m_cameraX;
	int cameraY = (int)MapPointer->m_cameraY;

	int mapX = cameraX - WINDOW_WIDTH/2;
	int mapY = cameraY - WINDOW_HEIGHT/2;

	int downX = (mapX/TILE_SIZE)*TILE_SIZE;
	int downY = (mapY/TILE_SIZE)*TILE_SIZE;

	for (int y = downY-mapY; y < WINDOW_HEIGHT; y+=TILE_SIZE)
	{
		for (int x = downX-mapX; x < WINDOW_WIDTH; x+=TILE_SIZE)
		{
			VRenderImage(x,y,TILE_SIZE,TILE_SIZE,TYPE_TILE,MapPointer->m_allTilesMap[mapY/32][mapX/32]);
			mapX+=TILE_SIZE;
		}
		mapY+=TILE_SIZE;
		mapX = cameraX - WINDOW_WIDTH/2;
	}

	VClearHUD();

	VRenderButton("Return",20,WINDOW_HEIGHT,98,0,49);
	VRenderImage(230,WINDOW_HEIGHT,TILE_SIZE,TILE_SIZE,TYPE_TILE,MapPointer->m_selectedTile);
}

void	CRender::RenderMapEditor(CMap *MapPointer)
{
	int cameraX = (int)MapPointer->m_cameraX;
	int cameraY = (int)MapPointer->m_cameraY;

	int mapX = cameraX - WINDOW_WIDTH/2;
	int mapY = cameraY - WINDOW_HEIGHT/2;

	int downX = (mapX/TILE_SIZE)*TILE_SIZE;
	int downY = (mapY/TILE_SIZE)*TILE_SIZE;

	for (int y = downY-mapY; y < WINDOW_HEIGHT; y+=TILE_SIZE)
	{
		for (int x = downX-mapX; x < WINDOW_WIDTH; x+=TILE_SIZE)
		{
			VRenderImage(x,y,TILE_SIZE,TILE_SIZE,TYPE_TILE,MapPointer->m_mapEditor[mapY/32][mapX/32]);
			mapX+=TILE_SIZE;
		}
		mapY+=TILE_SIZE;
		mapX = cameraX - WINDOW_WIDTH/2;
	}

	VClearHUD();

	VRenderButton("Return",20,WINDOW_HEIGHT,98,0,49);
	VRenderButton("All tiles",210,WINDOW_HEIGHT,98,0,49);
	VRenderButton("Save map",400,WINDOW_HEIGHT,98,0,49);
}

void	CRender::RenderMap(CMap *MapPointer)
{
	int cameraX = (int)MapPointer->m_cameraX;
	int cameraY = (int)MapPointer->m_cameraY;

	// upper left map location
	int mapX = cameraX - WINDOW_WIDTH/2;
	int mapY = cameraY - WINDOW_HEIGHT/2;

	// for the whole tile (top border tiles and left border tiles) to be drawn downX and downY need to be 0 
	// if it's not zero, we draw a part of it ( x = downX - mapX .... )
	int downX = (mapX/TILE_SIZE)*TILE_SIZE;
	int downY = (mapY/TILE_SIZE)*TILE_SIZE;

	// there's no check for if a tile needs to be cut on bottom border ( VClearHUD should clean those tile parts... )

	for (int y = downY-mapY; y < WINDOW_HEIGHT; y+=TILE_SIZE)
	{
		for (int x = downX-mapX; x < WINDOW_WIDTH; x+=TILE_SIZE)
		{
			VRenderImage(x,y,TYPE_TILE,MapPointer->m_map[mapY/32][mapX/32]);
			mapX+=TILE_SIZE;
		}
		mapY+=TILE_SIZE;
		mapX = cameraX - WINDOW_WIDTH/2;
	}



	// m_playerX and m_playerY are map locations, we need window locations
	int x = (int)(MapPointer->m_playerX); //- MapPointer->m_cameraX + WINDOW_WIDTH/2);
	int y = (int)(MapPointer->m_playerY); //- MapPointer->m_cameraY + WINDOW_HEIGHT/2);

	//if (x >= 0 && y >= 0 && x <= WINDOW_WIDTH - TILE_SIZE && y <= WINDOW_HEIGHT - TILE_SIZE) // do not let player be rendered onto bottom screen
	if (CheckIfInWindowEx((int)MapPointer->m_cameraX,(int)MapPointer->m_cameraY,x,y))	
		VRenderImage(x,y,TILE_SIZE,TILE_SIZE,TYPE_PLAYER,0); // render player

	for (size_t i = 0; i < MapPointer->m_closeEnemyXY.size(); i++)
	{
		x = (int)MapPointer->m_closeEnemyXY[i].x;
		y = (int)MapPointer->m_closeEnemyXY[i].y;
		if (CheckIfInWindowEx((int)MapPointer->m_cameraX,(int)MapPointer->m_cameraY,x,y))
			VRenderImage(x,y,TILE_SIZE,TILE_SIZE,TYPE_ENEMY,MapPointer->m_closeEnemyXY[i].imgID);
		/* You can not use ->m_imageSurface[i] for image that needs to be drawn, because there can be more than one*/
	}

	for (size_t i = 0; i < MapPointer->m_npcXY.size(); i++)
	{
		x = (int)MapPointer->m_npcXY[i].x;
		y = (int)MapPointer->m_npcXY[i].y;
		if (CheckIfInWindowEx((int)MapPointer->m_cameraX,(int)MapPointer->m_cameraY,x,y))
			VRenderImage(x,y,TILE_SIZE,TILE_SIZE,TYPE_NPC,MapPointer->m_npcXY[i].imgID);
	}

	for (size_t i = 0; i < MapPointer->m_spell.size(); i++)
	{
		x = (int)MapPointer->m_spell[i].x;
		y = (int)MapPointer->m_spell[i].y;
		if (CheckIfInWindowEx((int)MapPointer->m_cameraX,(int)MapPointer->m_cameraY,x,y))
			VRenderImage(x,y,TILE_SIZE,TILE_SIZE,TYPE_SPELL,MapPointer->m_spell[i].imgID);
	}
}

void	CRender::RenderHUD(int fps,int hp,int mp)
{
	VRenderFPS(fps);
	VRenderText("HP:",20,WINDOW_HEIGHT+10,98,0,49);
	VRenderValue(hp,60,WINDOW_HEIGHT+10,98,0,49);
	VRenderText("MP:",20,WINDOW_HEIGHT+30,98,0,49);
	VRenderValue(mp,60,WINDOW_HEIGHT+30,98,0,49);
}

void	CRender::SetTextBox(std::string text)
{
	m_text = text;
}

void	CRender::RenderMainMenu()
{
	VRenderButton("New game",WINDOW_WIDTH/2-96,WINDOW_HEIGHT-144-64*3,25,23,112);
	VRenderButton("Map editor",WINDOW_WIDTH/2-96,WINDOW_HEIGHT-144-64*2,25,25,112);
	VRenderButton("Options",WINDOW_WIDTH/2-96,WINDOW_HEIGHT-144-64,25,25,112);
	VRenderButton("Exit",WINDOW_WIDTH/2-96,WINDOW_HEIGHT-144,25,23,112);
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

bool	CRender::CheckIfInWindowEx(int cameraX,int cameraY,int &x,int &y)
{
	// get window locations
	x = x - cameraX + WINDOW_WIDTH/2;
	y = y - cameraY + WINDOW_HEIGHT/2;

	// do not allow the entity to be drawn outside of map
	if (cameraX <= WINDOW_WIDTH/2)
	{
		if (x < (cameraX - WINDOW_WIDTH/2))
			return false;
	}

	if (cameraY <= WINDOW_HEIGHT/2)
	{
		if (y < (cameraY - WINDOW_HEIGHT/2))
			return	false;
	}

	if (cameraX >= (MAP_WIDTH - WINDOW_WIDTH/2))
	{
		if (x > (cameraX + WINDOW_WIDTH/2))
			return	false;
	}

	if (cameraY >= (MAP_HEIGHT - WINDOW_HEIGHT/2))
	{
		if (y > (cameraY + WINDOW_HEIGHT/2))
			return	false;
	}

	// entity in window, let it be drawn

	int tempX = x - cameraX - WINDOW_WIDTH/2;
	int tempY = y - cameraY - WINDOW_HEIGHT/2;

	return	true;
}
