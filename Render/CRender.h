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
#include <string>

class CMap;

class CRender
{

private:

protected:

	std::string		m_text;

	bool	CRender::CheckIfInWindow(int cameraX,int cameraY,int &x,int &y);


	virtual void	RenderFPS(int fps)=0;
	// image will be drawn on main window
	virtual void	RenderImage(int x,int y,const int type,int i)=0; 

public:

	virtual	bool	Init()=0;
	virtual void	Clean()=0;

	virtual	void	ClearWindow()=0;
	virtual void	ClearHUD()=0;

	virtual void	UpdateWindow()=0;

	virtual void	RenderOptions()=0;
	virtual void	RenderText(char *text,int x,int y,int r,int  g,int b)=0;
	virtual void	RenderValue(int value,int x,int y,int r,int g,int b)=0;
	virtual void	RenderButton(char *text,int x,int y,int r,int g,int b)=0;
	virtual bool	RenderTextBox(unsigned int &words,bool &next,bool first)=0;

	void	RenderMap(CMap *MapPointer);
	void	RenderMapEditor(CMap *MapPointer);
	void	RenderAllTiles(CMap *MapPointer);
	void    RenderMainMenu();
	void	SetTextBox(std::string text);
	void	RenderHUD(int fps,int hp,int mp);

	virtual	~CRender(){}

};