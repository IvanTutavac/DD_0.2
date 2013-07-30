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
#include "../Log.h"
#include "../Events/CEventMessage.h"
#include "CLogic.h"
#include "Quest\CQuestManager.h"
#include "Map\CMap.h"
#include "Map\CMovement.h"
#include "Map\CCollision.h"
#include "Entity\CEntityManager.h"
#include "Entity\CPlayer.h"
#include "Entity\CEnemy.h"
#include "Entity\CNPC.h"
#include "Input\CMouse.h"
#include "Action\CAction.h"
#include "GUI\CGUI.h"
#include "GUI\CWindow.h"
#include "CSpell.h"
#include "../CTimer.h"
//#include "../configuration.h"

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

	m_pMap			=	DD_NEW CMap;
	m_pMovement		=	DD_NEW CMovement;
	m_pCollision	=	DD_NEW CCollision;
	m_pQuest		=	DD_NEW CQuestManager;
	m_pEntity		=	DD_NEW CEntityManager;
	m_pSpell		=	DD_NEW CSpell;
	m_pMouse		=	DD_NEW CMouse;
	m_pAction		=	DD_NEW CAction;
	m_pGUI			=	DD_NEW CGUI;

	if (!m_pMap->Init())
		return	false;
	Log("Map init finished");

	if (!m_pMovement->Init())
		return	false;

	if (!m_pCollision->Init())
		return	false;

	if (!m_pMouse->Init())
		return	false;

	if (!m_pAction->Init())
		return	false;

	if (!m_pQuest->Init())
		return	false;
	Log("Quests loaded");

	if (!m_pSpell->Init())
		return	false;
	Log("Spells loaded");

	if (!m_pEntity->Init(m_pSpell))
		return	false;
	Log("Entities loaded");

	if (!m_pGUI->Init())
		return	false;
	Log("GUI loaded");

	m_pEntity->SetMapEnemy(m_pMap);

	InitFlags();

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

	m_pMovement->Clean();
	DD_DELETE(m_pMovement);

	m_pCollision->Clean();
	DD_DELETE(m_pCollision);

	m_pMouse->Clean();
	DD_DELETE(m_pMouse);

	m_pAction->Clean();
	DD_DELETE(m_pAction);

	m_pQuest->Clean();
	DD_DELETE(m_pQuest);

	m_pEntity->Clean();
	DD_DELETE(m_pEntity);

	m_pSpell->Clean();
	DD_DELETE(m_pSpell);

	m_pGUI->Clean();
	DD_DELETE(m_pGUI);
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
	Action(); // reads input
	Nearby(EventMessage); // maybe it's better to have the F button check here 
	Movement(EventMessage);
	Collision();
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

	m_pEntity->m_pPlayer->UpdateSpellDuration(deltaTime);

	for (size_t i = 0; i < m_pEntity->m_VCloseEnemy.size(); i++)
	{
		m_pEntity->m_VCloseEnemy[i].UpdateSpellDuration(deltaTime);
	}
}

void	CLogic::Pathfinding()
{

}

void	CLogic::Movement(const CEventMessage *EventMessage)
{
	if (m_lockFlags.cameraMovement)
		return;

	if (!m_lockFlags.cameraMovement && m_pAction->isCameraEnabled()/*conversation, menus should lock the camera*/  && EventMessage->m_MotionEvent.Event == ME_moved)
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
			m_pEntity->m_VNpc[m_nearNPCIndex].AvailableConversations();

			// if no (quest) conversation available 
			if (m_pEntity->m_VNpc[m_nearNPCIndex].m_availableConversations.size() < 1)
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
	m_pCollision->PlayerEnemyCollision(m_pMap);
	m_pCollision->PlayerNPCCollision(m_pMap);
	m_pCollision->SpellCollision(m_pMap,m_pEntity,m_pSpell);
}

// delete it later !!!!!!!!!!!
#include "Message\CMessage.h"

bool	CLogic::CheckMouseClick(const CEventMessage *EventMessage)
{
	if (m_logicFlags.state == LGS_intro)
	{
	}
	else if (m_logicFlags.state == LGS_mainMenu)
	{
		if (EventMessage->m_Event.Event == AE_ReleasedLeftClick)
		{
			//m_pMouse->MenuCLick(EventMessage->m_Event.x,EventMessage->m_Event.y,m_logicFlags,m_renderFlags);
			m_pGUI->m_pMainMenu->CheckClick(EventMessage->m_Event.x,EventMessage->m_Event.y,ClickType::releasedLeft);
			{
				m_action	=	m_pGUI->m_pMainMenu->m_action;
			}
		}
	}
	else if (m_logicFlags.state == LGS_mainOptions || m_logicFlags.state == LGS_options)
	{
		if (EventMessage->m_Event.Event == AE_ReleasedLeftClick)
		{
			m_pMouse->OptionsClick(EventMessage->m_Event.x,EventMessage->m_Event.y,m_logicFlags,m_renderFlags);
		}
	}
	else if (m_logicFlags.state == LGS_inGame)
	{
		if (EventMessage->m_Event.Event == AE_ReleasedLeftClick)
		{
			m_pMouse->InGameClick(EventMessage->m_Event.x,EventMessage->m_Event.y,m_logicFlags,m_renderFlags);
		}
	}
	else if (m_logicFlags.state == LGS_mapEditor)
	{
		if (EventMessage->m_Event.Event == AE_ReleasedLeftClick)
		{
			m_drawTiles = false;
			m_pMouse->MapEditorClickRelease(EventMessage->m_Event.x,EventMessage->m_Event.y,m_logicFlags,m_renderFlags);
		}
		else if (EventMessage->m_Event.Event == AE_PressedLeftClick || m_drawTiles)
		{
			m_drawTiles = true;
			m_pMouse->MapEditorClickPress(EventMessage->m_MotionEvent.x,EventMessage->m_MotionEvent.y);
		}
	}
	else if (m_logicFlags.state == LGS_allTiles)
	{
		if (EventMessage->m_Event.Event == AE_ReleasedLeftClick)
		{
			m_pMouse->MapEditorAllTilesClick(EventMessage->m_Event.x,EventMessage->m_Event.y,m_logicFlags,m_renderFlags);
		}
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

bool	CLogic::Action()
{
	if (!m_pAction->ReadGUIClick(m_pMap,m_action,m_logicFlags,m_renderFlags))
	{
		return	false;
	}
	if (!m_pAction->ReadMouseMessage(m_pMouse->m_pMessage,m_pMap,m_pQuest,m_pEntity,m_textRenderInfo,m_renderFlags))
	{
		return	false;
	}

	return	true;
}

void	CLogic::FinalCheck()
{
	m_pMap->CheckTimedOutSpells(deltaTime);

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
		if (m_pEntity->m_VNpc[m_textRenderInfo.selectedNPCIndex].ConversationWait(m_textRenderInfo.selectedConversationIndex))
		{
			int questID = m_pEntity->m_VNpc[m_textRenderInfo.selectedNPCIndex].GetQuestID(m_textRenderInfo.selectedConversationIndex);

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
				m_pEntity->m_VNpc[m_textRenderInfo.selectedNPCIndex].UpdateConversation(m_textRenderInfo.selectedConversationIndex);
				m_pQuest->SetActiveQuest(questID,false);
				return;
			}
		}
		// update Conversation status
		m_pEntity->m_VNpc[m_textRenderInfo.selectedNPCIndex].UpdateConversation(m_textRenderInfo.selectedConversationIndex);

		// we check for conversation selection 
		if (m_pEntity->m_VNpc[m_textRenderInfo.selectedNPCIndex].ConversationSelection(m_textRenderInfo.selectedConversationIndex))
		{
			m_textRenderInfo.setTextBox =	true;
			m_textRenderInfo.chars		=	0;
			m_pEntity->m_VNpc[m_textRenderInfo.selectedNPCIndex].GetTextIndex(m_textRenderInfo.selectedTextIndex,m_textRenderInfo.selectedConversationIndex);
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

void	CLogic::PlayerSpellCast(const CEventMessage *EventMessage)
{
	if (EventMessage->m_Event.Event >= 0 && EventMessage->m_Event.Event <= 4) // Pressed button from 1 to 5
	{    
		// EventMessage->m_Event.Event is spell index if check above was true	
		int k = EventMessage->m_Event.Event;

		if (k >= (int)m_pEntity->m_pPlayer->m_spell.size()) // there's no spell in the slot
			return;

		int index = m_pEntity->m_pPlayer->m_spell[k].index;
		int l = m_pSpell->m_VAllSpells[index].ID;
		int lvl = m_pEntity->m_pPlayer->m_spell[index].lvl;

		if (m_pEntity->m_pPlayer->GetMP() < m_pSpell->m_VAllSpells[index].spell[lvl].cost)
			return;

		bool castAllowed = m_pEntity->m_pPlayer->CheckSpellCD(k);

		if (castAllowed)
		{
			int hp = m_pEntity->m_pPlayer->GetHP();
			int mp = m_pEntity->m_pPlayer->GetMP();
			
			m_pSpell->CastSpell(index,lvl,hp,mp);

			m_pEntity->m_pPlayer->SetHP(hp);
			m_pEntity->m_pPlayer->SetMP(mp);

			if (m_pSpell->m_VAllSpells[index].moving)
			{
				m_pSpell->SetupSpellMap(m_pMap,l,m_pMap->m_playerX,m_pMap->m_playerY,m_pSpell->m_VAllSpells[index].spell[lvl].duration,(int)m_pSpell->m_VAllSpells[index].spell[lvl].speed,lvl,-1);
			}
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

bool	CLogic::CheckIfAlive()
{
	if (m_pEntity->m_pPlayer->GetHP() < 1)
		return	false;

	if (m_pEntity->m_VCloseEnemy.empty())
		return true;

	for (int i = m_pEntity->m_VCloseEnemy.size()-1; i >= 0; i--)
	{
		if (m_pEntity->m_VCloseEnemy[i].GetHP() < 1)
		{
			m_pQuest->UpdateQuest(m_pEntity->m_VCloseEnemy[i].GetTypeID());

			m_pMap->m_closeEnemyXY.erase(m_pMap->m_closeEnemyXY.begin()+i);
			m_pEntity->m_VCloseEnemy[i].Clean();
			m_pEntity->m_VCloseEnemy.erase(m_pEntity->m_VCloseEnemy.begin()+i);
		}
	}

	return	true;
}
