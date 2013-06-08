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

enum AllEvents
{
	// do not change order
	AE_firstSpell, 
	AE_secondSpell,
	AE_thirdSpell,
	AE_fourthSpell,
	AE_fifthSpell, 
	// from here on it can be changed
	AE_nothing, 
	AE_PressedLeftClick,
	AE_ReleasedLeftClick,
	AE_PressedRightClick,
	AE_ReleasedRightClick,
	AE_PressedEsc,
	AE_PressedSpace,
	AE_PressedF,
	AE_lockedMovement,
	AE_lockedCamera,
	AE_lockedMouseClicks,
	AE_PressedO
};

enum UpDown
{
	UD_nothing,
	UD_up,
	UD_down
};

enum LeftRight
{
	LR_nothing,
	LR_left,
	LR_right
};

enum MotionEvent
{
	ME_nothing,
	ME_moved
};

struct _event
{
	AllEvents	Event; // use AE_
	int		x;
	int		y;
	int		ID; 
};

struct _motionEvent
{
	MotionEvent Event; // use ME_
	int x;
	int y;
	int ID;
};

class CEventMessage
{

public:

	_event			m_Event;
	_motionEvent	m_MotionEvent;
	UpDown			m_moveUpDown;
	LeftRight		m_moveLeftRight;

	bool			m_continueConversation;

	CEventMessage();

	void			Init();
};