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

#include "CPlayer.h"

bool	CPlayer::Init()
{
	if (!this->InitEntity())
		return	false;


	return	true;
}

void	CPlayer::Clean()
{
	this->CleanEntity();
}

int		CPlayer::GetSkillPoints()
{
	return	m_skillPoints;
}

void	CPlayer::SetSkillPoints(int value)
{
	m_skillPoints = value;
}

void	CPlayer::ReduceSkillPoints(int value)
{
	m_skillPoints -= value;
}