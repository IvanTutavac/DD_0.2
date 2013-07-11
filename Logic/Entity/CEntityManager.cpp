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

#include "CPlayer.h"
#include "CEnemy.h"
#include "CNPC.h"
#include "..\Map\CMap.h"
#include "CEntityManager.h"
#include "..\CSpell.h"
#include "..\..\Log.h"
#include "..\..\debug.h"

#pragma pack(1)
struct _enemy
{
	int	hp,mp;
	int typeID;
	char name[20];
};
#pragma pack()

bool	CEntityManager::Init(CSpell *Spell)
{
	Log("Entities...");

	m_UniqueEnemyID = 0;

	m_pPlayer = DD_NEW CPlayer;

	// make a limit for 5 spells !

	_EntitySpell temp;
	temp.duration	=	Spell->m_VAllSpells[0].spell[0].cd;
	temp.index		=	0;
	temp.lvl		=	0;
	temp.casted		=	false;
	temp.tempDuration = temp.duration;

	m_pPlayer->m_VSpell.push_back(temp);
	m_pPlayer->m_spell.push_back(temp);

	temp.duration	=	Spell->m_VAllSpells[1].spell[0].cd;
	temp.index		=	1;
	temp.lvl		=	0;
	temp.casted		=	false;
	temp.tempDuration = temp.duration;

	m_pPlayer->m_VSpell.push_back(temp);
	m_pPlayer->m_spell.push_back(temp);

	m_pPlayer->SetHP(50);
	m_pPlayer->SetMP(50);
	m_pPlayer->SetSkillPoints(3);

	if (!m_pPlayer->Init())
		return	false;
	Log("Player loaded");

	if (!LoadEnemies())
		return	false;
	Log("Enemies loaded");

	CNPC	NPC;//*NPC = DD_NEW CNPC;

	if (!NPC.Init())
		return	false;

	NPC.SetQuestID(0,0); // testing...

	m_VNpc.push_back(NPC);

	CNPC	NPC1;//*NPC1 = DD_NEW CNPC;

	if (!NPC1.Init("Just testing..."))
		return	true;

	m_VNpc.push_back(NPC1);

	Log("Npc loaded");

	return	true;
}

void	CEntityManager::Clean()
{
	m_pPlayer->Clean();
	DD_DELETE(m_pPlayer);

	for (size_t i = 0; i < m_VEnemyList.size(); i++)
	{
		m_VEnemyList[i].Clean();
	}

	for (size_t i = 0; i < m_VEnemy.size(); i++)
	{
		m_VEnemy[i].Clean();
	}

	for (size_t i = 0; i < m_VCloseEnemy.size(); i++)
	{
		m_VCloseEnemy[i].Clean();
	}

	for (size_t i = 0; i < m_VNpc.size(); i++)
	{
		m_VNpc[i].Clean();
	}
}

bool	CEntityManager::LoadEnemies()
{
	// enemy list... 
	// map should contain enemy locations, and logic should contain enemies copied from enemyList by typeID, they should get ID++;

	_enemy	enemy;
	std::fstream dat;

	dat.open("data/enemy.dat",std::ios::in | std::ios::binary);

	if (!dat.is_open())
	{
		dat.clear();
		dat.close();
		return	false;
	}
	while (true)
	{
		dat.read((char*)&enemy,sizeof(enemy));

		if (dat.eof())
			break;

		CEnemy	temp;

		if (!temp.Init())
		{
			dat.clear();
			dat.close();
			return	false;
		}

		temp.SetName(enemy.name);
		temp.SetHP(enemy.hp);
		temp.SetMP(enemy.mp);
		temp.SetTypeID(enemy.typeID);
		temp.SetID(-1);
		m_VEnemyList.push_back(temp);
	}

	dat.clear();
	dat.close();
	return	true;
}

bool	CEntityManager::SetMapEnemy(CMap *Map)
{
	// fali provjera dal je uniqueEnemyID > m_IDLimit
	// trebalo bi citati iz datoteka koji neprijatelj je na kojoj lokaciji mape
	// map editor bi to trebao raditi
	CEnemy Enemy;

	if (!Enemy.Init())
		return false;

	Enemy.SetHP(m_VEnemyList[0].GetHP());
	Enemy.SetMP(m_VEnemyList[0].GetMP());
	Enemy.SetName(m_VEnemyList[0].GetName());
	Enemy.SetTypeID(m_VEnemyList[0].GetTypeID());
	Enemy.SetID(m_UniqueEnemyID++);

	m_VCloseEnemy.push_back(Enemy);

	_location	temp;
	temp.ID = m_VCloseEnemy.back().GetID();
	temp.imgID = m_VCloseEnemy.back().GetTypeID();
	temp.x = 32*2,temp.y = 32*7;

	Map->m_closeEnemyXY.push_back(temp);

	CEnemy Enemy1;

	if (!Enemy1.Init())
		return	false;
	
	Enemy1.SetHP(m_VEnemyList[0].GetHP());
	Enemy1.SetMP(m_VEnemyList[0].GetMP());
	Enemy1.SetName(m_VEnemyList[0].GetName());
	Enemy1.SetTypeID(m_VEnemyList[0].GetTypeID());
	Enemy1.SetID(m_UniqueEnemyID++);

	m_VCloseEnemy.push_back(Enemy1);

	temp.ID = m_VCloseEnemy.back().GetID();
	temp.imgID = m_VCloseEnemy.back().GetTypeID();
	temp.x = 32*15,temp.y = 32*3;

	Map->m_closeEnemyXY.push_back(temp);


	return	true;
}
