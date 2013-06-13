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

#pragma	once

class	CQuest
{
private:

	char	m_name[20];
	int		m_typeID;
	int		m_requiredQuestID;
	int		m_requiredID; // monster ID
	int		m_numRequired;
	int		m_goldReward;
	int		m_itemReward; // itemID
	int		m_numItemReward;

	bool	m_completed;
	bool	m_active;

public:

	CQuest();

	bool	Init();
	void	Clean();

	void	SetName(char *value);
	char*   GetName();

	void	SetTypeID(int value);
	int		GetTypeID();

	void	SetRequiredQuestID(int value);
	int		GetRequiredQuestID();

	void	SetRequiredID(int value);
	int		GetRequiredID();

	void	SetNumRequired(int value);
	int		GetNumRequired();

	void	SetGoldReward(int value);
	int		GetGoldReward();

	void	SetItemReward(int value);
	int		GetItemReward();

	void	SetNumItemReward(int value);
	int		GetNumItemReward();

	void	SetCompleted(bool value);
	bool	IsCompleted();

	void	SetActive(bool value);
	bool	IsActive();

};