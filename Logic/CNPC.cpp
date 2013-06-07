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

#include "CNPC.h"
#include "CConversation.h"
#include "..\debug.h"

bool	CNPC::Init()
{
	CConversation	*Conversation =	DD_NEW CConversation;

	Conversation->m_VText.push_back("prva druga treca cetvrta peta sesta sedma osma deveta deseta bla prva1 druga1 treca1 cetvrta1 peta1 sesta1 sedma1 osma1 deveta1 deseta1 bla ibla trla");

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