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

	dummySpell.spell[0].duration		=	3;
	dummySpell.spell[0].value			=	25;
	dummySpell.spell[0].value1			=	0;
	dummySpell.spell[0].value2			=	0;
	dummySpell.spell[0].cd				=	1;
	dummySpell.spell[0].tempCD			=	dummySpell.spell[0].cd;
	dummySpell.spell[0].speed			=	300;
	dummySpell.spell[0].n				=	1;
	dummySpell.spell[0].range			=	0;
	dummySpell.spell[0].cost			=	10;
	dummySpell.spell[0].skillPoints		=	1;

	dummySpell.spell[1].duration		=	3;
	dummySpell.spell[1].value			=	30;
	dummySpell.spell[1].value1			=	0;
	dummySpell.spell[1].value2			=	0;
	dummySpell.spell[1].cd				=	1;
	dummySpell.spell[1].tempCD			=	dummySpell.spell[0].cd;
	dummySpell.spell[1].speed			=	320;
	dummySpell.spell[1].n				=	1;
	dummySpell.spell[1].range			=	0;
	dummySpell.spell[1].cost			=	10;
	dummySpell.spell[1].skillPoints		=	1;

	dummySpell.type = ST_hpRemoval;
	dummySpell.moving = true;
	dummySpell.casted = false;
	dummySpell.ID = 0;
	strcpy_s(dummySpell.name,"Fire");

	m_VAllSpells.push_back(dummySpell);

	dummySpell.spell[0].cost			=	0;
	dummySpell.spell[0].duration		=	2;
	dummySpell.spell[0].value			=	25;
	dummySpell.spell[0].value1			=	0;
	dummySpell.spell[0].value2			=	0;
	dummySpell.spell[0].cd				=	1;
	dummySpell.spell[0].tempCD			=	dummySpell.spell[0].cd;
	dummySpell.spell[0].speed			=	450;
	dummySpell.spell[0].n				=	1;
	dummySpell.spell[0].range			=	0;
	dummySpell.spell[0].skillPoints		=	1;

	dummySpell.spell[1].cost			=	0;
	dummySpell.spell[1].duration		=	2.1;
	dummySpell.spell[1].value			=	30;
	dummySpell.spell[1].value1			=	0;
	dummySpell.spell[1].value2			=	0;
	dummySpell.spell[1].cd				=	1;
	dummySpell.spell[1].tempCD			=	dummySpell.spell[0].cd;
	dummySpell.spell[1].speed			=	470;
	dummySpell.spell[1].n				=	1;
	dummySpell.spell[1].range			=	0;
	dummySpell.spell[1].skillPoints		=	1;

	dummySpell.type = ST_hpRemoval;
	dummySpell.moving = true;
	dummySpell.casted = false;
	dummySpell.ID = 1;
	strcpy_s(dummySpell.name,"Bolt");

	m_VAllSpells.push_back(dummySpell);

	return	true;
}

void	CSpell::Clean()
{

}

void	CSpell::CastSpell(int index,int lvl,int &hp,int &mp)
{
	if (m_VAllSpells[index].type == ST_hpHeal)
	{
		hp += m_VAllSpells[index].spell[lvl].value;
	}

	mp -= m_VAllSpells[index].spell[lvl].cost;
}

void	CSpell::SpellHit(int index,int lvl,int &hp,int &mp)
{
	if (m_VAllSpells[index].type == ST_hpRemoval)
	{
		hp -= m_VAllSpells[index].spell[lvl].value;
	}
}

void	CSpell::SetupSpellMap(CMap *MapPointer,int id,float x,float y,double duration,int speed,int lvl,int parent)
{
	_location1		newSpell;
	newSpell.x			=	x;
	newSpell.y			=	y;
	newSpell.imgID		=	id;
	newSpell.lvl		=	lvl;
	newSpell.parent		=	parent;
	newSpell.speed		=	speed;
	newSpell.state		=	LS_nothing;
	newSpell.duration	=	duration;

	MapPointer->m_spell.push_back(newSpell); 

	MapPointer->m_spell.back().ID = m_UniqueSpellID;
	m_UniqueSpellID++;

	if (++m_UniqueSpellID > m_IDLimit)
		m_UniqueSpellID = 0;
}