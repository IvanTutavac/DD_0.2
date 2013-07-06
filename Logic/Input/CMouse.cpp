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

#include "CMouse.h"
#include "..\Message\CMouseMessage.h"
#include "..\..\debug.h"
#include "..\..\const.h"

bool	CMouse::Init()
{
	m_pMessage = DD_NEW CMouseMessage;

	if (!m_pMessage->Init())
		return	false;

	return	true;
}

void	CMouse::Clean()
{
	m_pMessage->Clean();
	DD_DELETE(m_pMessage);
}

void	CMouse::MenuCLick(int mouseX,int mouseY,_logicFlags &logicFlags,_renderFlags &renderFlags)
{
	if (CheckClick(mouseX,mouseY,224,144,192,64)) // new game button
	{
		logicFlags.state		=	LGS_inGame;
		renderFlags.state		=	RS_renderMap;
		m_pMessage->m_message	=	MouseMessage::initMap;
	}
	else if (CheckClick(mouseX,mouseY,224,208,192,64)) // map editor button 
	{
		logicFlags.state		=	LGS_mapEditor;
		renderFlags.state		=	RS_renderMapEditor;
		m_pMessage->m_message	=	MouseMessage::initMapEditor;
	}
	else if (CheckClick(mouseX,mouseY,224,272,192,64)) // options button 
	{
		logicFlags.state		=	LGS_mainOptions;
		renderFlags.state		=	RS_renderOptions;
	}
	else if (CheckClick(mouseX,mouseY,224,336,192,64)) // exit button 
	{
		logicFlags.state		=	LGS_exit;
		renderFlags.state		=	RS_renderGameExit;
	}
}

void	CMouse::OptionsClick(int mouseX,int mouseY,_logicFlags &logicFlags,_renderFlags &renderFlags)
{
	if (CheckClick(mouseX,mouseY,128,480,192,64)) // return button 
	{
		if (logicFlags.state == LGS_mainOptions)
		{
			logicFlags.state		=	LGS_mainMenu;
			renderFlags.state		=	RS_renderMainMenu;
		}
		else if (logicFlags.state == LGS_options)
		{
			logicFlags.state		=	LGS_inGame;
			renderFlags.state		=	RS_renderMap;
		}
	}
	else if (CheckClick(mouseX,mouseY,320,480,192,64))  // exit button 
	{
		logicFlags.state	=	LGS_exit;
		renderFlags.state	=	RS_renderGameExit;
	}
	else if (CheckClick(mouseX,mouseY,32,32,192,64))
	{
		m_pMessage->m_message = MouseMessage::changeGrabMode;
	}
	else if (CheckClick(mouseX,mouseY,96,96,192,64))
	{
		m_pMessage->m_message = MouseMessage::changeFPSLock;
	}
}

void	CMouse::InGameClick(int mouseX,int mouseY,_logicFlags &logicFlags,_renderFlags &renderFlags)
{
	if (logicFlags.npcConversation != NPCC_nothing)
	{
		TextBoxClick(mouseX,mouseY,logicFlags,renderFlags);
	}
}

void	CMouse::TextBoxClick(int mouseX,int mouseY,_logicFlags &logicFlags,_renderFlags &renderFlags)
{
	if (renderFlags.textBoxState == RTBS_renderTextBox)
	{
		TextSelectionClick(mouseX,mouseY,logicFlags,renderFlags);
	}
	else if (renderFlags.textBoxState == RTBS_renderFirstTextBox)
	{
		ConversationSelectionClick(mouseX,mouseY,logicFlags,renderFlags);
	}
}

void	CMouse::TextSelectionClick(int mouseX,int mouseY,_logicFlags &logicFlags,_renderFlags &renderFlags)
{
	if (CheckClick(mouseX,mouseY,118,238,404,84))
	{
		if (logicFlags.npcConversation == NPCC_questTalk)
		{
			m_pMessage->m_message = MouseMessage::textQuestSelection;
			m_pMessage->x = mouseX;
			m_pMessage->y = mouseY;
		}
		else if (logicFlags.npcConversation == NPCC_commonTalk)
		{
			m_pMessage->m_message = MouseMessage::textCommonSelection;
		}
	}
}

void	CMouse::ConversationSelectionClick(int mouseX,int mouseY,_logicFlags &logicFlags,_renderFlags &renderFlags)
{
	m_pMessage->m_message = MouseMessage::conversationSelection;
	m_pMessage->x = mouseX;
	m_pMessage->y = mouseY;
}

void	CMouse::MapEditorAllTilesClick(int mouseX,int mouseY,_logicFlags &logicFlags,_renderFlags &renderFlags)
{
	if (CheckClick(mouseX,mouseY,20,WINDOW_HEIGHT,192,64)) // return button
	{
		logicFlags.state = LGS_mapEditor;
		renderFlags.state = RS_renderMapEditor;
	}
	else if (CheckClick(mouseX,mouseY,0,0,WINDOW_WIDTH,WINDOW_HEIGHT)) // tile clicked on window
	{
		m_pMessage->m_message = MouseMessage::setTileSelected;
		m_pMessage->x = mouseX;
		m_pMessage->y = mouseY;
	}
}

void	CMouse::MapEditorClickPress(int mouseX,int mouseY)
{
	// if it's pressed but not released we check mouseX and mouseY from motion event, because pressed event has x and y of where the click happened
	// so, while we're holding left mouse click pressed, set the map tile
	if (CheckClick(mouseX,mouseY,0,0,WINDOW_WIDTH,WINDOW_HEIGHT))
	{
		m_pMessage->m_message = MouseMessage::setTileOnMapExt;
		m_pMessage->x = mouseX;
		m_pMessage->y = mouseY;
	}
}

void	CMouse::MapEditorClickRelease(int mouseX,int mouseY,_logicFlags &logicFlags,_renderFlags &renderFlags)
{
	if (CheckClick(mouseX,mouseY,20,WINDOW_HEIGHT,192,64)) // return button
	{
		logicFlags.state		=	LGS_mainMenu;
		renderFlags.state		=	RS_renderMainMenu;
	}
	else if (CheckClick(mouseX,mouseY,210,WINDOW_HEIGHT,192,64)) // all tiles button
	{
		logicFlags.state		=	LGS_allTiles;
		renderFlags.state		=	RS_renderAllTiles;
		m_pMessage->m_message	=	MouseMessage::initAllTiles;
		
	}
	else if (CheckClick(mouseX,mouseY,400,WINDOW_HEIGHT,192,64)) // save button
	{
		m_pMessage->m_message	=	MouseMessage::saveMapEditorMap;
	}
	else if (CheckClick(mouseX,mouseY,0,0,WINDOW_WIDTH,WINDOW_HEIGHT)) // tile setting
	{
		m_pMessage->m_message	=	MouseMessage::setTileOnMap;
		m_pMessage->x = mouseX;
		m_pMessage->y = mouseY;
	}
}

bool	CMouse::CheckClick(int x1,int y1,int x2,int y2,int w2,int h2)
{
	if (x1 > x2 && x1 < x2 + w2 && y1 > y2 && y1 < y2 + h2)
		return	true;

	return	false;
}

