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

#include "GUIStructs.h"

class CButton
{

private:

	int		m_x,m_y,m_w,m_h;

	Action	m_onReleaseLeft;
	Action  m_onPressLeft;
	Action	m_onReleaseRight;
	Action	m_onPressRight;

	bool	CheckClick(int x1,int y1,int x2,int y2,int w2,int h2);

public:

	CButton();

	Action	m_action;

	// if the widget was clicked the m_action will be set 
	void	CheckIfClicked(int mouseX,int mouseY,ClickType clickType);
	
	void	SetMouse(Action lefClick,Action leftRelease,Action rightClick, Action rightRelease);
	void	SetOnPressLeft(Action action);
	void	SetOnReleaseLeft(Action action);
	void	SetOnPressRight(Action action);
	void	SetOnReleaseRight(Action action);
};