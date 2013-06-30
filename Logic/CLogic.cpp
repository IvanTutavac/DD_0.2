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
#include "../Events/CEventMessage.h"
#include "CLogic.h"
#include "Quest\CQuestManager.h"
#include "CMap.h"
#include "CMovement.h"
#include "CPlayer.h"
#include "CEnemy.h"
#include "CNPC.h"
#include "..\CTimer.h"
#include "..\configuration.h"

#pragma pack(1)
struct _enemy
{
	int	hp,mp;
	int typeID;
	char name[20];
};
#pragma pack()

void	_renderFlags::Reset()
{
	renderGameExit		=	false;
	renderTalkMessage	=	false;
	textBoxState		=	RTBS_nothing;
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
	npcConversation = NPCC_nothing;
}

// CLogic functions

bool	CLogic::Init()
{
	Log("Logic init started");

	m_UniqueSpellID		=	0;
	m_UniqueEnemyID		=	0;

	m_pMap			=	DD_NEW CMap;
	m_pMovement		=	DD_NEW CMovement;
	m_pQuest		=	DD_NEW CQuestManager;

	if (!m_pMap->Init())
		return	false;
	Log("Map init finished");

	if (!m_pQuest->Init())
		return	false;
	Log("Quests loaded");

	if (!LoadAllSpells())
		return	false;
	Log("Spells loaded");

	if (!LoadAllEntities())
		return	false;
	Log("Entities loaded");

	InitFlags();

	m_IDLimit							=	999999;
	m_drawTiles							=	false;
	m_textRenderInfo.nextTextBox		=	false;
	m_textRenderInfo.setTextBox			=	false;
	m_textRenderInfo.setFirstTextBox	=	false;
	m_textRenderInfo.setCommonTextBox	=	false;

	return	true;
}

void	CLogic::Clean()
{
	m_pMap->Clean();
	DD_DELETE(m_pMap);

	m_pQuest->Clean();
	DD_DELETE(m_pQuest);

	m_pPlayer->Clean();
	DD_DELETE(m_pPlayer);

	DD_DELETE(m_pMovement);

	for (size_t i = 0; i < m_VEnemyList.size(); i++)
	{
		m_VEnemyList[i].Clean();
		//DD_DELETE(m_VEnemyList[i]);
	}

	for (size_t i = 0; i < m_VEnemy.size(); i++)
	{
		m_VEnemy[i].Clean();
	}

	for (size_t i = 0; i < m_VCloseEnemy.size(); i++)
	{
		m_VCloseEnemy[i].Clean();
	}

	for (size_t i = 0; i < m_VNpc.size(); i++)
	{
		m_VNpc[i].Clean();
	}
}

bool	CLogic::LoadAllEntities()
{
	Log("Entities...");
	m_pPlayer = DD_NEW CPlayer;

	m_pPlayer->m_VSpell.push_back(m_VAllSpells[0]);
	m_pPlayer->m_spell.push_back(m_VAllSpells[0]); // make a limit for 5 spells !

	m_pPlayer->m_VSpell.push_back(m_VAllSpells[1]);
	m_pPlayer->m_spell.push_back(m_VAllSpells[1]);

	m_pPlayer->SetHP(50);
	m_pPlayer->SetMP(50);

	if (!m_pPlayer->Init())
		return	false;
	Log("Player loaded");

	if (!LoadEnemies())
		return	false;
	Log("Enemies loaded");
	
	SetMapEnemy(); 

	CNPC	NPC;//*NPC = DD_NEW CNPC;

	if (!NPC.Init())
		return	false;

	NPC.SetQuestID(0,0); // testing...

	m_VNpc.push_back(NPC);

	CNPC	NPC1;//*NPC1 = DD_NEW CNPC;

	if (!NPC1.Init("Just testing..."))
		return	true;

	m_VNpc.push_back(NPC1);

	Log("Npc loaded");

	return	true;
}

bool	CLogic::LoadEnemies()
{
	// enemy list... 
	// map should contain enemy locations, and logic should contain enemies copied from enemyList by typeID, they should get ID++;

	_enemy	enemy;
	std::fstream dat;

	dat.open("data/enemy.dat",std::ios::in | std::ios::binary);

	if (!dat.is_open())
	{
		dat.clear();
		dat.close();
		return	false;
	}
	while (true)
	{
		dat.read((char*)&enemy,sizeof(enemy));

		if (dat.eof())
			break;

		CEnemy	temp;

		if (!temp.Init())
		{
			dat.clear();
			dat.close();
			return	false;
		}

		temp.SetName(enemy.name);
		temp.SetHP(enemy.hp);
		temp.SetMP(enemy.mp);
		temp.SetTypeID(enemy.typeID);
		temp.SetID(-1);
		m_VEnemyList.push_back(temp);
	}

	dat.clear();
	dat.close();
	return	true;
}

bool	CLogic::SetMapEnemy()
{
	// fali provjera dal je uniqueEnemyID > m_IDLimit
	// trebalo bi citati iz datoteka koji neprijatelj je na kojoj lokaciji mape
	// map editor bi to trebao raditi
	CEnemy Enemy;// = DD_NEW CEnemy;

	if (!Enemy.Init())
		return false;

	Enemy.SetHP(m_VEnemyList[0].GetHP());
	Enemy.SetMP(m_VEnemyList[0].GetMP());
	Enemy.SetName(m_VEnemyList[0].GetName());
	Enemy.SetTypeID(m_VEnemyList[0].GetTypeID());
	Enemy.SetID(m_UniqueEnemyID++);

	m_VCloseEnemy.push_back(Enemy);

	_location	temp;
	temp.ID = m_VCloseEnemy.back().GetID();
	temp.imgID = m_VCloseEnemy.back().GetTypeID();
	temp.x = 32*2,temp.y = 32*7;

	m_pMap->m_closeEnemyXY.push_back(temp);

	return	true;
}

bool	CLogic::LoadAllSpells()
{
	_spell	dummySpell;
	dummySpell.cost = 10;
	dummySpell.type = ST_hpRemoval;
	dummySpell.duration = 3;
	dummySpell.value = 25;
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
	dummySpell.value = 25;
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
		UpdateConversationState();
	}

	return	true;
}

void	CLogic::UpdateObjects()
{
	// implementirati pretragu koja svrstava blizu neprijatelje u VCloseEnemy vektor, ako se udalje izbaci ih
	// kako ne bi trebali stalno letjeti kroz listu

	m_pPlayer->UpdateSpellDuration(deltaTime);

	for (size_t i = 0; i < m_VCloseEnemy.size(); i++)
	{
		m_VCloseEnemy[i].UpdateSpellDuration(deltaTime);
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
		{
			m_pMovement->CameraMovement(EventMessage->m_MotionEvent.x,EventMessage->m_MotionEvent.y,m_pMap->m_cameraX,m_pMap->m_cameraY,deltaTime);
		}
	}

	if (m_lockFlags.movement)
		return;

	if (m_logicFlags.state != LGS_inGame)
		return;

	// move player
	m_pMap->m_tempPlayerX = m_pMap->m_playerX;
	m_pMap->m_tempPlayerY = m_pMap->m_playerY;
	m_pMovement->MoveEntity(EventMessage->m_moveLeftRight,EventMessage->m_moveUpDown,m_pMap->m_playerX,m_pMap->m_playerY,m_pMap->m_playerSpeed,deltaTime);

	m_pMovement->SpellMovement(m_pMap,(float)EventMessage->m_MotionEvent.x,(float)EventMessage->m_MotionEvent.y,deltaTime);

}

void	CLogic::Nearby(CEventMessage *EventMessage)
{
	if (CheckIfNPCNearby())
	{
		if (EventMessage->m_Event.Event == AE_PressedF && !m_logicFlags.npcConversation)
		{
			m_textRenderInfo.chars					=	0;
			m_lockFlags.cameraMovement				=	true;
			m_lockFlags.movement					=	true;
			m_lockFlags.scroll						=	true;
			EventMessage->m_continueConversation	=	true;
			m_textRenderInfo.selectedNPCIndex		=	m_nearNPCIndex;

			// starting conversation, get available conversations
			m_VNpc[m_nearNPCIndex].AvailableConversations();

			// if no (quest) conversation available 
			if (m_VNpc[m_nearNPCIndex].m_availableConversations.size() < 1)
			{
				m_logicFlags.npcConversation		=	NPCC_commonTalk;
				m_textRenderInfo.setCommonTextBox	=	true;
				m_renderFlags.textBoxState			=	RTBS_renderTextBox; 
			}
			else
			{
				m_logicFlags.npcConversation		=	NPCC_questTalk;
				m_textRenderInfo.setFirstTextBox	=	true;
				m_renderFlags.textBoxState			=	RTBS_renderFirstTextBox;
			}
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
	SpellCollision();
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

	// nesto bi trebalo loviti vracenu vrijednost i ako je false, vratiti korisnika u izbornik
	CheckIfAlive();
}
// end of main logic functions

void	CLogic::UpdateConversationState()
{
	if (m_logicFlags.npcConversation == NPCC_questTalk)
	{
		// if wait state, to continue we need to check if the quest has been completed 
		// if not, we won't update conversation status 
		if (m_VNpc[m_textRenderInfo.selectedNPCIndex].ConversationWait(m_textRenderInfo.selectedConversationIndex))
		{
			int questID = m_VNpc[m_textRenderInfo.selectedNPCIndex].GetQuestID(m_textRenderInfo.selectedConversationIndex);

			// valjda ???
			if (!m_pQuest->CheckQuestCompleted(questID)) // quest has not been completed 
			{
				EndConversation();
				return;
			}
			else  
			{
				//completed = false because of CheckQuestCompleted 
				EndConversation();
				m_VNpc[m_textRenderInfo.selectedNPCIndex].UpdateConversation(m_textRenderInfo.selectedConversationIndex);
				m_pQuest->SetActiveQuest(questID,false);
				return;
			}
		}
		// update Conversation status
		m_VNpc[m_textRenderInfo.selectedNPCIndex].UpdateConversation(m_textRenderInfo.selectedConversationIndex);

		// we check for conversation selection 
		if (m_VNpc[m_textRenderInfo.selectedNPCIndex].ConversationSelection(m_textRenderInfo.selectedConversationIndex))
		{
			m_textRenderInfo.setTextBox =	true;
			m_textRenderInfo.chars		=	0;
			m_VNpc[m_textRenderInfo.selectedNPCIndex].GetTextIndex(m_textRenderInfo.selectedTextIndex,m_textRenderInfo.selectedConversationIndex);
		}
		else // yes or no clicked so we finish the conversation
		{
			EndConversation();
			return; // check later
		}
		/*
		// if quest conversation has finished
		if (m_VNpc[m_textRenderInfo.selectedNPCIndex]->ConversationFinished(m_textRenderInfo.selectedConversationIndex))
		{
			int questID = m_VNpc[m_textRenderInfo.selectedNPCIndex]->GetQuestID(m_textRenderInfo.selectedConversationIndex);

			if (questID != -1)
			{
				// quest completed

			}
		}*/
	}
	// end a normal conversation
	else if (m_logicFlags.npcConversation == NPCC_commonTalk)
	{
		EndConversation();
		return;
	}
}

void	CLogic::EndConversation()
{
	m_logicFlags.npcConversation	=	NPCC_nothing;
	m_renderFlags.textBoxState		=	RTBS_nothing;
	m_textRenderInfo.nextTextBox	=	false;
	m_lockFlags.cameraMovement		=	false;
	m_lockFlags.movement			=	false;
	m_lockFlags.scroll				=	false;
}

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
		CheckTextBoxClick(EventMessage);
	}
}

void	CLogic::CheckTextBoxClick(const CEventMessage *EventMessage)
{
	if (m_renderFlags.textBoxState == RTBS_renderTextBox) 
	{
		CheckTextSelectionClick(EventMessage);
	}
	else if (m_renderFlags.textBoxState == RTBS_renderFirstTextBox)
	{
		CheckConversationSelectionClick(EventMessage);
	}
}

void	CLogic::CheckConversationSelectionClick(const CEventMessage *EventMessage)
{
	int height = m_VNpc[m_textRenderInfo.selectedNPCIndex].m_NumConversations;

	if (CheckPointCollision(EventMessage->m_Event.x,EventMessage->m_Event.y,118,240,404,height * 20))
	{
		m_renderFlags.textBoxState			=	RTBS_renderTextBox;
		m_textRenderInfo.setTextBox			=	true;

		for (int i = 0; i < height; i++)
		{
			if (EventMessage->m_Event.y >= 240+20*i && EventMessage->m_Event.y <= 240+20*(i+1))
			{
				// conversation index
				m_textRenderInfo.selectedConversationIndex = m_VNpc[m_textRenderInfo.selectedNPCIndex].GetConversationIndex(i);				
			}
		}

		// text index
		m_VNpc[m_textRenderInfo.selectedNPCIndex].GetTextIndex(m_textRenderInfo.selectedTextIndex,m_textRenderInfo.selectedConversationIndex);
	}
}

void	CLogic::CheckTextSelectionClick(const CEventMessage	*EventMessage)
{
	if(CheckPointCollision(EventMessage->m_Event.x,EventMessage->m_Event.y,118,238,404,84))
	{
		if (m_logicFlags.npcConversation == NPCC_questTalk)
		{
			// yes no selection
			if (m_VNpc[m_textRenderInfo.selectedNPCIndex].ConversationSelection(m_textRenderInfo.selectedConversationIndex))
			{
				if (EventMessage->m_Event.y >= 260 && EventMessage->m_Event.y <= 280) // yes option
				{
					m_textRenderInfo.setTextBox =	true;
					m_textRenderInfo.chars		=	0;
					m_VNpc[m_textRenderInfo.selectedNPCIndex].SetConversationStateYes(m_textRenderInfo.selectedConversationIndex);
					m_VNpc[m_textRenderInfo.selectedNPCIndex].GetTextIndex(m_textRenderInfo.selectedTextIndex,m_textRenderInfo.selectedConversationIndex);

					// set the quest to active
					int questID = m_VNpc[m_textRenderInfo.selectedNPCIndex].GetQuestID(m_textRenderInfo.selectedConversationIndex);
					if (questID != -1)
					{
						m_pQuest->FindIndex(questID);
						m_pQuest->SetActiveQuest(true);
					}

				}
				else if (EventMessage->m_Event.y > 280 && EventMessage->m_Event.y <= 300) // no option
				{
					m_textRenderInfo.setTextBox =	true;
					m_textRenderInfo.chars		=	0;
					m_VNpc[m_textRenderInfo.selectedNPCIndex].SetConversationStateNo(m_textRenderInfo.selectedConversationIndex);
					m_VNpc[m_textRenderInfo.selectedNPCIndex].GetTextIndex(m_textRenderInfo.selectedTextIndex,m_textRenderInfo.selectedConversationIndex);
				}
			}
			else  // next textBox
				m_textRenderInfo.nextTextBox = true;
		}
		else if (m_logicFlags.npcConversation == NPCC_commonTalk)
		{
			m_textRenderInfo.nextTextBox = true;
		}
	}
}

void	CLogic::CheckMapEditorClickRelease(const CEventMessage *EventMessage)
{
	if (CheckPointCollision(EventMessage->m_Event.x,EventMessage->m_Event.y,20,WINDOW_HEIGHT,192,64)) // return button
	{
		m_logicFlags.state		=	LGS_mainMenu;
		m_renderFlags.state		=	RS_renderMainMenu;
	}
	else if (CheckPointCollision(EventMessage->m_Event.x,EventMessage->m_Event.y,210,WINDOW_HEIGHT,192,64)) // all tiles button
	{
		m_logicFlags.state		=	LGS_allTiles;
		m_renderFlags.state		=	RS_renderAllTiles;
		m_pMap->m_cameraX		=	320;
		m_pMap->m_cameraY		=	240;

		if (!m_pMap->m_allTilesMapState)
			m_pMap->InitAllTilesMap();
	}
	else if (CheckPointCollision(EventMessage->m_Event.x,EventMessage->m_Event.y,400,WINDOW_HEIGHT,192,64)) // save button
	{
		m_pMap->SaveMapEditorMap();
	}
	else if (CheckPointCollision(EventMessage->m_Event.x,EventMessage->m_Event.y,0,0,WINDOW_WIDTH,WINDOW_HEIGHT)) // tile setting
	{
		int k = (int)((EventMessage->m_Event.y + m_pMap->m_cameraY-WINDOW_HEIGHT/2)/32);
		int l = (int)((EventMessage->m_Event.x + m_pMap->m_cameraX-WINDOW_WIDTH/2)/32);

		m_pMap->m_mapEditor[k][l] = m_pMap->m_selectedTile;
	}
}

void	CLogic::CheckMapEditorClickPress(const CEventMessage *EventMessage)
{
	// if it's pressed but not released we check motion event, because pressed event has x and y of where the click happened
	// so, while we're holding left mouse click pressed, set the map tile
	if (CheckPointCollision(EventMessage->m_MotionEvent.x,EventMessage->m_MotionEvent.y,0,0,WINDOW_WIDTH,WINDOW_HEIGHT))
	{
		int k		=	(int)((EventMessage->m_MotionEvent.y + m_pMap->m_cameraY-WINDOW_HEIGHT/2)/32);
		float tempL	=	(float)((EventMessage->m_MotionEvent.x + m_pMap->m_cameraX-WINDOW_WIDTH/2)/32);
		int	l		=	(int)tempL;

		if (l > tempL)
			l--;

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

		m_pMap->m_selectedTile	=	m_pMap->m_allTilesMap[l][k];
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
				SetupSpellMap(l,m_pMap->m_playerX,m_pMap->m_playerY,m_pPlayer->m_spell[k].duration,(int)m_pPlayer->m_spell[k].speed);
			}
		}
	}
}

void	CLogic::SetupSpellMap(int id,float x,float y,double duration,int speed)
{
	_location1		newSpell;
	newSpell.x			=	x;
	newSpell.y			=	y;
	newSpell.imgID		=	id;
	newSpell.speed		=	speed;
	newSpell.state		=	LS_nothing;
	newSpell.duration	= duration;

	m_pMap->m_spell.push_back(newSpell); 

	m_pMap->m_spell.back().ID = m_UniqueSpellID;
	m_UniqueSpellID++;

	if (++m_UniqueSpellID > m_IDLimit)
		m_UniqueSpellID = 0;
}

void	CLogic::SpellCollision()
{
	if (m_pMap->m_spell.empty())
		return;

	bool	erase = false;
	bool	search = true;
	for (int i = m_pMap->m_spell.size()-1; i >= 0; i--)
	{
		search = true;

		if (CheckCollision((int)m_pMap->m_playerX,(int)m_pMap->m_playerY,(int)m_pMap->m_spell[i].x,(int)m_pMap->m_spell[i].y,TILE_SIZE))
		{
			// spell kad se tek casta jeste u koliziji s playerom
			// trebalo bi pamtiti ID tko je bacio spell, il enum koji govori dal je neprijatelj il player
			//erase = true;
		}
		
		for (size_t	j = 0; j < m_pMap->m_closeEnemyXY.size(); j++)
		{
			if (!search)
				break;

			if (CheckCollision((int)m_pMap->m_closeEnemyXY[j].x,(int)m_pMap->m_closeEnemyXY[j].y,(int)m_pMap->m_spell[i].x,(int)m_pMap->m_spell[i].y,TILE_SIZE))
			{
				for (size_t k = 0; k < m_VCloseEnemy.size(); k++)
				{
					if (m_VCloseEnemy[k].GetID() == m_pMap->m_closeEnemyXY[j].ID)
					{
						for (size_t l = 0; l < m_VAllSpells.size(); l++)
						{
							if (m_VAllSpells[l].ID == m_pMap->m_spell[i].imgID)
							{
								m_VCloseEnemy[k].ReduceHP(m_VAllSpells[l].value);
								search = false;
								erase = true;
								break;
							}
						}
					}
				}
			}
		}

		if (erase)
		{
			m_pMap->m_spell.erase(m_pMap->m_spell.begin()+i);
			erase = false;
		}
	}
}

void	CLogic::PlayerEnemyCollision()
{
	for (size_t i = 0; i < m_pMap->m_closeEnemyXY.size(); i++)
	{
		if (CheckCollision((int)m_pMap->m_playerX,(int)m_pMap->m_playerY,(int)m_pMap->m_closeEnemyXY[i].x,(int)m_pMap->m_closeEnemyXY[i].y,TILE_SIZE))
		{
			m_pMap->m_playerX = m_pMap->m_tempPlayerX;
			m_pMap->m_playerY = m_pMap->m_tempPlayerY;
		}
	}
}

void	CLogic::PlayerNPCCollision()
{
	for (size_t i = 0; i < m_pMap->m_npcXY.size(); i++)
	{
		if (CheckCollision((int)m_pMap->m_playerX,(int)m_pMap->m_playerY,(int)m_pMap->m_npcXY[i].x,(int)m_pMap->m_npcXY[i].y,TILE_SIZE))
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

bool	CLogic::CheckIfNPCNearby()
{
	for (size_t i = 0; i < m_pMap->m_npcXY.size(); i++)
	{
		if (CheckDistance((int)m_pMap->m_playerX,(int)m_pMap->m_playerY,(int)m_pMap->m_npcXY[i].x,(int)m_pMap->m_npcXY[i].y,40,40))
		{
			m_nearNPCIndex = i;
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
	if (m_pMap->m_spell.size() < 1)
		return;

	for (int i = m_pMap->m_spell.size()-1; i >= 0; i--)
	{
		m_pMap->m_spell[i].duration -= deltaTime;
		if (m_pMap->m_spell[i].duration <= 0)
		{

			m_pMap->m_spell.erase(m_pMap->m_spell.begin()+i);
		}
	}
}

bool	CLogic::CheckIfAlive()
{
	if (m_pPlayer->GetHP() < 1)
		return	false;

	if (m_VCloseEnemy.empty())
		return true;

	for (int i = m_VCloseEnemy.size()-1; i >= 0; i--)
	{
		if (m_VCloseEnemy[i].GetHP() < 1)
		{
			m_pQuest->UpdateQuest(m_VCloseEnemy[i].GetTypeID());

			for (size_t j = m_pMap->m_closeEnemyXY.size()-1; j >= 0; j--)
			{
				if (m_pMap->m_closeEnemyXY[j].ID == m_VCloseEnemy[i].GetID())
				{
					m_pMap->m_closeEnemyXY.erase(m_pMap->m_closeEnemyXY.begin()+j);
					m_VCloseEnemy[i].Clean();
					m_VCloseEnemy.erase(m_VCloseEnemy.begin()+i);
					break;
				}
			}
		}
	}

	return	true;
}
