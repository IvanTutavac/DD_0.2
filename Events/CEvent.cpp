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

#include "CEvent.h"
#include "CEventMessage.h"

bool		CEvent::Init()
{
	return	true;
}

void		CEvent::Clean()
{

}

bool		CEvent::CheckEvents(CEventMessage	*EventMessage)
{
	while (SDL_PollEvent(&m_event))
	{
		if (m_event.type == SDL_QUIT)
		{
			return false;
		}
		else if (m_event.type == SDL_KEYDOWN)
		{
			if (m_event.key.keysym.sym == SDLK_ESCAPE)
				PressedEsc(EventMessage);
			else if (m_event.key.keysym.sym == SDLK_LEFT)
				PressedLeft(EventMessage);
			else if (m_event.key.keysym.sym == SDLK_RIGHT)
				PressedRight(EventMessage);
			else if (m_event.key.keysym.sym == SDLK_UP)
				PressedUp(EventMessage);
			else if (m_event.key.keysym.sym == SDLK_DOWN)
				PressedDown(EventMessage);
			else if (m_event.key.keysym.sym == SDLK_SPACE)
				PressedSpace(EventMessage);
			else if (m_event.key.keysym.sym == SDLK_f)
				PressedF(EventMessage);
			else if (m_event.key.keysym.sym == SDLK_1)
				Pressed1(EventMessage);
			else if (m_event.key.keysym.sym == SDLK_2)
				Pressed2(EventMessage);
			else if (m_event.key.keysym.sym == SDLK_3)
				Pressed3(EventMessage);
			else if (m_event.key.keysym.sym == SDLK_4)
				Pressed4(EventMessage);
			else if (m_event.key.keysym.sym == SDLK_5)
				Pressed5(EventMessage);
			else if (m_event.key.keysym.sym == SDLK_o)
				PressedO(EventMessage);
		}
		else if (m_event.type == SDL_KEYUP)
		{
			if (m_event.key.keysym.sym == SDLK_ESCAPE)
				ReleasedEsc(EventMessage);
			else if (m_event.key.keysym.sym == SDLK_LEFT)
				ReleasedLeft(EventMessage);
			else if (m_event.key.keysym.sym == SDLK_RIGHT)
				ReleasedRight(EventMessage);
			else if (m_event.key.keysym.sym == SDLK_UP)
				ReleasedUp(EventMessage);
			else if (m_event.key.keysym.sym == SDLK_DOWN)
				ReleasedDown(EventMessage);
			else if (m_event.key.keysym.sym == SDLK_SPACE)
				ReleasedSpace(EventMessage);
			else if (m_event.key.keysym.sym == SDLK_f)
				ReleasedF(EventMessage);
			else if (m_event.key.keysym.sym == SDLK_1)
				Released1(EventMessage);
			else if (m_event.key.keysym.sym == SDLK_2)
				Released2(EventMessage);
			else if (m_event.key.keysym.sym == SDLK_3)
				Released3(EventMessage);
			else if (m_event.key.keysym.sym == SDLK_4)
				Released4(EventMessage);
			else if (m_event.key.keysym.sym == SDLK_5)
				Released5(EventMessage);
			else if (m_event.key.keysym.sym == SDLK_o)
				ReleasedO(EventMessage);
		}
		else if (m_event.type == SDL_MOUSEBUTTONDOWN)
		{
			if (m_event.button.button == SDL_BUTTON_LEFT)
				PressedLeftMouse(m_event.button.x,m_event.button.y,EventMessage);
			else if (m_event.button.button == SDL_BUTTON_RIGHT)
				PressedRightMouse(m_event.button.x,m_event.button.y,EventMessage);
		}
		else if (m_event.type == SDL_MOUSEBUTTONUP)
		{
			if (m_event.button.button == SDL_BUTTON_LEFT)
				ReleasedLeftMouse(m_event.button.x,m_event.button.y,EventMessage);
			else if (m_event.button.button == SDL_BUTTON_RIGHT)
				ReleasedRightMouse(m_event.button.x,m_event.button.y,EventMessage);
		}
		else if (m_event.type == SDL_MOUSEMOTION)
		{
			MouseMovement(EventMessage);
		}
	}
	return true;
}

void		CEvent::PressedLeftMouse(int x,int y,CEventMessage	*EventMessage)
{
	EventMessage->m_Event.Event		=	AE_PressedLeftClick;
	EventMessage->m_Event.x			=	x;
	EventMessage->m_Event.y			=	y;
}

void		CEvent::PressedRightMouse(int x,int y,CEventMessage	*EventMessage)
{
	EventMessage->m_Event.Event		=	AE_PressedRightClick;
	EventMessage->m_Event.x			=	x;
	EventMessage->m_Event.y			=	y;
}

void		CEvent::PressedLeft(CEventMessage	*EventMessage)
{
	EventMessage->m_moveLeftRight	=	LR_left;
}

void		CEvent::PressedRight(CEventMessage	*EventMessage)
{
	EventMessage->m_moveLeftRight	=	LR_right;
}

void		CEvent::PressedUp(CEventMessage	*EventMessage)
{
	EventMessage->m_moveUpDown		=	UD_up;
}

void		CEvent::PressedDown(CEventMessage	*EventMessage)
{
	EventMessage->m_moveUpDown		=	UD_down;
}

void		CEvent::PressedEsc(CEventMessage	*EventMessage)
{
	EventMessage->m_Event.Event		=	AE_PressedEsc;
}

void		CEvent::PressedSpace(CEventMessage	*EventMessage)
{
	EventMessage->m_Event.Event		=	AE_PressedSpace;
}

void		CEvent::Pressed1(CEventMessage *EventMessage)
{
	EventMessage->m_Event.Event		=	AE_firstSpell;
}

void		CEvent::Pressed2(CEventMessage *EventMessage)
{
	EventMessage->m_Event.Event		=	AE_secondSpell;
}

void		CEvent::Pressed3(CEventMessage *EventMessage)
{
	EventMessage->m_Event.Event		=	AE_thirdSpell;
}

void		CEvent::Pressed4(CEventMessage *EventMessage)
{
	EventMessage->m_Event.Event		=	AE_fourthSpell;
}

void		CEvent::Pressed5(CEventMessage *EventMessage)
{
	EventMessage->m_Event.Event		=	AE_fifthSpell;
}

void		CEvent::PressedO(CEventMessage *EventMessage)
{
	EventMessage->m_Event.Event		=	AE_PressedO;
}

// released

void		CEvent::PressedF(CEventMessage	*EventMessage)
{
	EventMessage->m_Event.Event		=	AE_PressedF;
}

void		CEvent::ReleasedLeftMouse(int x,int y,CEventMessage	*EventMessage)
{
	EventMessage->m_Event.Event		=	AE_ReleasedLeftClick;
	EventMessage->m_Event.x			=	x;
	EventMessage->m_Event.y			=	y;
}

void		CEvent::ReleasedRightMouse(int x,int y,CEventMessage	*EventMessage)
{
	EventMessage->m_Event.Event		=	AE_ReleasedRightClick;
	EventMessage->m_Event.x			=	x;
	EventMessage->m_Event.y			=	y;
}

void		CEvent::ReleasedLeft(CEventMessage	*EventMessage)
{
	EventMessage->m_moveLeftRight	=	LR_nothing;
}

void		CEvent::ReleasedRight(CEventMessage	*EventMessage)
{
	EventMessage->m_moveLeftRight	=	LR_nothing;
}

void		CEvent::ReleasedUp(CEventMessage	*EventMessage)
{
	EventMessage->m_moveUpDown		=	UD_nothing;
}

void		CEvent::ReleasedDown(CEventMessage	*EventMessage)
{
	EventMessage->m_moveUpDown		=	UD_nothing;
}

void		CEvent::ReleasedEsc(CEventMessage	*EventMessage)
{
	EventMessage->m_Event.Event		=	AE_nothing;
}

void		CEvent::ReleasedSpace(CEventMessage	*EventMessage)
{
	EventMessage->m_Event.Event		=	AE_nothing;
}

void		CEvent::ReleasedF(CEventMessage	*EventMessage)
{
	EventMessage->m_Event.Event		=	AE_nothing;
}

void		CEvent::Released1(CEventMessage *EventMessage)
{
	EventMessage->m_Event.Event		=	AE_nothing;
}

void		CEvent::Released2(CEventMessage *EventMessage)
{
	EventMessage->m_Event.Event		=	AE_nothing;
}

void		CEvent::Released3(CEventMessage *EventMessage)
{
	EventMessage->m_Event.Event		=	AE_nothing;
}

void		CEvent::Released4(CEventMessage *EventMessage)
{
	EventMessage->m_Event.Event		=	AE_nothing;
}

void		CEvent::Released5(CEventMessage *EventMessage)
{
	EventMessage->m_Event.Event		=	AE_nothing;
}

void		CEvent::ReleasedO(CEventMessage *EventMessage)
{
	EventMessage->m_Event.Event		=	AE_nothing;
}


void		CEvent::MouseMovement(CEventMessage *EventMessage)
{
	EventMessage->m_MotionEvent.Event	=	ME_moved;
	EventMessage->m_MotionEvent.x		=	m_event.motion.x;
	EventMessage->m_MotionEvent.y		=	m_event.motion.y;
}