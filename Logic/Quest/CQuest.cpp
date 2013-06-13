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

#include "CQuest.h"
#include <string>

CQuest::CQuest()
{
	m_typeID		=	0;
	m_requiredID	=	0;
	m_numRequired	=	0;
	m_goldReward	=	0;
	m_itemReward	=	0;
	m_numItemReward	=	0;

	m_completed		=	false;
	m_active		=	false;
}

bool	CQuest::Init()
{
	return	true;
}

void	CQuest::Clean()
{

}

void	CQuest::SetName(char *value)
{
	int	size = std::char_traits<char>::length(value);
	std::char_traits<char>::copy(m_name,value,size);
	m_name[size]='\0';
}

char*	CQuest::GetName()
{
	return	m_name;
}

void	CQuest::SetTypeID(int value)
{
	m_typeID = value;
}

int		CQuest::GetTypeID()
{
	return	m_typeID;
}

void	CQuest::SetRequiredQuestID(int value)
{
	m_requiredQuestID = value;
}

int		CQuest::GetRequiredQuestID()
{
	return	m_requiredQuestID;
}

void	CQuest::SetRequiredID(int value)
{
	m_requiredID = value;
}

int		CQuest::GetRequiredID()
{
	return	m_requiredID;
}

void	CQuest::SetNumRequired(int value)
{
	m_numRequired	=	value;
}

int		CQuest::GetNumRequired()
{
	return	m_numRequired;
}

void	CQuest::SetGoldReward(int value)
{
	m_goldReward	=	value;
}

int		CQuest::GetGoldReward()
{
	return	m_goldReward;
}

void	CQuest::SetItemReward(int value)
{
	m_itemReward = value;
}

int		CQuest::GetItemReward()
{
	return	m_itemReward;
}

void	CQuest::SetNumItemReward(int value)
{
	m_numItemReward	=	value;
}

int		CQuest::GetNumItemReward()
{
	return	m_numItemReward;
}

void	CQuest::SetCompleted(bool value)
{
	m_completed = value;
}

bool	CQuest::IsCompleted()
{
	return	m_completed;
}

void	CQuest::SetActive(bool value)
{
	m_active = value;
}

bool	CQuest::IsActive()
{
	return	m_active;
}