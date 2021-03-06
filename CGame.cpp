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
#include "Render\CSDLRender.h"
#include "Logic\CLogic.h"
#include "Events\CEvent.h"
#include "Events\CEventMessage.h"
#include "configuration.h"
#include "Logic\Entity\CEntityManager.h"
#include "Logic\Entity\CEntity.h"
#include "Logic\Entity\CEnemy.h"
#include "Logic\Map\CMap.h"
#include "Logic\Entity\CPlayer.h"
#include "Logic\Entity\CNPC.h"
#include "Logic\CConversation.h"
#include "Logic\Input\CMouse.h"
#include "Logic\GUI\CGUI.h"
#include "Logic\GUI\CWindow.h"

// default configuration
bool			g_grabMode	=	true;
bool			g_FPSLimit	=	true;
int				g_windowX	=	640;
int				g_windowY	=	480;
int				WINDOW_WIDTH = g_windowX;
int				WINDOW_HEIGHT = g_windowY;

bool	CGame::Init()
{
	WINDOW_WIDTH = g_windowX;
	WINDOW_HEIGHT = g_windowY;

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

	if (!m_pRender->VInit())
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
	m_pRender->VClean();
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

	m_pLogic->m_renderFlags.state	= RS_renderMainMenu;

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

	if (m_pLogic->m_textRenderInfo.setFirstTextBox)
	{
		m_pRender->SetTextBox(m_pLogic->m_pEntity->m_VNpc[m_pLogic->m_textRenderInfo.selectedNPCIndex].m_availableConversations);

		m_pLogic->m_textRenderInfo.setFirstTextBox = false;
	}
	else if (m_pLogic->m_textRenderInfo.setCommonTextBox)
	{
		m_pRender->SetTextBox(m_pLogic->m_pEntity->m_VNpc[m_pLogic->m_textRenderInfo.selectedNPCIndex].m_conversation);

		m_pLogic->m_textRenderInfo.setCommonTextBox = false;
	}
	else if (m_pLogic->m_textRenderInfo.setTextBox)
	{
		int i	=	m_pLogic->m_textRenderInfo.selectedNPCIndex;
		int j	=	m_pLogic->m_textRenderInfo.selectedConversationIndex;
		int	k	=	m_pLogic->m_textRenderInfo.selectedTextIndex;

		m_pRender->SetTextBox(m_pLogic->m_pEntity->m_VNpc[i].m_VConversation[j]->m_VText[k]);

		m_pLogic->m_textRenderInfo.setTextBox = false;
	}

	m_pRender->VReadMessage(m_pLogic->m_pMouse->m_pMessage);

	return	true;
}

bool	CGame::Render(double deltaTime)
{
	if (m_pLogic->m_renderFlags.state == RS_renderMainMenu)
	{
		m_pRender->VClearWindow();
		m_pRender->RenderButtons(m_pLogic->m_pGUI->m_pMainMenu->m_renderData);
		//m_pRender->RenderMainMenu();
	}
	else if (m_pLogic->m_renderFlags.state == RS_renderOptions)
	{
		m_pRender->VClearWindow();
		m_pRender->RenderButtons(m_pLogic->m_pGUI->m_pOptions->m_renderData);
		m_pRender->RenderStringData(m_pLogic->m_pGUI->m_pOptions->m_renderData);
		//m_pRender->RenderOptions();
	}
	else if (m_pLogic->m_renderFlags.state == RS_renderMap)
	{
		m_pRender->RenderMap(m_pLogic->m_pMap);

		if (m_pLogic->m_renderFlags.textBoxState == RTBS_renderFirstTextBox)
		{
			m_pRender->VRenderTextBox(m_pLogic->m_textRenderInfo.chars,m_pLogic->m_textRenderInfo.nextTextBox,true);
		}
		else if (m_pLogic->m_renderFlags.textBoxState	== RTBS_renderTextBox)
		{
			 m_pEventMessage->m_continueConversation = m_pRender->VRenderTextBox(m_pLogic->m_textRenderInfo.chars,m_pLogic->m_textRenderInfo.nextTextBox,false);
		}
		RenderHUD(deltaTime);
	}
	else if (m_pLogic->m_renderFlags.state == RS_renderMapEditor)
	{
		m_pRender->RenderMapEditor(m_pLogic->m_pMap);
	}
	else if (m_pLogic->m_renderFlags.state == RS_renderAllTiles)
	{
		m_pRender->RenderAllTiles(m_pLogic->m_pMap);
	}
	else if (m_pLogic->m_renderFlags.state == RS_renderGameExit)
	{
		
	}

	m_pRender->VUpdateWindow();
	
	return	true;
}

void	CGame::RenderHUD(double deltaTime)
{
	m_pRender->VClearHUD();
	m_pRender->RenderHUD((int)deltaTime,m_pLogic->m_pEntity->m_pPlayer->GetHP(),m_pLogic->m_pEntity->m_pPlayer->GetMP()); // 1000000/deltaTime

	if (m_pLogic->m_renderFlags.renderTalkMessage)
	{
		m_pRender->VRenderText("Press F to talk",WINDOW_WIDTH/3,WINDOW_HEIGHT,102,0,153);
	}

	// for testing
	m_pRender->VRenderValue((int)m_pLogic->m_pMap->m_cameraX,590,WINDOW_HEIGHT+20,98,0,49);
	m_pRender->VRenderValue((int)m_pLogic->m_pMap->m_cameraY,590,WINDOW_HEIGHT+40,98,0,49);
	m_pRender->VRenderValue((int)m_pLogic->m_pMap->m_playerX,530,WINDOW_HEIGHT+20,98,0,49);
	m_pRender->VRenderValue((int)m_pLogic->m_pMap->m_playerY,530,WINDOW_HEIGHT+40,98,0,49);
	m_pRender->VRenderValue(m_pEventMessage->m_MotionEvent.x,470,WINDOW_HEIGHT+20,98,0,49);
	m_pRender->VRenderValue(m_pEventMessage->m_MotionEvent.y,470,WINDOW_HEIGHT+40,98,0,49);
}




