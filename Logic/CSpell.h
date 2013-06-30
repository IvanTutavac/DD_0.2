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

#include <vector>
#include "dataStructs.h"

class CMap;

class CSpell
{

private:

	int m_UniqueSpellID; // used to set unique _spell.ID
	int	m_IDLimit;

public:

	std::vector<_spell> m_VAllSpells; // contains all spells

	bool	Init();
	void	Clean();

	// create spell data needed for movement ( render and timer data )
	//_spell[index].ID
	// entityX and entityY
	// _spell[index].duration
	//_spell[index].speed
	// parent  EntityID, -1 for player, everything else is EnemyID
	void	SetupSpellMap(CMap *MapPointer,int id,float x,float y,double duration,int speed,int parent);

	void	CastSpell(int index,int &hp,int &mp);
	void	SpellHit(int index,int &hp,int &mp);
};