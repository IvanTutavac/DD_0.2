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

#include "CCollision.h"
#include "..\Entity\CEntityManager.h"
#include "..\Entity\CEnemy.h"
#include "..\Entity\CPlayer.h"
#include "..\CSpell.h"
#include "CMap.h"

bool	CCollision::Init()
{
	return	true;
}

void	CCollision::Clean()
{

}

void	CCollision::SpellCollision(CMap *MapPointer,CEntityManager *EntityPointer,CSpell *SpellPointer)
{
	if (MapPointer->m_spell.empty())
		return;

	for (int i = MapPointer->m_spell.size()-1; i >= 0; i--)
	{
		// check if player was hit by a spell
		if (CheckCollision((int)MapPointer->m_playerX,(int)MapPointer->m_playerY,(int)MapPointer->m_spell[i].x,(int)MapPointer->m_spell[i].y,TILE_SIZE))
		{
			// check to see if the spell was not created by player
			if (MapPointer->m_spell[i].parent != -1)
			{
				// spell casted by enemy
				int hp = EntityPointer->m_pPlayer->GetHP();
				int mp = EntityPointer->m_pPlayer->GetMP();

				SpellPointer->SpellHit(MapPointer->m_spell[i].imgID,hp,mp);
				
				EntityPointer->m_pPlayer->SetHP(hp);
				EntityPointer->m_pPlayer->SetMP(mp);
				
				MapPointer->m_spell.erase(MapPointer->m_spell.begin()+i);

				continue;
			}
		}

		//checking if an enemy has been hit by a spell

		for (size_t j = 0; j < MapPointer->m_closeEnemyXY.size(); j++)
		{
			if (CheckCollision((int)MapPointer->m_closeEnemyXY[j].x,(int)MapPointer->m_closeEnemyXY[j].y,(int)MapPointer->m_spell[i].x,(int)MapPointer->m_spell[i].y,TILE_SIZE))
			{
				int hp = EntityPointer->m_VCloseEnemy[j].GetHP();
				int mp = EntityPointer->m_VCloseEnemy[j].GetMP();

				SpellPointer->SpellHit(MapPointer->m_spell[i].imgID,hp,mp);

				EntityPointer->m_VCloseEnemy[j].SetHP(hp);
				EntityPointer->m_VCloseEnemy[j].SetMP(mp);
				
				// enemies created on map must be created in the same order in EntityManager so that the same index can be used to access both
				MapPointer->m_spell.erase(MapPointer->m_spell.begin()+i);
				
				break;
			}
		}
	}
}

void	CCollision::PlayerEnemyCollision(CMap *MapPointer)
{
	for (size_t i = 0; i < MapPointer->m_closeEnemyXY.size(); i++)
	{
		if (CheckCollision((int)MapPointer->m_playerX,(int)MapPointer->m_playerY,(int)MapPointer->m_closeEnemyXY[i].x,(int)MapPointer->m_closeEnemyXY[i].y,TILE_SIZE))
		{
			MapPointer->m_playerX = MapPointer->m_tempPlayerX;
			MapPointer->m_playerY = MapPointer->m_tempPlayerY;
		}
	}
}

void	CCollision::PlayerNPCCollision(CMap *MapPointer)
{
	for (size_t i = 0; i < MapPointer->m_npcXY.size(); i++)
	{
		if (CheckCollision((int)MapPointer->m_playerX,(int)MapPointer->m_playerY,(int)MapPointer->m_npcXY[i].x,(int)MapPointer->m_npcXY[i].y,TILE_SIZE))
		{
			MapPointer->m_playerX = MapPointer->m_tempPlayerX;
			MapPointer->m_playerY = MapPointer->m_tempPlayerY;
		}
	}
}

bool	CCollision::CheckCollision(int x1,int y1,int x2,int y2,int size)
{
	if (x1+size > x2 && x1 < x2+size && y1+size > y2 && y1 < y2+size)
		return	true;

	return	false;
}
