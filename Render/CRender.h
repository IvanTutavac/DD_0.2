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
class CMessage;

class CRender
{

private:

protected:

	std::string		m_text;

	// will only return true if the whole entity is in the window
	// x and y will be updated for window drawing, so use tempX and tempY for them ( mapX != windowX)
	bool			CheckIfInWindow(int cameraX,int cameraY,int &x,int &y);
	// allows entities not to be fully drawn ( if a half is visible, that half will be drawn...)
	// x and y will be updated for window drawing, so use tempX and tempY for them ( mapX != windowX)
	// tempX and tempY used for input must no be transformed into Window locations, the method already does it
	bool			CheckIfInWindowEx(int cameraX,int cameraY,int &x,int &y);

	virtual void	VRenderFPS(int fps)=0;
	virtual void	VRenderImage(int x,int y,const int type,int i)=0;
	// image will be drawn on main window
	virtual void	VRenderImage(int x,int y,int w,int h,const int type,int i)=0; 
	//virtual void	VRenderImage(int x,int y,int cutX,int cutY,const int type, int i)=0;

public:

	virtual	bool	VInit()=0;
	virtual void	VClean()=0;

	virtual	void	VClearWindow()=0;
	virtual void	VClearHUD()=0;

	virtual void	VUpdateWindow()=0;

	virtual void	VRenderOptions()=0;
	virtual void	VRenderText(char *text,int x,int y,int r,int  g,int b)=0;
	virtual void	VRenderValue(int value,int x,int y,int r,int g,int b)=0;
	virtual void	VRenderButton(char *text,int x,int y,int r,int g,int b)=0;
	virtual bool	VRenderTextBox(unsigned int &words,bool &next,bool first)=0;
	virtual void	VReadMessage(CMessage *Message)=0;

	void	RenderMap(CMap *MapPointer);
	void	RenderMapEditor(CMap *MapPointer);
	void	RenderAllTiles(CMap *MapPointer);
	void    RenderMainMenu();
	void	SetTextBox(std::string text);
	void	RenderHUD(int fps,int hp,int mp);

	virtual	~CRender(){}

};