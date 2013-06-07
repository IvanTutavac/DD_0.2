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

#include "debug.h"
#include "Log.h"
#include "CGame.h"
#include "CTimer.h"
//#include "Render\IRender.h"
#include "Render\CSDLRender.h"
#include "Logic\CLogic.h"
#include "Events\CEvent.h"
#include "Events\CEventMessage.h"
#include "configuration.h"
#include "Logic\CEntity.h"
#include "Logic\CMap.h"
#include "Logic\CPlayer.h"
#include "Logic\CNPC.h"
#include "Logic\CConversation.h"
//#include <cstring>
// default configuration
SDL_GrabMode	g_grabMode	=	SDL_GRAB_ON;
bool			g_FPSLimit	=	true;

std::ofstream logDat1;

bool	CGame::Init()
{
	Log("Game Init started");
	if (SDL_Init(SDL_INIT_VIDEO) == -1)
		return	false;
	Log("SDL Init...");
	
	m_pTimer		=	DD_NEW CTimer;
	m_pRender		=	DD_NEW CSDLRender;
	m_pLogic		=	DD_NEW CLogic;
	m_pEvent		=	DD_NEW CEvent;
	m_pEventMessage	=	DD_NEW CEventMessage;

	if (!m_pLogic->Init())
		return	false;
	Log("Logic init finished");

	if (!m_pRender->Init())
		return	false;
	Log("Render init finished");

	if (!m_pEvent->Init())
		return false;
	Log("Event init finished");

	m_pEventMessage->Init();

	return	true;
}

bool	CGame::Clean()
{
	m_pRender->Clean();
	m_pLogic->Clean();
	m_pEvent->Clean();

	DD_DELETE(m_pTimer);
	DD_DELETE(m_pRender);
	DD_DELETE(m_pLogic);
	DD_DELETE(m_pEvent);
	DD_DELETE(m_pEventMessage);

	SDL_Quit();

	return	true;
}

bool	CGame::Run()
{
	m_gameLoop	=	true;
	m_chars		=	0; 

	// zbog testa, kasnije bi to state ili neki vrag trebao raditi
	//m_pLogic->m_renderFlags.renderMap	=	true; // remove later
	m_pLogic->m_renderFlags.state	= RS_renderMainMenu;
	//m_pLogic->m_renderFlags.renderTextBox = true;

	double	deltaTime	=	0;
	double  fps			=	0;
	double  currentTime =   0;

	m_pTimer->Start();
	double vSyncLastTime = m_pTimer->GetTimeEx(); 

	while (m_gameLoop)
	{
		currentTime = m_pTimer->GetTimeEx(); 
		if (!Events())
			break;
		m_pTimer->End();
		deltaTime = m_pTimer->GetTime();
		fps		  = m_pTimer->GetMicro();
		m_pTimer->Start();
		m_gameLoop = Logic(deltaTime);
		if (g_FPSLimit)
		{
			if (vSyncLastTime + 8333.333333 <= currentTime)
			{
				fps	= (currentTime-vSyncLastTime);
				vSyncLastTime	=	currentTime; 
				Render(1000000/fps);
			}
		}
		else 
		{
			fps	= (currentTime-vSyncLastTime);
			vSyncLastTime	=	currentTime; 
			Render(1000000/fps);
		}
	}
	return	true;
}

// private		

bool	CGame::Events()
{
	return	m_pEvent->CheckEvents(m_pEventMessage);
}

bool	CGame::Logic(double deltaTime)
{
	if (!m_pLogic->Run(m_pEventMessage,deltaTime))
		return	false;
	if (m_pLogic->m_setTextBox)
	{
		m_pRender->SetTextBox(m_pLogic->m_VNpc[0]->m_VConversation[0]->m_VText[0]);
		m_pLogic->m_setTextBox = false;
	}
	return	true;
}

bool	CGame::Render(double deltaTime)
{
	if (m_pLogic->m_renderFlags.state == RS_renderMainMenu)
	{
		m_pRender->ClearWindow();
		m_pRender->RenderMainMenu();
	}
	else if (m_pLogic->m_renderFlags.state == RS_renderOptions)
	{
		m_pRender->ClearWindow();
		m_pRender->RenderOptions();
	}
	else if (m_pLogic->m_renderFlags.state == RS_renderMap)
	{
		m_pRender->RenderMap(m_pLogic->m_pMap);
		if (m_pLogic->m_renderFlags.renderTextBox)
		{
			// before calling RenderTextBox, m_pRender->SetTextBox(m_pLogic->m_VNpc[i]->m_VConversation[k]->m_VText[k]) needs to be called
			m_pRender->RenderTextBox(m_chars,m_pLogic->m_nextTextBox);
		}
		RenderHUD(deltaTime);
	}
	else if (m_pLogic->m_renderFlags.state == RS_renderMapEditor)
	{
		m_pRender->RenderMapEditor(m_pLogic->m_pMap);
	}
	else if (m_pLogic->m_renderFlags.state == RS_renderAllTiles)
	{
		m_pRender->ClearHUD();
		m_pRender->RenderAllTiles(m_pLogic->m_pMap);
	}
	else if (m_pLogic->m_renderFlags.state == RS_renderGameExit)
	{
		
	}

	m_pRender->UpdateWindow();
	
	return	true;
}

void	CGame::RenderHUD(double deltaTime)
{
	m_pRender->ClearHUD();
	m_pRender->RenderHUD(deltaTime,m_pLogic->m_pPlayer->GetHP(),m_pLogic->m_pPlayer->GetMP()); // 1000000/deltaTime

	if (m_pLogic->m_renderFlags.renderTalkMessage)
	{
		m_pRender->RenderText("Press F to talk",240,480,102,0,153);
	}

	// for testing
	m_pRender->RenderValue(m_pLogic->m_pMap->m_cameraX,590,500,98,0,49);
	m_pRender->RenderValue(m_pLogic->m_pMap->m_cameraY,590,520,98,0,49);
	m_pRender->RenderValue(m_pLogic->m_pMap->m_playerX,530,500,98,0,49);
	m_pRender->RenderValue(m_pLogic->m_pMap->m_playerY,530,520,98,0,49);
	m_pRender->RenderValue(m_pEventMessage->m_Event.x,470,500,98,0,49);
	m_pRender->RenderValue(m_pEventMessage->m_Event.y,470,520,98,0,49);
	m_pRender->RenderValue(m_pEventMessage->m_MotionEvent.x,410,500,98,0,49);
	m_pRender->RenderValue(m_pEventMessage->m_MotionEvent.y,410,520,98,0,49);
}



