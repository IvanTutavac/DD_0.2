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

#include "CGUI.h"
#include "CWindow.h"
#include "..\..\debug.h"

bool	CGUI::Init()
{
	m_pMainMenu	=	DD_NEW CWindow;
	m_pOptions	=	DD_NEW CWindow;
	m_pInGame	=	DD_NEW CWindow;

	if (!m_pMainMenu->Init(WindowType::mainMenu))
		return	false;

	if (!m_pOptions->Init(WindowType::options))
		return	false;

	if (!m_pInGame->Init(WindowType::inGame))
		return	false;

	return	true;
}

void	CGUI::Clean()
{
	m_pMainMenu->Clean();
	m_pOptions->Clean();
	m_pInGame->Clean();
	
	DD_DELETE(m_pMainMenu);
	DD_DELETE(m_pOptions);
	DD_DELETE(m_pInGame);
}

void	CGUI::UpdateResData()
{
	m_pMainMenu->UpdateResData();
	m_pOptions->UpdateResData();
}

