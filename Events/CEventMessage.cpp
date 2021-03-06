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

#include "CEventMessage.h"

CEventMessage::CEventMessage()
{
	
}

void	CEventMessage::Init()
{
	m_moveLeftRight		=	LR_nothing;
	m_moveUpDown		=	UD_nothing;
	m_Event.Event		=	AE_nothing;
	m_MotionEvent.Event	=	ME_nothing;
	m_Event.ID			=	0;
	m_Event.x			=	0;
	m_Event.y			=	0;
	m_MotionEvent.ID	=	0;
	m_MotionEvent.x		=	0;
	m_MotionEvent.y		=	0;
	m_continueConversation = true;
}