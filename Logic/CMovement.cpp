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
#include "CMovement.h"
#include "..\const.h"

void	CMovement::SpellMovement(CMap *MapPointer,float x,float y,double deltaTime)
{
	if (MapPointer->m_spell.empty())
		return;

	float	d = 0;

	for (int i = MapPointer->m_spell.size()-1; i >= 0; i--)
	{
		// spell casted, calculate where it will be moved
		if (MapPointer->m_spell[i].state == LS_nothing)
		{
			x-= MapPointer->m_playerX;
			y-= MapPointer->m_playerY;

			d = sqrt(x*x+y*y);

			x /= d;
			y /= d;

			MapPointer->m_spell[i].tempX = x;
			MapPointer->m_spell[i].tempY = y;
			MapPointer->m_spell[i].state = LS_moving;
		}

		MapPointer->m_spell[i].x += (float)(MapPointer->m_spell[i].tempX * deltaTime * MapPointer->m_spell[i].speed);
		MapPointer->m_spell[i].y += (float)(MapPointer->m_spell[i].tempY * deltaTime * MapPointer->m_spell[i].speed);

		if (MapPointer->m_spell[i].x < 0 || MapPointer->m_spell[i].x > MAP_WIDTH-TILE_SIZE || MapPointer->m_spell[i].y < 0 || MapPointer->m_spell[i].y > MAP_HEIGHT-TILE_SIZE)
		{
			// spell outside of map, delete it
			MapPointer->m_spell.erase(MapPointer->m_spell.begin()+i);
		}
	}
}

void	CMovement::CameraMovement(int mouseX,int mouseY,float &cameraX,float &cameraY,double deltaTime)
{
	float v = 400;
	float speed = v*0.70710678118f;

	if (mouseX >= WINDOW_WIDTH-1 && cameraX < MAP_WIDTH - WINDOW_WIDTH/2)
	{
		if (mouseY <= 0 && cameraY > WINDOW_HEIGHT/2)
		{
			cameraY -= (float)(speed * deltaTime);
			cameraX += (float)(speed * deltaTime);
		}
		else if (mouseY >= WINDOW_HEIGHT-1+HUD_HEIGHT && cameraY < MAP_HEIGHT - WINDOW_HEIGHT/2)
		{
			cameraY += (float)(speed * deltaTime);
			cameraX += (float)(speed * deltaTime);
		}
		else
			cameraX += (float)(v * deltaTime);
	}
	else if (mouseX <= 0 && cameraX > WINDOW_WIDTH/2)
	{
		if (mouseY <= 0 && cameraY > WINDOW_HEIGHT/2)
		{
			cameraY -= (float)(speed * deltaTime);
			cameraX -= (float)(speed * deltaTime);
		}
		else if (mouseY >= WINDOW_HEIGHT-1+HUD_HEIGHT && cameraY < MAP_HEIGHT - WINDOW_HEIGHT/2)
		{
			cameraY += (float)(speed * deltaTime);
			cameraX -= (float)(speed * deltaTime);
		}
		else
			cameraX -= (float)(v * deltaTime);
	}
	else if (mouseY >= WINDOW_HEIGHT-1+HUD_HEIGHT && cameraY < MAP_HEIGHT - WINDOW_HEIGHT/2)
	{
		cameraY += (float)(v * deltaTime);
	}
	else if (mouseY <= 0 && cameraY > WINDOW_HEIGHT/2)
	{
		cameraY -= (float)(v * deltaTime);
	}

	if (cameraX > MAP_WIDTH - WINDOW_WIDTH/2)
		cameraX = MAP_WIDTH - WINDOW_WIDTH/2;	
	else if (cameraX < WINDOW_WIDTH/2)
		cameraX = WINDOW_WIDTH/2;

	if (cameraY < WINDOW_HEIGHT/2)
		cameraY = WINDOW_HEIGHT/2;
	else if (cameraY > MAP_HEIGHT - WINDOW_HEIGHT/2)
		cameraY = MAP_HEIGHT - WINDOW_HEIGHT/2;
}

void	CMovement::MoveEntity(int leftRight,int upDown,float &x,float &y,float speed,double deltaTime)
{
	if (leftRight == m_right)
	{
		if (upDown == m_up)
		{
			MoveUp(y,speed*0.70710678118f,deltaTime);
			MoveRight(x,speed*0.70710678118f,deltaTime);
		}
		else if (upDown == m_down)
		{
			MoveDown(y,speed*0.70710678118f,deltaTime);
			MoveRight(x,speed*0.70710678118f,deltaTime);
		}
		else
			MoveRight(x,speed,deltaTime);
	}
	else if (leftRight == m_left)
	{
		if (upDown == m_up)
		{
			MoveUp(y,speed*0.70710678118f,deltaTime);
			MoveLeft(x,speed*0.70710678118f,deltaTime);
		}
		else if (upDown == m_down)
		{
			MoveDown(y,speed*0.70710678118f,deltaTime);
			MoveLeft(x,speed*0.70710678118f,deltaTime);
		}
		else
			MoveLeft(x,speed,deltaTime);
	}
	else if (upDown == m_up)
		MoveUp(y,speed,deltaTime);
	else if (upDown == m_down)
		MoveDown(y,speed,deltaTime);
}

void	CMovement::MoveRight(float &x,float speed,double deltaTime)
{
	x += ((float)(speed*deltaTime));

	if (x > MAP_WIDTH - TILE_SIZE)
	{
		x = MAP_WIDTH - TILE_SIZE;
	}
}

void	CMovement::MoveLeft(float &x,float speed,double deltaTime)
{
	x -= ((float)(speed*deltaTime));

	if (x < 0)
	{
		x = 0;
	}
}

void	CMovement::MoveUp(float &y,float speed,double deltaTime)
{
	y -= ((float)(speed*deltaTime));

	if (y < 0)
	{
		y = 0;
	}
}

void	CMovement::MoveDown(float &y,float speed,double deltaTime)
{
	y += ((float)(speed*deltaTime));

	if (y > MAP_HEIGHT - TILE_SIZE)
	{
		y = MAP_HEIGHT - TILE_SIZE-1;
	}
}
