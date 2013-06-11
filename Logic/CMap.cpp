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

#include "CMap.h"
//#include "..\CTimer.h"
#include <fstream>

bool	CMap::Init()
{
	m_playerX		=	32;
	m_playerY		=	32;
	m_playerSpeed	=	100;
	m_cameraX		=	320;
	m_cameraY		=	240;

	m_allTilesMapState	=	false;
	m_selectedTile	=	2;

	_location	temp;
	temp.x = 96,temp.y = 96;
	temp.ID = 0;
	temp.imgID = 0;

	m_npcXY.push_back(temp);

	_location temp1;
	temp1.x = 14*32,temp1.y = 7*32;
	temp1.ID = 1;
	temp1.imgID = 0;

	m_npcXY.push_back(temp1);

	if (!LoadMap("data/Map1.dat"))
		return	false;

	return	true;
}

void	CMap::Clean()
{
	
}

bool	CMap::LoadMap(char *fileName)
{
	std::fstream	dat;
	dat.open(fileName,std::ios::in | std::ios::binary);
	if (!dat.is_open())
	{
		dat.clear();
		dat.close();
		return	false;
	}
	dat.read((char*)&m_map,sizeof(m_map));
	dat.close();
	dat.clear();
	return	true;
}

bool	CMap::LoadMapEditorMap(char *fileName)
{
	std::fstream	dat;
	dat.open(fileName,std::ios::in |std::ios::binary);
	if (!dat.is_open())
	{
		dat.clear();
		dat.close();
		return	false;
	}
	dat.read((char*)&m_mapEditor,sizeof(m_map));
	dat.close();
	dat.clear();
	return	true;
}

void	CMap::SaveMapEditorMap()
{
	std::fstream	dat;
	dat.open("data/tempMap.dat",std::ios::out | std::ios::binary);
	dat.write((char*)&m_mapEditor,sizeof(m_mapEditor));
	dat.clear();
	dat.close();
}


void	CMap::ResetMapEditorMap()
{
	for (int i = 0; i < WINDOW_HEIGHT / 32; i++)
	{
		for (int j = 0; j < WINDOW_WIDTH / 32; j++)
		{
			m_mapEditor[i][j] = 0;
		}
	}
}

void	CMap::InitAllTilesMap()
{
	int	k	=	1;
	int l	=	1;
	int	n	=	0;

	for (int i = 0; i < MAP_HEIGHT/32; i++)
	{
		for (int j = 0; j < MAP_WIDTH/32; j++)
		{
			m_allTilesMap[i][j] = TILE_NUMBER_FINISH+10;
		}
	}
	
	bool	again = true;

	do
	{
		for (int i = 0; i < k * WINDOW_HEIGHT/32 && again; i++)
		{
			for (int j = 0; j < l * WINDOW_WIDTH/32 && again; j++)
			{
				m_allTilesMap[i][j] = n;
				n++;
				if (n > TILE_NUMBER_FINISH)
				{
					again = false;
				}
			}
		}

		if (l < 3)
			l++;
		else
			k++;

	}while(n < ((k * WINDOW_WIDTH/32) * (l * WINDOW_HEIGHT/32)) && again);

	for (int i = 0; i < MAP_HEIGHT / 32; i++)
	{
		for (int j = 0; j < MAP_WIDTH / 32; j++)
		{
			if (m_allTilesMap[i][j] > TILE_NUMBER_FINISH)
			{
				m_allTilesMap[i][j] = 2;
			}
		}
	}

	m_allTilesMapState	=	true;
}
