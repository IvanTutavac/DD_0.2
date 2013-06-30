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
#include "../../const.h"
#include <vector>

enum LocationState
{
	LS_nothing,
	LS_moving
};

struct _location
{
	float x,y;
	float tempX,tempY;
	int ID;
	int imgID; // which img represents the entity
	int speed; 
};

struct _location1
{
	float x,y;
	float tempX,tempY;
	int ID;
	int imgID; // which img represents the entity
	int	parent; // who has used the spell, -1 for player, everything else enemy
	int speed; 
	LocationState state;  // use LS_
	double duration; // for how long will the object last
};

class CMap
{

private:

public:

	short	m_map[MAP_HEIGHT/32][MAP_WIDTH/32];
	short   m_mapEditor[MAP_HEIGHT/32][MAP_WIDTH/32];
	short	m_allTilesMap[MAP_HEIGHT/32][MAP_WIDTH/32];

	float	m_playerX; // mapX -> if we need windowX - WINDIW_WIDTH/2
	float	m_playerY; // mapY -> if we need windowY - WINDOW_HEIGHT/2
	float	m_tempPlayerX; // so that we can revert the movement ( collision ... )
	float   m_tempPlayerY; 
	float   m_playerSpeed; 
	float	m_cameraX;
	float	m_cameraY;

	bool	m_allTilesMapState;
	int		m_selectedTile;

	std::vector<_location>	m_enemyXY;
	std::vector<_location>	m_closeEnemyXY;
	std::vector<_location>	m_npcXY;
	std::vector<_location1>  m_spell;

	bool	Init();
	void	Clean();  

	void	ResetMapEditorMap();
	void	InitAllTilesMap();

	bool	LoadMap(char *fileName);
	bool	LoadMapEditorMap(char *fileName);
	void	SaveMapEditorMap();
};