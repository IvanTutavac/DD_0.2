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
#include "..\..\configuration.h"
#include "..\..\debug.h"

bool	CWindow::Init(WindowType type)
{
	m_type	=	type;

	m_renderData	=	DD_NEW CGUIRenderData; 

	if (type == WindowType::options)
	{
		InitOptionsWindow();
		SetRenderTextData(true);
		//SetRenderStringData(true); // UpdateData will do this (InitOptionsWindow calls it)...
	}
	else if (type == WindowType::mainMenu)
	{
		InitMainMenu();
		SetRenderTextData(true);
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
	CTextBox	grabMode("",230,57,32,32);
	CTextBox	fpsLimit("",230,121,32,32);

	CButton		grab("Grab mode:",32,32,192,64);
	grab.SetOnReleaseLeft(Action::checkGrabMode);

	CButton		FPS("Limit FPS:",32,96,192,64);
	FPS.SetOnReleaseLeft(Action::checkFPSLimit);

	CButton		Res1("640 x 480",32,160,192,64);
	Res1.SetOnReleaseLeft(Action::changeRes640x480);

	CButton		Res2("800 x 600",32,224,192,64);
	Res2.SetOnReleaseLeft(Action::changeRes800x600);

	CButton		Res3("1024 x 768",32,288,192,64);
	Res3.SetOnReleaseLeft(Action::changeRes1024x768);

	CButton		Return("Return",128,WINDOW_HEIGHT,192,64);
	Return.SetOnReleaseLeft(Action::returnFromOptions);

	CButton		Exit("Exit",320,WINDOW_HEIGHT,192,64);
	Exit.SetOnReleaseLeft(Action::quitGame);

	m_button.push_back(grab);
	m_button.push_back(FPS);
	m_button.push_back(Res1);
	m_button.push_back(Res2);
	m_button.push_back(Res3);
	m_button.push_back(Return);
	m_button.push_back(Exit);

	m_textBox.push_back(grabMode);
	m_textBox.push_back(fpsLimit);

	UpdateOptionsData();
	SetRenderTextData(true);

	return	true;
}

void	CWindow::UpdateOptionsData()
{
	if (g_grabMode == true)
	{
		m_textBox[0].SetText("Yes");
		m_renderData->m_stringData[0].text = m_textBox[0].GetText();
	}
	else
	{
		m_textBox[0].SetText("No");
		m_renderData->m_stringData[0].text = m_textBox[0].GetText();
	}

	if (g_FPSLimit == true)
	{
		m_textBox[1].SetText("Yes");
		m_renderData->m_stringData[1].text = m_textBox[1].GetText();
	}
	else
	{
		m_textBox[1].SetText("No");
		m_renderData->m_stringData[1].text = m_textBox[1].GetText();
	}
}

void	CWindow::UpdateData(WindowType type)
{
	if (type == WindowType::options && type == m_type)
	{	
		UpdateOptionsData();
	}
}

void	CWindow::UpdateResData()
{
	if (WindowType::options == m_type)
	{	
		UpdateOptionsResData();
	}
	else if (WindowType::mainMenu == m_type)
	{
		UpdateMenuResData();
	}
}
void	CWindow::UpdateMenuResData()
{
	m_button[0].SetX(WINDOW_WIDTH/2-96);
	m_button[0].SetY(WINDOW_HEIGHT-144-64*3);
	m_renderData->m_textData[0].x = m_button[0].GetX();
	m_renderData->m_textData[0].y = m_button[0].GetY();

	m_button[1].SetX(WINDOW_WIDTH/2-96);
	m_button[1].SetY(WINDOW_HEIGHT-144-64*2);
	m_renderData->m_textData[1].x = m_button[1].GetX();
	m_renderData->m_textData[1].y = m_button[1].GetY();

	m_button[1].SetX(WINDOW_WIDTH/2-96);
	m_button[1].SetY(WINDOW_HEIGHT-144-64);
	m_renderData->m_textData[1].x = m_button[1].GetX();
	m_renderData->m_textData[1].y = m_button[1].GetY();

	m_button[1].SetX(WINDOW_WIDTH/2-96);
	m_button[1].SetY(WINDOW_HEIGHT-144);
	m_renderData->m_textData[1].x = m_button[1].GetX();
	m_renderData->m_textData[1].y = m_button[1].GetY();
}

void	CWindow::UpdateOptionsResData()
{
	for (size_t i = 0; i < m_button.size(); i++)
	{
		if (!strcmp(m_button[i].GetName(),"Return") || !(strcmp(m_button[i].GetName(),"Exit")))
		{
			m_button[i].SetY(WINDOW_HEIGHT);
			m_renderData->m_textData[i].x = m_button[i].GetY();
		}
	}
}

void	CWindow::SetRenderTextData(bool button)
{
	m_renderData->m_textDataLimit = 0;

	if (button)
	{
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
}

void	CWindow::SetRenderStringData(bool textBox)
{
	m_renderData->m_stringDataLimit  = 0;

	if (textBox)
	{
		for (size_t i = 0; i < m_textBox.size(); i++)
		{
			m_renderData->m_stringData[i].x		=	m_textBox[i].GetX();
			m_renderData->m_stringData[i].y		=	m_textBox[i].GetY();
			m_renderData->m_stringData[i].w		=	m_textBox[i].GetW();
			m_renderData->m_stringData[i].h		=	m_textBox[i].GetH();
			m_renderData->m_stringData[i].text	=	m_textBox[i].GetText();
			
			m_renderData->m_stringDataLimit++;
		}
	}
}

bool	CWindow::CheckClick(int mouseX,int mouseY,ClickType clickType)
{
	for (size_t i = 0; i < m_button.size(); i++)
	{
		if (m_button[i].CheckIfClicked(mouseX,mouseY,clickType))
		{
			m_action = m_button[i].m_action;
			return true;
		}
	}

	// for (int i = 0; i < .... next widgets...
	return	false;
}
