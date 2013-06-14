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

#pragma pack(1)
struct _quest
{
	char	m_name[20];
	int		m_typeID;
	int		m_requiredQuestID;
	int		m_requiredID;
	int		m_numRequired;
	int		m_goldReward;
	int		m_itemReward; // itemID
	int		m_numItemReward;

	bool	m_completed;
	bool	m_active;
};
#pragma pack()

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
	for (size_t i = 0; i < m_VQuests.size(); i++)
	{
		m_VQuests[i]->Clean();
		DD_DELETE(m_VQuests[i]);
	}
	Log("QuestLog","Quests deleted");
}

bool	CQuestManager::LoadData()
{
	_quest	quest;

	std::fstream	dat;
	
	dat.open("data/quest.dat",std::ios::in | std::ios::binary);

	if (!dat.is_open())
	{
		dat.clear();
		dat.close();
		Log("QuestLog","File not found");
		return	false;
	}

	while (true)
	{
		dat.read((char*)&quest,sizeof(quest));

		if (dat.eof())
			break;

		CQuest	*temp = DD_NEW CQuest;

		if (!temp->Init())
		{
			dat.clear();
			dat.close();
			Log("QuestLog","Quest->Init failed");
			return	false;
		}

		temp->SetActive(quest.m_active);
		temp->SetCompleted(quest.m_completed);
		temp->SetGoldReward(quest.m_goldReward);
		temp->SetItemReward(quest.m_itemReward);
		temp->SetName(quest.m_name);
		temp->SetNumItemReward(quest.m_numItemReward);
		temp->SetNumRequired(quest.m_numRequired);
		temp->SetRequiredID(quest.m_requiredID);
		temp->SetRequiredQuestID(quest.m_requiredQuestID);
		temp->SetTypeID(quest.m_typeID);

		m_VQuests.push_back(temp);
	}

	dat.clear();
	dat.close();

	return	true;
}

void	CQuestManager::UpdateQuest(int ID)
{
	for (size_t i = 0; i < m_VQuests.size(); i++)
	{
		if (m_VQuests[i]->GetRequiredID() == ID) // enemy found
		{
			int num = m_VQuests[i]->GetNumRequired();

			if (num < 1)
			{
				num = 0;
			}
			else
			{
				m_VQuests[i]->SetNumRequired(num-1);
			}
		}
	}
}

void	CQuestManager::FindIndex(int ID)
{
	m_index = GetIndex(ID);
}

int		CQuestManager::GetIndex()
{
	return	m_index;
}

void	CQuestManager::SetActiveQuest(bool value)
{
	m_VQuests[m_index]->SetActive(value);
}

void	CQuestManager::SetActiveQuest(int ID,bool value)
{
	m_VQuests[GetIndex(ID)]->SetActive(value);
}

bool	CQuestManager::IsActive()
{
	return	m_VQuests[m_index]->IsActive();
}

bool	CQuestManager::IsActive(int ID)
{
	return	m_VQuests[GetIndex(ID)]->IsActive();
}

int		CQuestManager::GetIndex(int ID)
{
	for (size_t i = 0; i < m_VQuests.size(); i++)
	{
		if (m_VQuests[i]->GetTypeID() == ID)
			return	i;
	}
	return	-1;
}

void	CQuestManager::SetCompletedQuest(bool value)
{
	m_VQuests[m_index]->SetCompleted(value);
}

void	CQuestManager::SetCompletedQuest(int ID,bool value)
{
	m_VQuests[GetIndex(ID)]->SetCompleted(value);
}

bool	CQuestManager::IsCompleted(int ID)
{
	return	m_VQuests[GetIndex(ID)]->IsCompleted();
}

bool	CQuestManager::IsCompleted()
{
	return	m_VQuests[m_index]->IsCompleted();
}

bool	CQuestManager::CheckQuestCompleted(int ID)
{
	int index = GetIndex(ID);

	if (m_VQuests[index]->GetNumRequired() < 1)
	{
		m_VQuests[index]->SetCompleted(true);

		return	true;
	}
	return	false;
}

bool	CQuestManager::CheckQuestCompleted()
{
	if (m_VQuests[m_index]->GetNumRequired() < 1)
	{
		m_VQuests[m_index]->SetCompleted(true);

		return	true;
	}
	return	false;
}