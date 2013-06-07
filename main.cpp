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

#define _CRTDBG_MAP_ALLOC
#include "debug.h"
#include "SDL.h"
#include "CGame.h"
#include "Log.h"

/*
	DD_NEW insted of new !!!!!
	DD_DELETE instead of delete -> will delete if it was allocated and set it to NULL
	DD_DELETE_ARRAY for array
	#include "debug.h" for each new cpp file that uses them
*/

/*
	#include <Windows.h>
	MessageBox(NULL,"Fontovi fail","Error",MB_OK);
*/

int	main (int argc, char * argv[])
{
	int tmpDebugFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
    tmpDebugFlag |= _CRTDBG_CHECK_ALWAYS_DF;
    _CrtSetDbgFlag(tmpDebugFlag);

	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// everything starts from here !!!!!!!!!!!

	CGame	*Game	=	DD_NEW		CGame;

	CleanLogFile();
	Log("Started...");

	if (!Game->Init())
		return	1;
	Log("Game Init finished...");

	Game->Run();
	Log("Game Run finished...");

	Game->Clean();
	Log("Game Clean finshed...");

	DD_DELETE(Game);

	_CrtDumpMemoryLeaks();

	return	0;
}