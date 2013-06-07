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
#include "SDL.h"

class CEventMessage;

class CEvent
{

private:

	SDL_Event		m_event;

	void			PressedLeftMouse(int x,int y,CEventMessage	*EventMessage);
	void			PressedRightMouse(int x,int y,CEventMessage	*EventMessage);
	void			PressedLeft(CEventMessage	*EventMessage);
	void			PressedRight(CEventMessage	*EventMessage);
	void			PressedUp(CEventMessage	*EventMessage);
	void			PressedDown(CEventMessage	*EventMessage);
	void			PressedEsc(CEventMessage	*EventMessage);
	void			PressedSpace(CEventMessage	*EventMessage);
	void			PressedF(CEventMessage	*EventMessage);
	void			Pressed1(CEventMessage *EventMessage);
	void			Pressed2(CEventMessage *EventMessage);
	void			Pressed3(CEventMessage *EventMessage);
	void			Pressed4(CEventMessage *EventMessage);
	void			Pressed5(CEventMessage *EventMessage);
	void			PressedO(CEventMessage *EventMessage);

	void			ReleasedLeftMouse(int x,int y,CEventMessage	*EventMessage);
	void			ReleasedRightMouse(int x,int y,CEventMessage	*EventMessage);
	void			ReleasedLeft(CEventMessage	*EventMessage);
	void			ReleasedRight(CEventMessage	*EventMessage);
	void			ReleasedUp(CEventMessage	*EventMessage);
	void			ReleasedDown(CEventMessage	*EventMessage);
	void			ReleasedEsc(CEventMessage	*EventMessage);
	void			ReleasedSpace(CEventMessage	*EventMessage);
	void			ReleasedF(CEventMessage	*EventMessage);
	void			Released1(CEventMessage *EventMessage);
	void			Released2(CEventMessage *EventMessage);
	void			Released3(CEventMessage *EventMessage);
	void			Released4(CEventMessage *EventMessage);
	void			Released5(CEventMessage *EventMessage);
	void			ReleasedO(CEventMessage *EventMessage);

	void			MouseMovement(CEventMessage *EventMessage);

public:

	bool			Init();
	void			Clean();

	bool			CheckEvents(CEventMessage *EventMessage);

};