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

#include "CWindow.h"
#include "CGUIRenderData.h"
#include "..\..\const.h"
#include "..\..\debug.h"

bool	CWindow::Init(WindowType type)
{
	m_renderData	=	DD_NEW CGUIRenderData; 

	if (type == WindowType::options)
	{
		InitOptionsWindow();
	}
	else if (type == WindowType::mainMenu)
	{
		InitMainMenu();
		SetRenderButtonData();
	}

	return	true;
}

void	CWindow::Clean()
{
	DD_DELETE(m_renderData);
}

bool	CWindow::InitMainMenu()
{
	CButton	newGame("New game",WINDOW_WIDTH/2-96,WINDOW_HEIGHT-144-64*3,192,64);
	newGame.SetOnReleaseLeft(Action::startGame);
	
	CButton	mapEditor("Map editor",WINDOW_WIDTH/2-96,WINDOW_HEIGHT-144-64*2,192,64);
	mapEditor.SetOnReleaseLeft(Action::startMapEditor);

	CButton	options("Options",WINDOW_WIDTH/2-96,WINDOW_HEIGHT-144-64,192,64);
	options.SetOnReleaseLeft(Action::options);

	CButton	exit("Exit",WINDOW_WIDTH/2-96,WINDOW_HEIGHT-144,192,64);
	exit.SetOnReleaseLeft(Action::quitGame);

	m_button.push_back(newGame);
	m_button.push_back(mapEditor);
	m_button.push_back(options);
	m_button.push_back(exit);

	return	true;
}

bool	CWindow::InitOptionsWindow()
{
	return	true;
}

void	CWindow::SetRenderButtonData()
{
	m_renderData->m_textDataLimit = 0;

	for (size_t i = 0; i < m_button.size(); i++)
	{
		m_renderData->m_textData[i].x		=	m_button[i].GetX();
		m_renderData->m_textData[i].y		=	m_button[i].GetY();
		m_renderData->m_textData[i].w		=	m_button[i].GetW();
		m_renderData->m_textData[i].h		=	m_button[i].GetH();
		m_renderData->m_textData[i].text	=	m_button[i].GetName();

		m_renderData->m_textDataLimit++;
	}
}

void	CWindow::CheckClick(int mouseX,int mouseY,ClickType clickType)
{
	for (size_t i = 0; i < m_button.size(); i++)
	{
		if (m_button[i].CheckIfClicked(mouseX,mouseY,clickType))
		{
			m_action = m_button[i].m_action;
			return;
		}
	}

	// for (int i = 0; i < .... next widgets...
}
