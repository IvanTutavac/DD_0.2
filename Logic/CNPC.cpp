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
	std::string	text13	=	"Yes";
	std::string	text14	=	"Nothing, nothing...";
	std::string someText = "prva druga treca cetvrta peta sesta sedma osma deveta deseta bla prva1 druga1 treca1 cetvrta1 peta1 sesta1 sedma1 osma1 deveta1 deseta1 bla ibla trla";

	Conversation->m_VText.push_back(text11);
	Conversation->m_VText.push_back(text12);
	Conversation->m_VText.push_back(text13);
	Conversation->m_VText.push_back(text14);
	Conversation->m_VText.push_back(someText);
	Conversation->m_hidden = false;

	m_VConversation.push_back(Conversation);

	CConversation	*Conversation1	=	DD_NEW	CConversation;

	std::string	text21	=	"Second option(test)";
	std::string	text22	=	"Introduction";
	std::string	text23	=	"Yes";
	std::string	text24	=	"No";
	std::string	text25	=	"Accepted bla bla";

	Conversation1->m_VText.push_back(text21);
	Conversation1->m_VText.push_back(text22);
	Conversation1->m_VText.push_back(text23);
	Conversation1->m_VText.push_back(text24);
	Conversation1->m_VText.push_back(text25);
	Conversation1->m_hidden = false;

	m_VConversation.push_back(Conversation1);

	CConversation	*Conversation2	=	DD_NEW	CConversation;

	std::string	text31	=	"Third option(test)";
	std::string	text32	=	"Third selected";
	std::string	text33	=	"Yea";
	std::string	text34	=	"None";
	std::string	text35	=	"So, you have to...";

	Conversation2->m_VText.push_back(text31);
	Conversation2->m_VText.push_back(text32);
	Conversation2->m_VText.push_back(text33);
	Conversation2->m_VText.push_back(text34);
	Conversation2->m_VText.push_back(text35);
	Conversation2->m_hidden = false;

	m_VConversation.push_back(Conversation2);

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