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

#include "CEntity.h"
#include "..\CTimer.h"
#include "..\debug.h"
#include "..\const.h"

bool	CEntity::InitEntity()
{
	return	true;
}

void	CEntity::CleanEntity()
{

}

void	CEntity::SetName(char *value)
{
	int	size = std::char_traits<char>::length(value);
	std::char_traits<char>::copy(m_name,value,size);
	m_name[size]='\0';
}

char*	CEntity::GetName()
{
	return	m_name;
}

int		CEntity::GetHP()
{
	return	m_hp;
}

void	CEntity::SetHP(int value)
{
	m_hp = value;
}

void	CEntity::ReduceHP(int value)
{
	m_hp -= value;
}

int		CEntity::GetMP()
{
	return	m_mp;
}

void	CEntity::SetMP(int value)
{
	m_mp = value;
}

void	CEntity::ReduceMP(int value)
{
	m_mp -= value;
	if (m_mp < 0)
		m_mp = 0;
}

void	CEntity::CastSpell(int spell)
{
	ReduceMP(m_spell[spell].cost);
}

void	CEntity::SpellHit(int spell)
{

}

bool	CEntity::CheckSpellCD(int ID)
{
	for (int i = 0; i < m_VSpell.size(); i++)
	{
		if (m_VSpell[i].ID == ID)
		{
			if (m_VSpell[i].cd <= 0)
			{
				m_VSpell[i].cd = m_VSpell[i].tempCD;

				return	true;
			}
			else if (!m_VSpell[i].casted)
			{
				m_VSpell[i].casted = true;
				
				return	true;
			}
		}
	}
	return	false;
}

void	CEntity::UpdateSpellDuration(double deltaTime)
{
	for (int i = 0; i < m_VSpell.size(); i++)
	{
		if (m_VSpell[i].casted)
		{
			m_VSpell[i].duration -= deltaTime;
			m_VSpell[i].cd	-= deltaTime;
		}
	}
}

