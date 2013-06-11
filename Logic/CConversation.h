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
#include <vector>
#include <string>
#include "dataStructs.h"

/*
	CNPC[NPCIndex] ->
		check m_hidden to see if we can show a conversation
		CConversation[ConversationIndex]->
			m_VText[TextIndex]
			 -> 0 text for conversation selection -> if selected we can continue with this conversation
			 -> 1 text is for lets say introduction
			 -> 2 text is for yes option
			 -> 3 text is for no option
			 -> we can hide the conversation or put finished option
*/

class	CConversation
{
private:

public:

	int							m_questID; // if != -1 this conversation is a quest
	int							m_questRequiredID;  // if == 1, no quest required
	std::vector<std::string>	m_VText;
	TextState					m_state; // use TS_
	
	bool	m_hidden;

	int		GetTextIndex();
	void	UpdateState();
};