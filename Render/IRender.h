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

class IRender
{

private:

public:

	virtual	bool	Init()=0;
	virtual void	Clean()=0;

	virtual	void	ClearWindow()=0;
	virtual void	ClearHUD()=0;

	virtual void	UpdateWindow()=0;

	virtual void	RenderMap(CMap *MapPointer)=0;
	virtual void	RenderMapEditor(CMap *MapPointer)=0;
	virtual void	RenderAllTiles(CMap *MapPointer)=0;
	virtual void    RenderMainMenu()=0;
	virtual void	RenderOptions()=0;
	virtual void	RenderText(char *text,int x,int y,int r,int  g,int b)=0;
	virtual void	RenderValue(int value,int x,int y,int r,int g,int b)=0;
	virtual void	RenderButton(char *text,int x,int y,int r,int g,int b)=0;
	virtual void	RenderTextBox(int &words,bool &next)=0;
	virtual	void	SetTextBox(std::string text)=0;
	virtual void	RenderHUD(int fps,int hp,int mp)=0;

	virtual	~IRender(){}

};