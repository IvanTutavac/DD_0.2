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

class CMap;
class CEntityManager;
class CSpell;

class CCollision
{

private:

	bool	CheckCollision(int x1,int y1,int x2,int y2,int size);

public:

	bool	Init();
	void	Clean();
	
	// checks for collision between player and all close enemies
	void	PlayerEnemyCollision(CMap *MapPointer);

	// checks for collision between player and all NPC
	void	PlayerNPCCollision(CMap *MapPointer);

	// it will check if player or enemy have been hit by a spell, do the spell action, and delete it from map
	void	SpellCollision(CMap *MapPointer,CEntityManager *EntityPointer,CSpell *SpellPointer);
};
