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

enum class ClickType
{
	pressedLeft,
	releasedLeft,
	pressedRight,
	releasedRight
};

enum class Action
{
	nothing,
	startGame,
	loadGame,
	quitGame,
	options,
	startMapEditor
};

enum class WindowType
{
	mainMenu,
	optionsInGame,
	options,
	loadGame,
	inGame
};
