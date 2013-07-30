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

#include "CTextBox.h"

CTextBox::CTextBox(std::string text,int x,int y,int w,int h)
{
	m_text	=	text;
	m_x = x, m_y = y, m_w = w, m_h = h;
}

void	CTextBox::SetText(std::string text)
{
	m_text = text;
}

std::string* CTextBox::GetText()
{
	return	&m_text;
}

int		CTextBox::GetX()
{
	return	m_x;
}

int		CTextBox::GetY()
{
	return	m_y;
}

int		CTextBox::GetW()
{
	return	m_w;
}

int		CTextBox::GetH()
{
	return	m_h;
}
