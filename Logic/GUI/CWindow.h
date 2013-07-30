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
#include "CTextBox.h"
#include <vector>

class CGUIRenderData;

class CWindow
{
private:

	WindowType	m_type;

	std::vector<CButton> m_button;
	std::vector<CTextBox> m_textBox;

	bool	InitOptionsWindow();
	void	UpdateOptionsData();
	void	UpdateOptionsResData();
	void	UpdateMenuResData();
	bool	InitMainMenu();

	void	SetRenderTextData(bool button);
	void	SetRenderStringData(bool textBox);

public:

	CGUIRenderData *m_renderData;
	Action	m_action; // check click sets it

	void	UpdateData(WindowType type);
	void	UpdateResData();
	bool	Init(WindowType type);
	void	Clean();

	bool	CheckClick(int mouseX,int mouseY,ClickType clickType);
};