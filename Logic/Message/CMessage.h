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

enum class MouseMessage
{
	nothing,
	initMap,
	initMapEditor,
	changeGrabMode,
	changeFPSLock,
	changeResolution640x480,
	changeResolution800x600,
	changeResolution1024x768,
	textQuestSelection,
	textCommonSelection,
	conversationSelection,
	setTileSelected,
	setTileOnMap, 
	initAllTiles,
	saveMapEditorMap
};

enum class RenderMessage
{
	nothing,
	resizeWindow,
	changeGrabMode
};

enum class MessageToGUI
{
	nothing,
	updateOptionsData,
	updateResData
};

class CMessage
{

private:

public:

	MouseMessage	m_mouseMessage;
	RenderMessage	m_renderMessage;
	MessageToGUI	m_messageToGUI;
	
	int	x,y;

	bool	Init();
	void	Clean();

};