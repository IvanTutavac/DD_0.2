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


#include "..\debug.h"
#include "CNPC.h"
#include "CConversation.h"


bool	CNPC::Init()
{
	CConversation	*Conversation	=	DD_NEW	CConversation;

	std::string text11	=	"First option (test)";
	std::string text12	=	"Introduction to options...";
	std::string text121 =	"Yes or no? nextLine yes nextLine no";
	std::string	text13	=	"Yes";
	std::string	text14	=	"Nothing, nothing...";
	std::string someText =	"prva druga treca cetvrta peta sesta sedma osma deveta deseta bla prva1 druga1 treca1 cetvrta1 peta1 sesta1 sedma1 osma1 deveta1 deseta1 bla ibla trla";

	Conversation->m_VText.push_back(text11);
	Conversation->m_VText.push_back(text12);
	Conversation->m_VText.push_back(text121);
	Conversation->m_VText.push_back(text13);
	Conversation->m_VText.push_back(text14);
	Conversation->m_VText.push_back(someText);
	Conversation->m_hidden			=	false;
	Conversation->m_state			=	TS_introduction;
	Conversation->m_questID			=	-1;
	Conversation->m_questRequiredID =	-1;

	m_VConversation.push_back(Conversation);

	CConversation	*Conversation1	=	DD_NEW	CConversation;

	std::string	text21	=	"Second option(test)";
	std::string	text22	=	"Introduction";
	std::string	text221 =	"Yes or no nextLine will do nextLine will not do";
	std::string	text23	=	"Yes";
	std::string	text24	=	"No";
	std::string	text25	=	"Accepted bla bla";

	Conversation1->m_VText.push_back(text21);
	Conversation1->m_VText.push_back(text22);
	Conversation1->m_VText.push_back(text221);
	Conversation1->m_VText.push_back(text23);
	Conversation1->m_VText.push_back(text24);
	Conversation1->m_VText.push_back(text25);
	Conversation1->m_hidden			=	false;
	Conversation1->m_state			=	TS_introduction;
	Conversation1->m_questID		=	-1;
	Conversation1->m_questRequiredID =	-1;

	m_VConversation.push_back(Conversation1);

	CConversation	*Conversation2	=	DD_NEW	CConversation;

	std::string	text31	=	"Third option(test)";
	std::string	text32	=	"Third selected";
	std::string	text321 =	"Decide nextLine Sure nextLine yea right";
	std::string	text33	=	"Yea";
	std::string	text34	=	"None";
	std::string	text35	=	"So, you have to...";

	Conversation2->m_VText.push_back(text31);
	Conversation2->m_VText.push_back(text32);
	Conversation2->m_VText.push_back(text321);
	Conversation2->m_VText.push_back(text33);
	Conversation2->m_VText.push_back(text34);
	Conversation2->m_VText.push_back(text35);
	Conversation2->m_hidden			=	false;
	Conversation2->m_state			=	TS_introduction;
	Conversation2->m_questID		=	-1;
	Conversation2->m_questRequiredID =	-1;

	m_VConversation.push_back(Conversation2);

	m_conversation = "Default razgovor";

	return	true;
}

bool	CNPC::Init(std::string text)
{
	m_conversation = text;

	return	true;
}

bool	CNPC::Init(std::vector<std::string> VText,int questID,int questRequiredID,TextState state,bool hidden = false)
{
	if (VText.size() < 1)
		return	false;

	CConversation	*Conversation = DD_NEW CConversation;

	for (int i = 0; i < VText.size(); i++)
	{
		Conversation->m_VText.push_back(VText[i]);
	}

	Conversation->m_hidden	=	hidden;
	Conversation->m_state	=	state;
	Conversation->m_questID =	questID;
	Conversation->m_questRequiredID	=	questRequiredID;

	m_VConversation.push_back(Conversation);

	return	true;
}

void	CNPC::Clean()
{
	for (int i = 0; i < m_VConversation.size(); i++)
	{
		DD_DELETE(m_VConversation[i]);
	}
	m_VConversation.clear();
}

void	CNPC::AvailableConversations()
{
	m_availableConversations.clear();
	m_NumConversations = 0;

	for (int i = 0; i < m_VConversation.size(); i++)
	{
		if (!m_VConversation[i]->m_hidden)
		{
			m_availableConversations += m_VConversation[i]->m_VText[0];
			m_availableConversations += " nextLine ";
			m_NumConversations++;
		}
	}
}

void	CNPC::UpdateConversation(int index)
{
	m_VConversation[index]->UpdateState();

	if (m_VConversation[index]->m_state == TS_finished)
	{
		UpdateConversationVisibility(index);
	}
}

int		CNPC::GetQuestID(int index)
{
	if (m_VConversation[index]->m_questID != -1)
	{
		return	m_VConversation[index]->m_questID;
	}

	return	-1;
}

void	CNPC::UpdateConversationVisibility(int index)
{
	for (int i = 0; i < m_VConversation.size(); i++)
	{
		if (m_VConversation[i]->m_hidden && m_VConversation[i]->m_questRequiredID != -1)
		{
			if (m_VConversation[i]->m_questRequiredID == m_VConversation[index]->m_questID)
			{
				m_VConversation[i]->m_hidden = false;
			}
		}
	}
	m_VConversation[index]->m_hidden = true;
}

bool	CNPC::ConversationFinished(int index)
{
	if (m_VConversation[index]->m_state == TS_finished)
		return	true;

	return	false;
}

bool	CNPC::ConversationSelection(int index)
{
	if (m_VConversation[index]->m_state == TS_option)
		return	true;

	return	false;
}

bool	CNPC::ConversationWait(int index)
{
	if (m_VConversation[index]->m_state == TS_wait)
		return	true;

	return	false;
}

void	CNPC::SetConversationStateYes(int index)
{
	m_VConversation[index]->m_state = TS_yes;
}

void	CNPC::SetConversationStateNo(int index)
{
	m_VConversation[index]->m_state = TS_no;
}

int		CNPC::GetConversationIndex(int index)
{
	int k = -1;
	for (int i = 0; i < m_VConversation.size(); i++)
	{
		if (!m_VConversation[i]->m_hidden)
		{
			k++;
		}

		if (k == index)
		{
			return	i;
		}
	}

	return	-1;
}

void	CNPC::GetTextIndex(int &value,int index)
{
	value = m_VConversation[index]->GetTextIndex();
}