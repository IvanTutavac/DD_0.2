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
#include "..\Message\CMessage.h"
#include "..\..\configuration.h"
#include "..\Entity\CEntityManager.h"
#include "..\Entity\CNPC.h"
#include "..\Quest\CQuestManager.h"
#include "..\..\const.h"
//#include "SDL.h"

bool	CAction::Init()
{
	if (g_grabMode == false)
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

bool	CAction::ReadMouseMessage(CMessage *Message,CMap *Map,CQuestManager *Quest,CEntityManager *Entity,_TextRenderInfo &textRenderInfo,_renderFlags &renderFlags)
{
	bool	returnValue = true;

	if (Message->m_mouseMessage != MouseMessage::nothing)
	{
		if (Message->m_mouseMessage == MouseMessage::initMap)
		{
			InitMap(Map);
		}
		else if (Message->m_mouseMessage == MouseMessage::initMapEditor)
		{
			InitMapEditor(Map);
		}
		else if (Message->m_mouseMessage == MouseMessage::changeFPSLock)
		{
			ChangeFPSLock(Message);
		}
		else if (Message->m_mouseMessage == MouseMessage::changeGrabMode)
		{
			ChangeGrabMode(Message);
		}
		else if (Message->m_mouseMessage == MouseMessage::changeResolution1024x768)
		{
			ChangeResolution(Message,1024,768);
		}
		else if (Message->m_mouseMessage == MouseMessage::changeResolution800x600)
		{
			ChangeResolution(Message,800,600);
		}
		else if (Message->m_mouseMessage == MouseMessage::changeResolution640x480)
		{
			ChangeResolution(Message,640,480);
		}
		else if (Message->m_mouseMessage == MouseMessage::textQuestSelection)
		{
			TextQuestSelection(Message->x,Message->y,Quest,Entity,textRenderInfo);
		}
		else if (Message->m_mouseMessage == MouseMessage::textCommonSelection)
		{
			NextTextBox(textRenderInfo);
		}
		else if (Message->m_mouseMessage == MouseMessage::conversationSelection)
		{
			ConversationSelection(Message->x,Message->y,textRenderInfo,renderFlags,Entity);
		}
		else if (Message->m_mouseMessage == MouseMessage::setTileSelected)
		{
			SetTileSelected(Message->x,Message->y,Map);
		}
		else if (Message->m_mouseMessage == MouseMessage::setTileOnMap)
		{
			SetTileOnMap(Message->x,Message->y,Map);
		}
		else if (Message->m_mouseMessage == MouseMessage::saveMapEditorMap)
		{
			Map->SaveMapEditorMap();
		}
		else if (Message->m_mouseMessage == MouseMessage::initAllTiles)
		{
			InitAllTiles(Map);
		}
	}

	// message was dealt with, reset it
	Message->m_mouseMessage = MouseMessage::nothing;

	return	returnValue;
}

bool	CAction::ReadGUIClick(CMap *Map,CMessage *Message,Action &action,_logicFlags &logicFlags,_renderFlags &renderFlags)
{
	if (action == Action::startGame)
	{
		if (!StartGame(logicFlags,renderFlags,Map))
			return	false;
	}
	else if (action == Action::loadGame)
	{
		if (!LoadGame(logicFlags,renderFlags,Map))
			return	false;
	}
	else if (action == Action::options)
	{
		StartMenuOptions(logicFlags,renderFlags);
	}
	else if (action == Action::quitGame)
	{
		QuitGame(logicFlags,renderFlags);
	}
	else if (action == Action::startMapEditor)
	{
		if (!StartMapEditor(logicFlags,renderFlags,Map))
			return	false;
	}
	else if (action == Action::returnFromOptions)
	{
		if (logicFlags.state == LGS_mainOptions)
		{
			StartMainMenu(logicFlags,renderFlags);
		}
		else if (logicFlags.state == LGS_options)
		{
			ResumeInGame(logicFlags,renderFlags);
		}
	}
	else if (action == Action::changeRes1024x768)
	{
		ChangeResolution(Message,1024,768);
	}
	else if (action == Action::changeRes640x480)
	{
		ChangeResolution(Message,640,480);
	}
	else if (action == Action::changeRes800x600)
	{
		ChangeResolution(Message,800,600);
	}
	else if (action == Action::checkFPSLimit)
	{
		ChangeFPSLock(Message);
	}
	else if (action == Action::checkGrabMode)
	{
		ChangeGrabMode(Message);
	}

	action = Action::nothing;

	return	true;
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

void	CAction::ChangeGrabMode(CMessage *Message)
{
	if (g_grabMode == true)
	{
		m_moveCamera = false;
		g_grabMode = false;
	}
	else
	{
		m_moveCamera = true;
		g_grabMode = true;
	}

	Message->m_renderMessage = RenderMessage::changeGrabMode;
	Message->m_messageToGUI	=	MessageToGUI::updateOptionsData;
	//SDL_SetWindowGrab(g_grabMode);
}

void	CAction::ChangeFPSLock(CMessage *Message)
{
	if (g_FPSLimit)
	{
		g_FPSLimit = false;
	}
	else
	{
		g_FPSLimit = true;
	}

	Message->m_messageToGUI	=	MessageToGUI::updateOptionsData;
}

void	CAction::ChangeResolution(CMessage *Message,int x,int y)
{
	if (x == g_windowX && y == g_windowY)
		return;

	g_windowX = x;
	g_windowY = y;
	WINDOW_WIDTH = x;
	WINDOW_HEIGHT = y;

	Message->m_renderMessage	=	RenderMessage::resizeWindow;
	Message->m_messageToGUI		=	MessageToGUI::updateResData;
}

bool	CAction::isCameraEnabled()
{
	return	m_moveCamera;
}

void	CAction::NextTextBox(_TextRenderInfo &textRenderInfo)
{
	textRenderInfo.nextTextBox = true;
}

void	CAction::TextQuestSelection(int mouseX,int mouseY,CQuestManager *Quest,CEntityManager *Entity,_TextRenderInfo &textRenderInfo)
{
	if (Entity->m_VNpc[textRenderInfo.selectedNPCIndex].ConversationSelection(textRenderInfo.selectedConversationIndex))
	{
		YesNoSelection(mouseX,mouseY,Quest,Entity,textRenderInfo);
	}
	else
	{
		NextTextBox(textRenderInfo);
	}
}

void	CAction::YesNoSelection(int mouseX,int mouseY,CQuestManager *Quest,CEntityManager *Entity,_TextRenderInfo &textRenderInfo)
{
	if (mouseY >= 260 && mouseY <= 280) // yes option
	{
		textRenderInfo.setTextBox =	true;
		textRenderInfo.chars		=	0;
		Entity->m_VNpc[textRenderInfo.selectedNPCIndex].SetConversationStateYes(textRenderInfo.selectedConversationIndex);
		Entity->m_VNpc[textRenderInfo.selectedNPCIndex].GetTextIndex(textRenderInfo.selectedTextIndex,textRenderInfo.selectedConversationIndex);

		// set the quest to active
		int questID = Entity->m_VNpc[textRenderInfo.selectedNPCIndex].GetQuestID(textRenderInfo.selectedConversationIndex);
		if (questID != -1)
		{
			Quest->FindIndex(questID);
			Quest->SetActiveQuest(true);
		}

	}
	else if (mouseY > 280 && mouseY <= 300) // no option
	{
		textRenderInfo.setTextBox =	true;
		textRenderInfo.chars		=	0;
		Entity->m_VNpc[textRenderInfo.selectedNPCIndex].SetConversationStateNo(textRenderInfo.selectedConversationIndex);
		Entity->m_VNpc[textRenderInfo.selectedNPCIndex].GetTextIndex(textRenderInfo.selectedTextIndex,textRenderInfo.selectedConversationIndex);
	}
}

void	CAction::ConversationSelection(int mouseX,int mouseY,_TextRenderInfo &textRenderInfo,_renderFlags &renderFlags,CEntityManager *Entity)
{
	int height = Entity->m_VNpc[textRenderInfo.selectedNPCIndex].m_NumConversations;

	if (CheckClick(mouseX,mouseY,118,240,404,height * 20))
	{
		if (Entity->m_VNpc[textRenderInfo.selectedNPCIndex].m_type == NPCType::quest)
		{
			renderFlags.textBoxState			=	RTBS_renderTextBox;
			textRenderInfo.setTextBox			=	true;

			for (int i = 0; i < height; i++)
			{
				if (mouseY >= 240+20*i && mouseY <= 240+20*(i+1))
				{
					// conversation index
					textRenderInfo.selectedConversationIndex = Entity->m_VNpc[textRenderInfo.selectedNPCIndex].GetConversationIndex(i);				
				}
			}

			// text index
			Entity->m_VNpc[textRenderInfo.selectedNPCIndex].GetTextIndex(textRenderInfo.selectedTextIndex,textRenderInfo.selectedConversationIndex);
		}
	}
}

void	CAction::SetTileSelected(int mouseX,int mouseY,CMap *Map)
{
	int k = mouseX / 32;
	int l = mouseY / 32;

	Map->m_selectedTile = Map->m_allTilesMap[l][k];
}

void	CAction::SetTileOnMap(int mouseX,int mouseY,CMap *Map)
{
	int k	=	(int)((mouseY + Map->m_cameraY-WINDOW_HEIGHT/2)/32);
	int l	=	(int)((mouseX + Map->m_cameraX-WINDOW_WIDTH/2)/32);

	Map->m_mapEditor[k][l] = Map->m_selectedTile;
}

void	CAction::InitAllTiles(CMap *Map)
{
	Map->m_cameraX = 320;
	Map->m_cameraY = 240;

	if (!Map->m_allTilesMapState)
	{
		Map->InitAllTilesMap();
	}
}

bool	CAction::StartGame(_logicFlags &logicFlags,_renderFlags &renderFlags,CMap *Map)
{
	ResumeInGame(logicFlags,renderFlags);

	if (!InitMap(Map))
		return	false;

	return	true;
}

void	CAction::ResumeInGame(_logicFlags &logicFlags,_renderFlags &renderFlags)
{
	logicFlags.state	=	LGS_inGame;
	renderFlags.state	=	RS_renderMap;
}

void	CAction::StartMainMenu(_logicFlags &logicFlags,_renderFlags &renderFlags)
{
	logicFlags.state	=	LGS_mainMenu;
	renderFlags.state	=	RS_renderMainMenu;
}

bool	CAction::StartMapEditor(_logicFlags &logicFlags,_renderFlags &renderFlags,CMap *Map)
{
	logicFlags.state	=	LGS_mapEditor;
	renderFlags.state	=	RS_renderMapEditor;

	if (!InitMapEditor(Map))
		return	false;
	return	true;
}

void	CAction::StartMenuOptions(_logicFlags &logicFlags,_renderFlags &renderFlags)
{
	logicFlags.state	=	LGS_mainOptions;
	renderFlags.state	=	RS_renderOptions;
}

void	CAction::QuitGame(_logicFlags &logicFlags,_renderFlags &renderFlags)
{
	logicFlags.state	=	LGS_exit;
	renderFlags.state	=	RS_renderGameExit;
}

bool	CAction::LoadGame(_logicFlags &logicFlags,_renderFlags &renderFlags,CMap *Map)
{
	return	true;
}

bool	CAction::CheckClick(int x1,int y1,int x2,int y2,int w2,int h2)
{
	if (x1 > x2 && x1 < x2 + w2 && y1 > y2 && y1 < y2 + h2)
		return	true;

	return	false;
}
