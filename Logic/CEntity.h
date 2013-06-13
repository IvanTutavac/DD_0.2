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

class CTimer;

class CEntity
{

private:

	int	m_hp;
	int	m_mp;
	char m_name[20];

protected:

	//std::vector<CTimer*> m_VSpellCDTimer; // each spell has a timer that is used to check spell cooldown
	bool	InitEntity();
	void	CleanEntity();

public:

	// Logic should contain all spells and after they are loaded there, they can be sent to entities that know them
	std::vector<_spell> m_VSpell; // all available spells for this entity, so spell ID from logic can not be used as an index to access a spell from this vector, search the spell by ID
	// enemy will probably not use this vector, spells should be directly sent to m_spell
	
	std::vector<_spell>	m_spell; // spells prepared for casting, event for the spell can be used as an index 

	void	SetName(char *value);
	char*	GetName();

	int		GetHP();
	void	SetHP(int value);
	void	ReduceHP(int value);

	int		GetMP();
	void	SetMP(int value);
	void	ReduceMP(int value);

	void	CastSpell(int spell);
	void	SpellHit(int spell); // probaj saznati koji spell od koordinata, tj. kad gledas koliziju dal je spell pogodio, tada znas i koji spell je to bio

	// returns true if spell can be used and will also reset duration and set casted to true
	bool	CheckSpellCD(int ID);

	// update .cd and .duration
	void	UpdateSpellDuration(double deltaTime);

};