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
#include "..\dataStructs.h"

class CConversation;

class CNPC
{
private:

public:

	std::vector<CConversation*>	m_VConversation;

	std::string m_conversation; // one text, after there's no text available we use this one

	std::string	m_availableConversations;
	int			m_NumConversations; 

	bool	Init();
	bool	Init(std::string text);
	bool	Init(std::vector<std::string> VText,int questID,int questRequiredID,TextState state,bool hidden);
	void	Clean();
	
	// clears previous data and sets m_availableConversations from each m_VConversation[i]->m_vText[0] that is not hidden
	void	AvailableConversations(); 

	// will update m_hidden 
	void	UpdateConversationVisibility(int index);

	// updates state 
	void	UpdateConversation(int index);
	
	bool	ConversationFinished(int index);
	bool	ConversationSelection(int index);
	bool	ConversationWait(int index);
	void	SetConversationStateYes(int index);
	void	SetConversationStateNo(int index);

	// returns -1 if it's not a quest, if it is a quest, it will return the questID
	int		GetQuestID(int index/*conversationIndex*/);
	void	SetQuestID(int index/*ConversationIndex*/,int value);

	int		GetConversationIndex(int i);

	// value becomes textIndex
	void	GetTextIndex(int &value,int index/*conversationIndex*/);
};