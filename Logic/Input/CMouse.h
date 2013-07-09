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

#include "..\dataStructs.h"

class CMessage;

/*
this class checks for mouse clicks, reads the game state and updates m_pMessage and logic data ( logicFlags, renderFlags...)
m_pMessage contains message for CAction which will deal with the message
*/
class CMouse
{

private:

	bool	CheckClick(int x1,int y1,int x2,int y2,int w2,int h2);
	void	TextBoxClick(int mouseX,int mouseY,_logicFlags &logicFlags,_renderFlags &renderFlags);
	void	TextSelectionClick(int mouseX,int mouseY,_logicFlags &logicFlags,_renderFlags &renderFlags);
	void	ConversationSelectionClick(int mouseX,int mouseY,_logicFlags &logicFlags,_renderFlags &renderFlags);

public:

	CMessage *m_pMessage;

	bool	Init();
	void	Clean();

	void	MenuCLick(int mouseX,int mouseY,_logicFlags &logicFlags,_renderFlags &renderFlags);
	void	OptionsClick(int mouseX,int mouseY,_logicFlags &logicFlags,_renderFlags &renderFlags);
	void	InGameClick(int mouseX,int mouseY,_logicFlags &logicFlags,_renderFlags &renderFlags);
	void	MapEditorClickRelease(int mouseX,int mouseY,_logicFlags &logicFlags,_renderFlags &renderFlags);
	// for mouseX and mouseY use MotionEvent.x and MotionEvent.y
	void	MapEditorClickPress(int mouseX,int mouseY);
	void	MapEditorAllTilesClick(int mouseX,int mouseY,_logicFlags &logicFlags,_renderFlags &renderFlags);
};