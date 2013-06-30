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

class CEnemy;
class CPlayer;
class CNPC;
class CSpell;
class CMap;

class CEntityManager
{

private:

	int				m_UniqueEnemyID;

	// loads all enemies which can be used 
	// so for each enemy instance there's one new CEnemy in m_VEnemyList
	bool	LoadEnemies();

public:

	std::vector<CEnemy> m_VEnemyList; // enemies which can be available 
	std::vector<CEnemy> m_VEnemy; // enemies on map
	std::vector<CEnemy> m_VCloseEnemy; // enemies near you

	std::vector<CNPC> m_VNpc;

	CPlayer			*m_pPlayer;

	bool	Init(CSpell *SpellPointer);
	void	Clean();

	// put enemies on map
	bool	SetMapEnemy(CMap *MapPointer);
};