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

#include "CSpell.h"
#include "Map\CMap.h"

bool	CSpell::Init()
{
	m_UniqueSpellID		=	0;
	m_IDLimit			=	999999;

	_spell	dummySpell;
	dummySpell.cost = 10;
	dummySpell.type = ST_hpRemoval;
	dummySpell.duration = 3;
	dummySpell.value = 25;
	dummySpell.moving = true;
	dummySpell.casted = false;
	dummySpell.ID = 0;
	dummySpell.cd = 1;
	dummySpell.tempCD = dummySpell.cd;
	dummySpell.speed = 300;
	strcpy_s(dummySpell.name,"Fire");

	m_VAllSpells.push_back(dummySpell);

	dummySpell.cost	=	0;
	dummySpell.type = ST_hpRemoval;
	dummySpell.duration = 2;
	dummySpell.value = 25;
	dummySpell.moving = true;
	dummySpell.casted = false;
	dummySpell.ID = 1;
	dummySpell.cd = 1;
	dummySpell.tempCD = dummySpell.cd;
	dummySpell.speed = 450;
	strcpy_s(dummySpell.name,"Bolt");

	m_VAllSpells.push_back(dummySpell);

	return	true;
}

void	CSpell::Clean()
{

}

void	CSpell::CastSpell(int index,int &hp,int &mp)
{
	if (m_VAllSpells[index].type == ST_hpHeal)
	{
		hp += m_VAllSpells[index].value;
	}

	mp -= m_VAllSpells[index].cost;
}

void	CSpell::SpellHit(int index,int &hp,int &mp)
{
	if (m_VAllSpells[index].type == ST_hpRemoval)
	{
		hp -= m_VAllSpells[index].value;
	}
}

void	CSpell::SetupSpellMap(CMap *MapPointer,int id,float x,float y,double duration,int speed,int parent)
{
	_location1		newSpell;
	newSpell.x			=	x;
	newSpell.y			=	y;
	newSpell.imgID		=	id;
	newSpell.parent		=	parent;
	newSpell.speed		=	speed;
	newSpell.state		=	LS_nothing;
	newSpell.duration	= duration;

	MapPointer->m_spell.push_back(newSpell); 

	MapPointer->m_spell.back().ID = m_UniqueSpellID;
	m_UniqueSpellID++;

	if (++m_UniqueSpellID > m_IDLimit)
		m_UniqueSpellID = 0;
}