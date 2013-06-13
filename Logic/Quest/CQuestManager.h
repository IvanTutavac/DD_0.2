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

#include <vector>

class CQuest;

class CQuestManager
{
private:

	std::vector<CQuest*> m_VQuests; // contains all quests

	int		m_index; // selected quest;

	bool	LoadData();

public:

	bool	Init();
	void	Clean();

	// call it after an enemy has died, it will check if the enemy killed was required for a quest and reduce the numRequired
	void	UpdateQuest(int ID/*enemyID*/);

	// search all quests and set m_index 
	void	FindIndex(int ID);
	int		GetIndex(int ID);
	int		GetIndex();

	void	SetActiveQuest(bool value);
	void	SetActiveQuest(int ID,bool value);
	bool	IsActive(int ID);
	bool	IsActive();

	// if numRequired < 1 set it to completed and return true
	bool	CheckQuestCompleted(int ID);
	bool	CheckQuestCompleted();
	void	SetCompletedQuest(bool value);
	void	SetCompletedQuest(int ID,bool value);
	bool	IsCompleted(int ID);
	bool	IsCompleted();


};