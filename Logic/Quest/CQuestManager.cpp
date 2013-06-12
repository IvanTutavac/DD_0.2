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

#include "CQuestManager.h"
#include "CQuest.h"
#include "..\..\Log.h"
#include "..\..\debug.h"

bool	CQuestManager::Init()
{
	CleanLogFile("QuestLog");

	if (!LoadData())
	{
		Log("QuestLog","LoadData() failed");
		return	false;
	}

	Log("QuestLog","Data loaded");


	Log("QuestLog","Quests loaded");
	return	true;
}

void	CQuestManager::Clean()
{
	for (int i = 0; i < m_VQuests.size(); i++)
	{
		m_VQuests[i]->Clean();
		DD_DELETE(m_VQuests[i]);
	}
	Log("QuestLog","Quests deleted");
}

bool	CQuestManager::LoadData()
{

	return	true;
}

void	CQuestManager::FindIndex()
{

}

int		CQuestManager::GetIndex()
{
	return	m_index;
}