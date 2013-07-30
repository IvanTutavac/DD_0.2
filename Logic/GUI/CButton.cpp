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

#include "CButton.h"
#include <string>

CButton::CButton(char *name,int x_,int y_,int w_,int h_)
{
	m_x = x_, m_y = y_, m_w = w_, m_h = h_;

	strcpy_s(m_name,name);

	m_onReleaseLeft		=	Action::nothing;
	m_onPressLeft		=	Action::nothing;
	m_onReleaseLeft		=	Action::nothing;
	m_onPressRight		=	Action::nothing;
	m_action			=	Action::nothing;
}



void	CButton::SetOnPressLeft(Action action)
{
	m_onPressLeft = action;
}

void	CButton::SetOnReleaseLeft(Action action)
{
	m_onReleaseLeft = action;
}

void	CButton::SetOnPressRight(Action action)
{
	m_onPressRight = action;
}

void	CButton::SetOnReleaseRight(Action action)
{
	m_onReleaseRight = action;
}

void	CButton::SetMouse(Action leftClick,Action leftRelease,Action rightClick,Action rightRelease)
{
	m_onPressLeft		=	leftClick;
	m_onPressRight		=	rightClick;
	m_onReleaseLeft		=	leftRelease;
	m_onReleaseRight	=	rightRelease;
}

bool	CButton::CheckIfClicked(int mouseX,int mouseY,ClickType clickType)
{
	if (CheckClick(mouseX,mouseY,m_x,m_y,m_w,m_h))
	{
		if (clickType == ClickType::pressedLeft)
		{
			m_action = m_onPressLeft;
			
			return true;
		}
		else if (clickType == ClickType::releasedLeft)
		{
			m_action = m_onReleaseLeft;
			
			return true;
		}
		else if (clickType == ClickType::pressedRight)
		{
			m_action = m_onPressRight;
			
			return true;
		}
		else if (clickType == ClickType::releasedRight)
		{
			m_action = m_onReleaseRight;
			
			return	true;
		}
	}

	return	false;
}

bool	CButton::CheckClick(int x1,int y1,int x2,int y2,int w2,int h2)
{
	if (x1 > x2 && x1 < x2 + w2 && y1 > y2 && y1 < y2 + h2)
		return	true;

	return	false;
}

int		CButton::GetX()
{
	return	m_x;
}

int		CButton::GetY()
{
	return	m_y;
}

int		CButton::GetH()
{
	return	m_h;
}

int		CButton::GetW()
{
	return	m_w;
}

char*	CButton::GetName()
{
	return	m_name;
}

void	CButton::SetX(int x_)
{
	m_x = x_;
}

void	CButton::SetY(int y_)
{
	m_y = y_;
}
