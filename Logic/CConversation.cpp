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

#include "CConversation.h"

int		CConversation::GetTextIndex()
{
	int	index = 0;

	if (m_state == TS_introduction)
		index = 1;
	else if (m_state == TS_option)
		index = 2;
	else if (m_state == TS_yes)
		index = 3;
	else if (m_state == TS_no)
		index = 4;
	else if (m_state == TS_wait)
		index = 5;

	return	index;
}

void	CConversation::UpdateState()
{
	if (m_state == TS_introduction)
		m_state = TS_option;
	else if (m_state == TS_yes)
		m_state = TS_wait;
	else if (m_state == TS_no)
		m_state = TS_introduction;
	else if (m_state == TS_wait)
		m_state = TS_finished;
}