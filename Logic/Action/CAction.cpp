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

#include "CAction.h"
#include "..\Map\CMap.h"
#include "..\Message\CMouseMessage.h"
#include "..\..\configuration.h"
#include "..\Entity\CEntityManager.h"
#include "..\Quest\CQuestManager.h"

bool	CAction::Init()
{
	if (g_grabMode == SDL_GRAB_OFF)
	{
		m_moveCamera = false;
	}
	else
	{
		m_moveCamera = true;
	}

	return	true;
}

void	CAction::Clean()
{

}

bool	CAction::ReadMouseMessage(CMouseMessage *Message,CMap *Map,CQuestManager *Quest,CEntityManager *Entity)
{
	bool	returnValue = true;

	if (Message->m_message != MouseMessage::nothing)
	{
		if (Message->m_message == MouseMessage::initMap)
		{
			InitMap(Map);
		}
		else if (Message->m_message == MouseMessage::initMapEditor)
		{
			InitMapEditor(Map);
		}
		else if (Message->m_message == MouseMessage::changeFPSLock)
		{
			ChangeFPSLock();
		}
		else if (Message->m_message == MouseMessage::changeGrabMode)
		{
			ChangeGrabMode();
		}
		else if (Message->m_message == MouseMessage::textQuestSelection)
		{
			this->TextQuestSelection();
		}
		else if (Message->m_message == MouseMessage::textCommonSelection)
		{
			this->NextTextBox();
		}
	}

	// message was dealt with, reset it
	Message->m_message = MouseMessage::nothing;

	return	returnValue;
}

bool	CAction::InitMapEditor(CMap *Map)
{
	Map->m_cameraX = 320;
	Map->m_cameraY = 240;

	if (!Map->LoadMapEditorMap("data/tempMap.dat"))
		return	false;

	return	true;
}

bool	CAction::InitMap(CMap *Map)
{
	Map->m_cameraX = 320;
	Map->m_cameraY = 240;
	// loadiranje next mape, odredjivanje koju mapu ucitati

	return	true;
}

void	CAction::ChangeGrabMode()
{
	if (g_grabMode == SDL_GRAB_ON)
	{
		m_moveCamera = false;
		g_grabMode = SDL_GRAB_OFF;
	}
	else
	{
		m_moveCamera = true;
		g_grabMode = SDL_GRAB_ON;
	}

	SDL_WM_GrabInput(g_grabMode);
}

void	CAction::ChangeFPSLock()
{
	if (g_FPSLimit)
	{
		g_FPSLimit = false;
	}
	else
	{
		g_FPSLimit = true;
	}
}

bool	CAction::isCameraEnabled()
{
	return	m_moveCamera;
}
