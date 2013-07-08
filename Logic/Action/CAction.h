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

class CMouseMessage;
class CMap;
class CQuestManager;
class CEntityManager;

class CAction
{

private:

	bool			m_moveCamera;

	bool			InitMapEditor(CMap *Map);
	bool			InitMap(CMap *Map);

	bool			CheckClick(int x1,int y1,int x2,int y2,int w2,int h2);

	// ReadMouseMessage calls these
	void			ChangeFPSLock();
	void			ChangeGrabMode();
	void			ChangeResolution(int x,int y);
	void			TextQuestSelection(int mouseX,int mouseY,CQuestManager *Quest,CEntityManager *Entity,_TextRenderInfo &textRenderInfo);
	void			YesNoSelection(int mouseX,int mouseY,CQuestManager *Quest,CEntityManager *Entity,_TextRenderInfo &textRenderInfo);
	void			NextTextBox(_TextRenderInfo &textRenderInfo);
	void			ConversationSelection(int mouseX,int mouseY,_TextRenderInfo &textRenderInfo,_renderFlags &renderFlags,CEntityManager *Entity);
	void			SetTileSelected(int mouseX,int mouseY,CMap *Map);
	void			SetTileOnMap(int mouseX,int mouseY,CMap *Map);
	void			InitAllTiles(CMap *Map);

public:

	bool	Init();
	void	Clean();

	bool	ReadMouseMessage(CMouseMessage *Message,CMap *Map,CQuestManager *Quest,CEntityManager *Entity,_TextRenderInfo &textRenderInfo,_renderFlags &renderFlags);

	// grab mode affects camera
	bool	isCameraEnabled();
};