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

#pragma once

const	int DATA_I = 32;

struct _valueData
{
	int value;
	int	x,y,w,h;
};

struct _textData
{
	char *text;
	int	x,y,w,h;
};

struct _boxData
{
	int	img;
	int	x,y,h,w;
};

// render reads this class 
// CGUI fills it with data
class CGUIRenderData
{
private:

public:

	_valueData	m_valueData[DATA_I];
	_textData	m_textData[DATA_I];
	_boxData	m_boxData[DATA_I];

	// upper limit
	// for (int i = 0; i < m_valueDataLimit)
	//	m_valueData[i].
	int			m_valueDataLimit;
	int			m_textDataLimit;
	int			m_boxDataLimit;
};
