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

#include "../debug.h"
#include "..\Log.h"
//#include "../Events/CEvent.h"
#include "../Events/CEventMessage.h"
#include "CLogic.h"
#include "CMap.h"
#include "CPlayer.h"
#include "CEnemy.h"
#include "CNPC.h"
#include "..\CTimer.h"
#include "..\configuration.h"
//#include "SDL.h"

//SDL_GrabMode g_grabMode;

void	_renderFlags::Reset()
{
	renderGameExit		=	false;
	renderTextBox		=	false;
	renderTalkMessage	=	false;
	state				=	RS_nothing;
}

void	_lockFlags::AllowAll()
{
	movement			=	true;
	cameraMovement		=	true;
	scroll				=	true;
}

void	_lockFlags::DisableAll()
{
	movement			=	false;
	cameraMovement		=	false;
	scroll				=	false;
}

void	_logicFlags::Reset()
{
	state = LGS_nothing;
	npcConversation = false;
}

// CLogic functions

bool	CLogic::Init()
{
	Log("Logic init started");
	m_pMap			=	DD_NEW CMap;

	if (!m_pMap->Init())
		return	false;
	Log("Map init finished");

	if (!LoadAllSpells())
		return	false;
	Log("Spells loaded");

	if (!LoadAllEntities())
		return	false;
	Log("Entities loaded");

	InitFlags();

	m_UniqueSpellID		=	0;
	m_IDLimit			=	999999;
	m_drawTiles			=	false;
	m_textRenderInfo.nextTextBox	=	false;
	m_textRenderInfo.setTextBox		=	false;
	nn = 0,nnn=0;

	return	true;
}

void	CLogic::Clean()
{
	m_pMap->Clean();
	DD_DELETE(m_pMap);

	m_pPlayer->Clean();
	DD_DELETE(m_pPlayer);

	for (int i = 0; i < m_VEnemy.size(); i++)
	{
		m_VEnemy[i]->Clean();
		DD_DELETE(m_VEnemy[i]);
	}

	for (int i = 0; i < m_VNpc.size(); i++)
	{
		m_VNpc[i]->Clean();
		DD_DELETE(m_VNpc[i]);
	}
}

bool	CLogic::LoadAllEntities()
{
	Log("Entities...");
	m_pPlayer = DD_NEW CPlayer;

	m_pPlayer->m_VSpell.push_back(m_VAllSpells[0]);
	m_pPlayer->m_spell.push_back(m_VAllSpells[0]); // treba sprijeciti da ima vise od pet elemenata

	m_pPlayer->m_VSpell.push_back(m_VAllSpells[1]);
	m_pPlayer->m_spell.push_back(m_VAllSpells[1]);

	m_pPlayer->SetHP(50);
	m_pPlayer->SetMP(50);

	if (!m_pPlayer->Init())
		return	false;
	Log("Player loaded");

	// dummy enemy

	CEnemy	*dummy = DD_NEW CEnemy;

	if (!dummy->Init())
		return	false;

	dummy->SetHP(50);
	dummy->SetMP(20);

	m_VEnemy.push_back(dummy);
	Log("Enemies loaded");

	CNPC	*NPC = DD_NEW CNPC;

	if (!NPC->Init())
		return	false;

	m_VNpc.push_back(NPC);

	Log("Npc loaded");

	return	true;
}

bool	CLogic::LoadAllSpells()
{
	_spell	dummySpell;
	dummySpell.cost = 10;
	dummySpell.type = ST_hpRemoval;
	dummySpell.duration = 3;
	dummySpell.value = 30;
	dummySpell.moving = true;
	dummySpell.casted = false;
	dummySpell.ID = 0;
	dummySpell.cd = 1;
	dummySpell.tempCD = dummySpell.cd;
	dummySpell.speed = 300;
	strcpy_s(dummySpell.name,"Fire");

	m_VAllSpells.push_back(dummySpell);

	dummySpell.cost	=	0;
	dummySpell.type = ST_hpRemoval;
	dummySpell.duration = 2;
	dummySpell.value = 30;
	dummySpell.moving = true;
	dummySpell.casted = false;
	dummySpell.ID = 1;
	dummySpell.cd = 1;
	dummySpell.tempCD = dummySpell.cd;
	dummySpell.speed = 450;
	strcpy_s(dummySpell.name,"Bolt");

	m_VAllSpells.push_back(dummySpell);

	return	true;
}

void	CLogic::InitFlags()
{
	m_renderFlags.Reset();
	m_lockFlags.DisableAll();
	m_logicFlags.Reset();
	m_logicFlags.state = LGS_mainMenu;
}

bool	CLogic::Run(CEventMessage	*EventMessage,double tempDeltaTime)
{
	if (!CheckState(EventMessage))
		return	false;

	deltaTime = tempDeltaTime;


	UpdateObjects();
	Pathfinding();
	if (!CheckMouseClick(EventMessage))
		return	false;
	CheckPlayerInput(EventMessage);
	Nearby(EventMessage); // maybe it's better to have the F button check here 
	Movement(EventMessage);
	Collision();
	Action();
	FinalCheck();

	if (m_logicFlags.state == LGS_exit)
		return	false;

	EventMessage->m_Event.Event			=	AE_nothing; 
	//EventMessage->m_MotionEvent.Event	=	ME_nothing;

	return	true;
}

// main logic functions

bool	CLogic::CheckState(CEventMessage	*EventMessage)
{
	if (EventMessage->m_Event.Event == AE_PressedEsc)
		return	false;

	if (!EventMessage->m_continueConversation && m_logicFlags.npcConversation)
	{
		m_logicFlags.npcConversation	=	false;
		m_lockFlags.cameraMovement		=	false;
		m_lockFlags.movement			=	false;
		m_lockFlags.scroll				=	false;
		nnn++;
	}

	return	true;
}

void	CLogic::UpdateObjects()
{
	// implementirati pretragu koja svrstava blizu neprijatelje u VCloseEnemy vektor, ako se udalje izbaci ih
	// kako ne bi trebali stalno letjeti kroz listu
	
	m_pPlayer->UpdateSpellDuration(deltaTime);
	
	for (int i = 0; i < m_VCloseEnemy.size(); i++)
	{
		m_VCloseEnemy[i]->UpdateSpellDuration(deltaTime);
	}
}

void	CLogic::Pathfinding()
{

}

void	CLogic::Movement(const CEventMessage *EventMessage)
{
	if (m_lockFlags.cameraMovement)
		return;

	if (!m_lockFlags.cameraMovement && g_grabMode == SDL_GRAB_ON && EventMessage->m_MotionEvent.Event == ME_moved)
	{
		if (m_logicFlags.state == LGS_inGame || m_logicFlags.state == LGS_allTiles || m_logicFlags.state == LGS_mapEditor)
			CameraMovement(EventMessage);
	}

	if (m_lockFlags.movement)
		return;

	if (m_logicFlags.state != LGS_inGame)
		return;

	m_pMap->m_tempPlayerX = m_pMap->m_playerX;
	m_pMap->m_tempPlayerY = m_pMap->m_playerY;
	MoveEntity(EventMessage,m_pMap->m_playerX,m_pMap->m_playerY,m_pMap->m_playerSpeed);

	SpellMovement(EventMessage);
	//MoveSpells(EventMessage);

	return; // movement not valid for other entities for now
	for (int i = 0; i < m_pMap->m_enemyXY.size(); i++)
	{
		//MoveEntity();
	}

}

void	CLogic::Nearby(CEventMessage *EventMessage)
{
	if (CheckIfNPCNearby())
	{
		if (EventMessage->m_Event.Event == AE_PressedF && !m_logicFlags.npcConversation)
		{
			if (nnn == 1)
			{
				nnn =1;
			}
			m_renderFlags.renderTextBox =	true;
			m_textRenderInfo.setTextBox =	true;
			m_textRenderInfo.chars		=	0;
			m_logicFlags.npcConversation =	true;
			m_lockFlags.cameraMovement	=	true;
			m_lockFlags.movement		=	true;
			m_lockFlags.scroll			=	true;
			EventMessage->m_continueConversation = true;
			nn++;
		}
		else
		{
			m_renderFlags.renderTalkMessage = true;
		}
	}
	else
	{
		m_renderFlags.renderTalkMessage = false;
	}
}

void	CLogic::Collision()
{
	PlayerEnemyCollision();
	PlayerNPCCollision();
}

bool	CLogic::CheckMouseClick(const CEventMessage *EventMessage)
{
	if (m_logicFlags.state == LGS_intro)
	{
	}
	else if (m_logicFlags.state == LGS_mainMenu)
	{
		if (EventMessage->m_Event.Event == AE_ReleasedLeftClick)
			if (!CheckMenuClick(EventMessage))
			return	false;
	}
	else if (m_logicFlags.state == LGS_mainOptions || m_logicFlags.state == LGS_options)
	{
		if (EventMessage->m_Event.Event == AE_ReleasedLeftClick)
			CheckOptionsClick(EventMessage);
	}
	else if (m_logicFlags.state == LGS_inGame)
	{
		if (EventMessage->m_Event.Event == AE_ReleasedLeftClick)
			CheckInGameClickRelease(EventMessage);
	}
	else if (m_logicFlags.state == LGS_mapEditor)
	{
		if (EventMessage->m_Event.Event == AE_ReleasedLeftClick)
		{
			m_drawTiles = false;
			CheckMapEditorClickRelease(EventMessage);
		}
		else if (EventMessage->m_Event.Event == AE_PressedLeftClick || m_drawTiles)
		{
			m_drawTiles = true;
			CheckMapEditorClickPress(EventMessage);
		}
	}
	else if (m_logicFlags.state == LGS_allTiles)
	{
		if (EventMessage->m_Event.Event == AE_ReleasedLeftClick)
			CheckAllTilesClick(EventMessage);
	}
	return	true;
}

void	CLogic::CheckPlayerInput(const CEventMessage *EventMessage)
{
	if (m_logicFlags.state == LGS_inGame)
	{
		PlayerSpellCast(EventMessage);

		if (EventMessage->m_Event.Event == AE_PressedO)
		{
			m_logicFlags.state		=	LGS_options;
			m_renderFlags.state		=	RS_renderOptions;
		}
	}
}

void	CLogic::Action()
{

}

void	CLogic::FinalCheck()
{
	CheckTimedOutSpells();
}
// end of main logic functions

bool	CLogic::InitMapEditor()
{
	m_pMap->m_cameraX = 320;
	m_pMap->m_cameraY = 240;

	if (!m_pMap->LoadMapEditorMap("data/tempMap.dat"))
		return	false;

	return	true;
}

bool	CLogic::InitMap()
{
	m_pMap->m_cameraX = 320;
	m_pMap->m_cameraY = 240;
	// loadiranje next mape, odredjivanje koju mapu ucitati

	return	true;
}

bool	CLogic::CheckMenuClick(const CEventMessage *EventMessage)
{
	if (CheckPointCollision(EventMessage->m_Event.x,EventMessage->m_Event.y,224,144,192,64))
	{
		m_logicFlags.state		=	LGS_inGame;
		m_renderFlags.state		=	RS_renderMap;
		if (!InitMap())
			return	false;
	}
	else if (CheckPointCollision(EventMessage->m_Event.x,EventMessage->m_Event.y,224,208,192,64)) // map editor button 
	{
		m_logicFlags.state		=	LGS_mapEditor;
		m_renderFlags.state		=	RS_renderMapEditor;
		if (!InitMapEditor())
			return	false;
	}
	else if (CheckPointCollision(EventMessage->m_Event.x,EventMessage->m_Event.y,224,272,192,64)) // options button 
	{
		m_logicFlags.state		=	LGS_mainOptions;
		m_renderFlags.state		=	RS_renderOptions;
	}
	else if (CheckPointCollision(EventMessage->m_Event.x,EventMessage->m_Event.y,224,336,192,64)) // exit button 
	{
		m_logicFlags.state		=	LGS_exit;
		m_renderFlags.state		=	RS_renderGameExit;
	}
	return	true;
}

void	CLogic::CheckOptionsClick(const CEventMessage *EventMessage)
{
	if (CheckPointCollision(EventMessage->m_Event.x,EventMessage->m_Event.y,128,480,192,64)) // return button 
	{
		if (m_logicFlags.state == LGS_mainOptions)
		{
			m_logicFlags.state		=	LGS_mainMenu;
			m_renderFlags.state		=	RS_renderMainMenu;
		}
		else if (m_logicFlags.state == LGS_options)
		{
			m_logicFlags.state		=	LGS_inGame;
			m_renderFlags.state		=	RS_renderMap;
		}
	}
	else if (CheckPointCollision(EventMessage->m_Event.x,EventMessage->m_Event.y,320,480,192,64))  // exit button 
	{
		m_logicFlags.state	=	LGS_exit;
		m_renderFlags.state	=	RS_renderGameExit;
	}
	else if (CheckPointCollision(EventMessage->m_Event.x,EventMessage->m_Event.y,32,32,192,64))
	{
		if (g_grabMode == SDL_GRAB_ON)
			g_grabMode = SDL_GRAB_OFF;
		else
			g_grabMode = SDL_GRAB_ON;

		SDL_WM_GrabInput(g_grabMode);
	}
	else if (CheckPointCollision(EventMessage->m_Event.x,EventMessage->m_Event.y,96,96,192,64))
	{
		if (g_FPSLimit)
			g_FPSLimit = false;
		else
			g_FPSLimit = true;
	}
}

void	CLogic::CheckInGameClickRelease(const CEventMessage *EventMessage)
{
	if (m_logicFlags.npcConversation)
	{
		if (CheckPointCollision(EventMessage->m_Event.x,EventMessage->m_Event.y,118,238,404,84))
		{
			m_textRenderInfo.nextTextBox = true;
		}
	}
}

void	CLogic::CheckMapEditorClickRelease(const CEventMessage *EventMessage)
{
	if (CheckPointCollision(EventMessage->m_Event.x,EventMessage->m_Event.y,20,WINDOW_HEIGHT,192,64))
	{
		m_logicFlags.state		=	LGS_mainMenu;
		m_renderFlags.state		=	RS_renderMainMenu;
	}
	else if (CheckPointCollision(EventMessage->m_Event.x,EventMessage->m_Event.y,210,WINDOW_HEIGHT,192,64))
	{
		m_logicFlags.state		=	LGS_allTiles;
		m_renderFlags.state		=	RS_renderAllTiles;
		m_pMap->m_cameraX		=	320;
		m_pMap->m_cameraY		=	240;
		if (!m_pMap->m_allTilesMapState)
			m_pMap->InitAllTilesMap();
	}
	else if (CheckPointCollision(EventMessage->m_Event.x,EventMessage->m_Event.y,400,WINDOW_HEIGHT,192,64))
	{
		m_pMap->SaveMapEditorMap();
	}
	else if (CheckPointCollision(EventMessage->m_Event.x,EventMessage->m_Event.y,0,0,WINDOW_WIDTH,WINDOW_HEIGHT))
	{
		int k = (int)((EventMessage->m_Event.y + m_pMap->m_cameraY-WINDOW_HEIGHT/2)/32);
		int l = (int)((EventMessage->m_Event.x + m_pMap->m_cameraX-WINDOW_WIDTH/2)/32);

		m_pMap->m_mapEditor[k][l] = m_pMap->m_selectedTile;
	}
}

void	CLogic::CheckMapEditorClickPress(const CEventMessage *EventMessage)
{
	// if it's pressed but not released we check motion event, because pressed event has x and y of where the click happened
	if (CheckPointCollision(EventMessage->m_MotionEvent.x,EventMessage->m_MotionEvent.y,0,0,WINDOW_WIDTH,WINDOW_HEIGHT))
	{
		int k = (int)((EventMessage->m_MotionEvent.y + m_pMap->m_cameraY-WINDOW_HEIGHT/2)/32);
		int l = (int)((EventMessage->m_MotionEvent.x + m_pMap->m_cameraX-WINDOW_WIDTH/2)/32);

		m_pMap->m_mapEditor[k][l] = m_pMap->m_selectedTile;
	}
}

void	CLogic::CheckAllTilesClick(const CEventMessage *EventMessage)
{
	if (CheckPointCollision(EventMessage->m_Event.x,EventMessage->m_Event.y,20,WINDOW_HEIGHT,192,64))
	{
		m_logicFlags.state		=	LGS_mapEditor;
		m_renderFlags.state		=	RS_renderMapEditor;
	}
	else if (CheckPointCollision(EventMessage->m_Event.x,EventMessage->m_Event.y,0,0,WINDOW_WIDTH,WINDOW_HEIGHT))
	{
		//int k = (int)((EventMessage->m_Event.y + m_pMap->m_cameraY-WINDOW_HEIGHT/2)/32);
		//int l = (int)((EventMessage->m_Event.x + m_pMap->m_cameraX-WINDOW_WIDTH/2)/32);

		int k = EventMessage->m_Event.x / 32;
		int l = EventMessage->m_Event.y / 32;

		m_pMap->m_selectedTile	=	m_pMap->m_allTilesMap[k][l];
	}
}

void	CLogic::PlayerSpellCast(const CEventMessage *EventMessage)
{
	if (EventMessage->m_Event.Event >= 0 && EventMessage->m_Event.Event <= 4) // Pressed button from 1 to 5
	{    
		// EventMessage->m_Event.Event  becomes spell index if check above was true	
		int k = EventMessage->m_Event.Event;
		int l = m_pPlayer->m_spell[k].ID;

		if (m_pPlayer->GetMP() < m_pPlayer->m_spell[k].cost)
			return;

		bool castAllowed = m_pPlayer->CheckSpellCD(l);

		if (castAllowed)
		{
			m_pPlayer->CastSpell(k);

			if (m_pPlayer->m_spell[k].moving)
			{
				SetupSpellMap(l,m_pMap->m_playerX,m_pMap->m_playerY,m_pPlayer->m_spell[k].duration,m_pPlayer->m_spell[k].speed);
			}
		}
	}
}

void	CLogic::SetupSpellMap(int id,int x,int y,int duration,int speed)
{
	_location1		newSpell;
	newSpell.x		=	x;
	newSpell.y		=	y;
	newSpell.imgID	=	id;
	newSpell.speed  =	speed;
	newSpell.state	=	LS_nothing;
	newSpell.duration = duration;

	m_pMap->m_spell.push_back(newSpell); 

	m_pMap->m_spell.back().ID = m_UniqueSpellID;
	m_UniqueSpellID++;
	if (m_UniqueSpellID++ > m_IDLimit)
		m_UniqueSpellID = 0;
}

void	CLogic::PlayerEnemyCollision()
{
	for (int i = 0; i < m_pMap->m_enemyXY.size(); i++)
	{
		if (CheckCollision(m_pMap->m_playerX,m_pMap->m_playerY,m_pMap->m_enemyXY[i].x,m_pMap->m_enemyXY[i].y,TILE_SIZE))
		{
			m_pMap->m_playerX = m_pMap->m_tempPlayerX;
			m_pMap->m_playerY = m_pMap->m_tempPlayerY;
		}
	}
}

void	CLogic::PlayerNPCCollision()
{
	for (int i = 0; i < m_pMap->m_npcXY.size(); i++)
	{
		if (CheckCollision(m_pMap->m_playerX,m_pMap->m_playerY,m_pMap->m_npcXY[i].x,m_pMap->m_npcXY[i].y,TILE_SIZE))
		{
			m_pMap->m_playerX = m_pMap->m_tempPlayerX;
			m_pMap->m_playerY = m_pMap->m_tempPlayerY;
		}
	}
}

bool	CLogic::CheckCollision(int x1,int y1,int x2,int y2,int size)
{
	if (x1+size > x2 && x1 < x2+size && y1+size > y2 && y1 < y2+size)
		return	true;
	return	false;
}

bool	CLogic::CheckPointCollision(int x1,int y1,int x2,int y2,int sizeX,int sizeY)
{
	if (x1 > x2 && x1 < x2 + sizeX && y1 > y2 && y1 < y2 + sizeY)
		return	true;
	return	false;
}

void	CLogic::SpellMovement(const CEventMessage *EventMessage)
{
	float x = 0, y = 0, d = 0;
	for (int i = m_pMap->m_spell.size()-1; i >= 0; i--)
	{
		if (m_pMap->m_spell[i].state == LS_nothing)
		{
			x = EventMessage->m_MotionEvent.x - m_pMap->m_playerX;
			y = EventMessage->m_MotionEvent.y - m_pMap->m_playerY;

			d = sqrt(x*x+y*y);

			x/= d;
			y/= d;

			m_pMap->m_spell[i].tempX = x;
			m_pMap->m_spell[i].tempY = y;
			m_pMap->m_spell[i].state = LS_moving;
		}

		m_pMap->m_spell[i].x += m_pMap->m_spell[i].tempX * deltaTime * m_pMap->m_spell[i].speed;
		m_pMap->m_spell[i].y += m_pMap->m_spell[i].tempY * deltaTime * m_pMap->m_spell[i].speed;

		if (m_pMap->m_spell[i].x < 0 || m_pMap->m_spell[i].x > MAP_WIDTH-TILE_SIZE || m_pMap->m_spell[i].y < 0 || m_pMap->m_spell[i].y > MAP_HEIGHT-TILE_SIZE)
		{ 
			// spell outside of map
			//DD_DELETE(m_pMap->m_spell[i].timer);
			m_pMap->m_spell.erase(m_pMap->m_spell.begin()+i);
		}
	}
}

void	CLogic::CameraMovement(const CEventMessage *EventMessage)
{
	float v = 600;
	float speed	= v*0.70710678118;

	if (EventMessage->m_MotionEvent.x >= WINDOW_WIDTH-1 && m_pMap->m_cameraX < MAP_WIDTH - WINDOW_WIDTH/2)
	{
		if (EventMessage->m_MotionEvent.y <= 0 && m_pMap->m_cameraY > WINDOW_HEIGHT/2)
		{
			m_pMap->m_cameraY -= speed * deltaTime;
			m_pMap->m_cameraX += speed * deltaTime;
		}
		else if (EventMessage->m_MotionEvent.y >= WINDOW_HEIGHT-1+HUD_HEIGHT && m_pMap->m_cameraY < MAP_HEIGHT - WINDOW_HEIGHT/2)
		{
			m_pMap->m_cameraY += speed * deltaTime;
			m_pMap->m_cameraX += speed * deltaTime;
		}
		else
			m_pMap->m_cameraX += v * deltaTime;
	}
	else if (EventMessage->m_MotionEvent.x <= 0 && m_pMap->m_cameraX > WINDOW_WIDTH/2)
	{
		if (EventMessage->m_MotionEvent.y <= 0 && m_pMap->m_cameraY > WINDOW_HEIGHT/2)
		{
			m_pMap->m_cameraY -= speed * deltaTime;
			m_pMap->m_cameraX -= speed * deltaTime;
		}
		else if (EventMessage->m_MotionEvent.y >= WINDOW_HEIGHT-1+HUD_HEIGHT && m_pMap->m_cameraY < MAP_HEIGHT - WINDOW_HEIGHT/2)
		{
			m_pMap->m_cameraY += speed * deltaTime;
			m_pMap->m_cameraX -= speed * deltaTime;
		}
		else
			m_pMap->m_cameraX -= v * deltaTime;
	}
	else if (EventMessage->m_MotionEvent.y >= WINDOW_HEIGHT-1+HUD_HEIGHT && m_pMap->m_cameraY < MAP_HEIGHT - WINDOW_HEIGHT/2)
	{
		m_pMap->m_cameraY += v * deltaTime;
	}
	else if (EventMessage->m_MotionEvent.y <= 0 && m_pMap->m_cameraY > WINDOW_HEIGHT/2)
	{
		m_pMap->m_cameraY -= v * deltaTime;
	}

	if (m_pMap->m_cameraX > MAP_WIDTH - WINDOW_WIDTH/2)
		m_pMap->m_cameraX = MAP_WIDTH - WINDOW_WIDTH/2;	
	else if (m_pMap->m_cameraX < WINDOW_WIDTH/2)
		m_pMap->m_cameraX = WINDOW_WIDTH/2;

	if (m_pMap->m_cameraY < WINDOW_HEIGHT/2)
		m_pMap->m_cameraY = WINDOW_HEIGHT/2;
	else if (m_pMap->m_cameraY > MAP_HEIGHT - WINDOW_HEIGHT/2)
		m_pMap->m_cameraY = MAP_HEIGHT - WINDOW_HEIGHT/2;

}

void	CLogic::MoveEntity(const CEventMessage *EventMessage,float &x,float &y,int speed)
{
	//m_pMap->m_tempPlayerX = m_pMap->m_playerX; // will be used to reset movement if needed
	//m_pMap->m_tempPlayerY = m_pMap->m_playerY;

	if (EventMessage->m_moveLeftRight == LR_right)
	{
		if (EventMessage->m_moveUpDown == UD_up)
		{
			MoveUp(y,speed*0.70710678118);
			MoveRight(x,speed*0.70710678118);
		}
		else if (EventMessage->m_moveUpDown == UD_down)
		{
			MoveDown(y,speed*0.70710678118);
			MoveRight(x,speed*0.70710678118);
		}
		else
			MoveRight(x,speed);
	}
	else if (EventMessage->m_moveLeftRight == LR_left)
	{
		if (EventMessage->m_moveUpDown == UD_up)
		{
			MoveUp(y,speed*0.70710678118);
			MoveLeft(x,speed*0.70710678118);
		}
		else if (EventMessage->m_moveUpDown == UD_down)
		{
			MoveDown(y,speed*0.70710678118);
			MoveLeft(x,speed*0.70710678118);
		}
		else
			MoveLeft(x,speed);
	}
	else if (EventMessage->m_moveUpDown == UD_up)
		MoveUp(y,speed);
	else if (EventMessage->m_moveUpDown == UD_down)
		MoveDown(y,speed);
}

void	CLogic::MoveRight(float &x,float speed,bool flag)
{
	float	temp	=	speed * deltaTime;
	x += temp;
	if (x > MAP_WIDTH - TILE_SIZE && flag)
	{
		x = MAP_WIDTH - TILE_SIZE;
	}
}

void	CLogic::MoveLeft(float &x,float speed,bool flag)
{
	float	temp	=	speed * deltaTime;
	x -= temp;
	if (x < 0 && flag)
	{
		x = 0;
	}
}
void	CLogic::MoveUp(float &y,float speed,bool flag)
{
	float	temp	=	speed * deltaTime;
	y -= temp;
	if (y < 0 && flag)
	{
		y = 0;
	}
}
void	CLogic::MoveDown(float &y,float speed,bool flag)
{
	float	temp	=	speed * deltaTime;
	y += temp;
	if (y > MAP_HEIGHT - TILE_SIZE && flag)
	{
		y = MAP_HEIGHT - TILE_SIZE - 1;
	}
}

bool	CLogic::CheckIfNPCNearby()
{
	for (int i = 0; i < m_pMap->m_npcXY.size(); i++)
	{
		if (CheckDistance(m_pMap->m_playerX,m_pMap->m_playerY,m_pMap->m_npcXY[i].x,m_pMap->m_npcXY[i].y,40,40))
		{
			m_textRenderInfo.selectedNPCIndex = i;
			return	true;
		}
	}
	return	false;
}

bool	CLogic::CheckDistance(int x1,int y1,int x2,int y2,int distanceX,int distanceY)
{
	if ((abs(x2-x1) < distanceX) && (abs(y2-y1) < distanceY))
		return	true;
	return	false;
}

void	CLogic::CheckTimedOutSpells()
{
	for (int i = m_pMap->m_spell.size()-1; i >= 0; i--)
	{
		m_pMap->m_spell[i].duration -= deltaTime;
		if (m_pMap->m_spell[i].duration <= 0)
		{
			
			m_pMap->m_spell.erase(m_pMap->m_spell.begin()+i);
		}
	}
}
