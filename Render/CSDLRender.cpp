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

#include "..\debug.h"
#include "..\Log.h"
#include "CSDLRender.h"
#include "CImageSurface.h"
#include "..\Logic\CMap.h"
#include "..\const.h"
#include "..\configuration.h"

//SDL_GrabMode g_grabMode;

// interface methods

bool	CSDLRender::Init()
{
	m_pWindow	=	NULL;

	if ((m_pWindow = SDL_SetVideoMode(WINDOW_WIDTH,WINDOW_HEIGHT+HUD_HEIGHT,32,SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_HWACCEL)) == NULL)
		return	false;

	Log("Window...");

	if (TTF_Init() == -1) // font init
		return	false;
	Log("TTF_Init ...");

	if (g_grabMode)
		SDL_WM_GrabInput(SDL_GRAB_ON);
	else
		SDL_WM_GrabInput(SDL_GRAB_OFF);

	if (!LoadClasses())
		return	false;
	Log("Images loaded");

	if (!LoadFonts())
		return	false;
	Log("Font loaded");

	m_clearHUDRect.x = 0, m_clearHUDRect.y = WINDOW_HEIGHT,m_clearHUDRect.w = WINDOW_WIDTH,m_clearHUDRect.h = HUD_HEIGHT;
	m_clearTextBoxRect.x = 118,m_clearTextBoxRect.y = 238,m_clearTextBoxRect.w = 404,m_clearTextBoxRect.h = 84;

	return	true;
}

void	CSDLRender::Clean()
{
	m_pTileSurface->DeleteAll();
	m_pEnemySurface->DeleteAll();
	m_pPlayerSurface->DeleteAll();
	m_pMenuSurface->DeleteAll();
	m_pNPCSurface->DeleteAll();
	m_pSpellSurface->DeleteAll();

	DD_DELETE(m_pTileSurface);
	DD_DELETE(m_pEnemySurface);
	DD_DELETE(m_pPlayerSurface);
	DD_DELETE(m_pMenuSurface);
	DD_DELETE(m_pNPCSurface);
	DD_DELETE(m_pSpellSurface);
	
	TTF_CloseFont(m_pHUDFont);

	TTF_Quit();
}

void	CSDLRender::ClearWindow()
{
	RenderImage(0,0,m_pMenuSurface->m_imageSurface[0],m_pWindow);
}

void	CSDLRender::ClearHUD()
{
	SDL_FillRect(m_pWindow,&m_clearHUDRect,SDL_MapRGB(m_pWindow->format,0,0,0));
	//RenderImage(0,480,m_pMenuSurface->m_imageSurface[2],m_pWindow);
}

void	CSDLRender::UpdateWindow()
{
	SDL_Flip(m_pWindow);
}

void	CSDLRender::RenderAllTiles(CMap *MapPointer)
{
	int	cameraX		=	MapPointer->m_cameraX/32;
	int	cameraY		=	MapPointer->m_cameraY/32;

	int	k =	0, l = 0;

	for (int i = -7; i < 8; i++)
	{
		for (int j = -10; j < 10; j++)
		{
			k = j, l = i;
			k += 10, l += 7;
			RenderImage(k*32,l*32,m_pTileSurface->m_imageSurface[MapPointer->m_allTilesMap[cameraY+i][cameraX+j]],m_pWindow);
		}
	}

	RenderButton("Return",20,WINDOW_HEIGHT,98,0,49);
	RenderImage(230,WINDOW_HEIGHT,m_pTileSurface->m_imageSurface[MapPointer->m_selectedTile],m_pWindow);
}

void	CSDLRender::RenderMapEditor(CMap *MapPointer)
{
	int cameraX = MapPointer->m_cameraX/32;
	int cameraY = MapPointer->m_cameraY/32;

	int k = 0, l = 0;

	for (int i = -7; i < 8; i++)
	{
		for (int j = -10; j < 10; j++)
		{
			k = j, l = i;
			k += 10, l+=7;
			RenderImage(k*32,l*32,m_pTileSurface->m_imageSurface[MapPointer->m_mapEditor[cameraY+i][cameraX+j]],m_pWindow);
			k = 0, l = 0;
		}
	}

	RenderButton("Return",20,WINDOW_HEIGHT,98,0,49);
	RenderButton("All tiles",210,WINDOW_HEIGHT,98,0,49);
	RenderButton("Save map",400,WINDOW_HEIGHT,98,0,49);
}

void	CSDLRender::RenderMap(CMap *MapPointer)
{
	int	cameraX = MapPointer->m_cameraX / 32; // window middle
	int	cameraY = MapPointer->m_cameraY / 32;

	int k = 0, l = 0; // where will the image be drawn if multiplied by 32
	// [cameraY + i][cameraX + j]  which tile needs to be drawn

	for (int i = -7; i < 8; i++)
	{
		for (int j = -10; j < 10; j++)
		{
			k = j, l = i;
			k+=10,l+=7; // we can not use negative numbers
			RenderImage(k*32,l*32,m_pTileSurface->m_imageSurface[MapPointer->m_map[cameraY+i][cameraX+j]],m_pWindow);
			k = 0, l = 0;
		}
	}

	// m_playerX and m_playerY are map locations, we need window locations
	int x = MapPointer->m_playerX - MapPointer->m_cameraX + WINDOW_WIDTH/2;
	int y = MapPointer->m_playerY - MapPointer->m_cameraY + WINDOW_HEIGHT/2;

	if (x >= 0 && y >= 0 && x <= WINDOW_WIDTH - TILE_SIZE && y <= WINDOW_HEIGHT - TILE_SIZE) // do not let player be rendered onto bottom screen
		RenderImage(x,y,m_pPlayerSurface->m_imageSurface[0],m_pWindow); // render player

	for (int i = 0; i < MapPointer->m_enemyXY.size(); i++)
	{
		x = MapPointer->m_enemyXY[i].x;
		y = MapPointer->m_enemyXY[i].y;
		if (CheckIfInWindow(MapPointer->m_cameraX,MapPointer->m_cameraY,x,y))
			RenderImage(x,y,m_pEnemySurface->m_imageSurface[i],m_pWindow);
	}

	for (int i = 0; i < MapPointer->m_npcXY.size(); i++)
	{
		x = MapPointer->m_npcXY[i].x;
		y = MapPointer->m_npcXY[i].y;
		if (CheckIfInWindow(MapPointer->m_cameraX,MapPointer->m_cameraY,x,y))
			RenderImage(x,y,m_pNPCSurface->m_imageSurface[i],m_pWindow);
	}

	for (int i = 0; i < MapPointer->m_spell.size(); i++)
	{
		x = MapPointer->m_spell[i].x;
		y = MapPointer->m_spell[i].y;
		if (CheckIfInWindow(MapPointer->m_cameraX,MapPointer->m_cameraY,x,y))
			RenderImage(x,y,m_pSpellSurface->m_imageSurface[MapPointer->m_spell[i].imgID],m_pWindow);
		/* With spells, you can not use ->m_imageSurface[i] for image that needs to be drawn, because there can be more than one same spell
		this must also be solved with enemies and npc because there can be same enemies on the map and i can not be used as an index ! */
	}
}

void	CSDLRender::RenderHUD(int fps,int hp,int mp)
{
	RenderFPS(fps);
	RenderText("HP:",20,490,98,0,49);
	RenderValue(hp,60,490,98,0,49);
	RenderText("MP:",20,520,98,0,49);
	RenderValue(mp,60,520,98,0,49);
}

void	CSDLRender::SetTextBox(std::string text)
{
	m_text = text;
}

void	CSDLRender::RenderTextBox(int &chars,bool &next)
{
	SDL_Color	fontColor;
	fontColor.r = 98, fontColor.g = 0,fontColor.b = 49;
	SDL_Surface *surface = NULL;

	bool	countChars = false;

	if (next)
	{
		if (chars > m_text.length())
			return;
		m_text = m_text.substr(chars);
		countChars = true;
	}

	if (chars == 0)
		countChars = true;

	char *tempText; 
	std::vector<char> tempText1(m_text.size()+1);

	std::copy(m_text.begin(),m_text.end(),tempText1.begin());
	tempText = &tempText1[0];
	//tempText = DD_NEW char[m_text.length()+1];
	//strcpy_s(tempText,sizeof(m_text),m_text.c_str());

	char *context = 0;
	char *temp = strtok_s(tempText," ",&context);
	int	x = 120, y = 240;

	SDL_FillRect(m_pWindow,&m_clearTextBoxRect,SDL_MapRGB(m_pWindow->format,255,255,255));

	while (true)
	{
		if (temp == NULL)
			break;

		if (countChars)
			chars++;

		surface	= TTF_RenderText_Solid(m_pHUDFont,temp,fontColor);

		if (x+surface->w >= 520 && y == 300)
			break;

		if (x + surface->w >= 520)
			x = 120,y+=20;

		RenderImage(x,y,surface,m_pWindow);

		x+=surface->w+8;

		temp = strtok_s(NULL," ",&context);

		SDL_FreeSurface(surface);
		surface = NULL;
	}

	next = false;
}

void	CSDLRender::RenderButton(char *text,int x,int y,int r,int g,int b)
{
	SDL_Color	fontColor;
	fontColor.r = r, fontColor.g = g, fontColor.b = b;

	SDL_Surface *surface = NULL;

	surface = TTF_RenderText_Solid(m_pHUDFont,text,fontColor);

	// the button needs to have the text centered
	int k = (x + m_pMenuSurface->m_imageSurface[1]->w / 2) - surface->w/2;
	int l = (y + m_pMenuSurface->m_imageSurface[1]->h / 2) - surface->h/2;

	RenderImage(x,y,m_pMenuSurface->m_imageSurface[1],m_pWindow); // render button
	RenderImage(k,l,surface,m_pWindow); // render text

	SDL_FreeSurface(surface);
	surface = NULL;
}

void	CSDLRender::RenderText(char *text,int x,int y,int r,int g,int b)
{
	SDL_Color	fontColor;
	fontColor.r =  r,fontColor.g = g,fontColor.b = b;

	SDL_Surface *tempSurface = NULL;

	tempSurface = TTF_RenderText_Solid(m_pHUDFont,text,fontColor);

	RenderImage(x,y,tempSurface,m_pWindow);

	SDL_FreeSurface(tempSurface);
	tempSurface = NULL;
}

void	CSDLRender::RenderValue(int value,int x,int y,int r,int g,int b)
{
	SDL_Color	fontColor;
	fontColor.b = b, fontColor.g = g, fontColor.r = r;

	char	text[25];

	_itoa_s(value,text,10);

	SDL_Surface *surface = NULL;

	surface = TTF_RenderText_Solid(m_pHUDFont,text,fontColor);

	RenderImage(x,y,surface,m_pWindow);

	SDL_FreeSurface(surface);
	surface = NULL;
}

void	CSDLRender::RenderMainMenu()
{
	RenderButton("New game",224,144,25,23,112);
	RenderButton("Map editor",224,208,25,25,112);
	RenderButton("Options",224,272,25,25,112);
	RenderButton("Exit",224,336,25,23,112);
}

void	CSDLRender::RenderOptions()
{
	RenderButton("Grab mode:",32,32,25,25,112);

	if (g_grabMode == SDL_GRAB_ON)
		RenderText("Yes",230,57,25,25,112);
	else
		RenderText("No",230,57,25,25,112);

	RenderButton("Limit FPS:",32,96,25,25,112);

	if (g_FPSLimit)
		RenderText("Yes",230,121,25,25,112);
	else
		RenderText("No",230,121,25,25,112);

	RenderButton("Return",128,480,25,25,112);
	RenderButton("Exit",320,480,25,25,112);
}

// rest

void	CSDLRender::RenderImage(int x,int y,SDL_Surface *image,SDL_Surface *surface)
{
	SDL_Rect	tempRect;
	tempRect.x = x;
	tempRect.y = y;

	SDL_BlitSurface(image,0,surface,&tempRect);
}

bool	CSDLRender::CheckIfInWindow(int cameraX,int cameraY,int &x,int &y)
{
	if (x < (cameraX - WINDOW_WIDTH/2) || x > (cameraX + WINDOW_WIDTH/2) || y < (cameraY - WINDOW_HEIGHT/2) || y > (cameraY + WINDOW_HEIGHT/2))
		return false; // outside
	// ( lets say, 1200X,800Y on map -> and our window has 640,480 )
	// MapX,MapY transform into X and Y for window 
	x-= (cameraX - WINDOW_WIDTH/2);
	y-= (cameraY - WINDOW_HEIGHT/2);
	return true;
}

bool	CSDLRender::LoadClasses()
{
	m_pTileSurface		=	DD_NEW	CTileImageSurface;
	m_pEnemySurface		=	DD_NEW	CEnemyImageSurface;
	m_pPlayerSurface	=	DD_NEW	CPlayerImageSurface;
	m_pMenuSurface		=	DD_NEW	CMenuImageSurface;
	m_pNPCSurface		=	DD_NEW	CNPCImageSurface;
	m_pSpellSurface		=	DD_NEW	CSpellImageSurface;

	if (m_pTileSurface->LoadAll() != 1) // tile 1 will return 0 if it failed to load
		return	false;
	Log("Tiles loaded");

	if (m_pEnemySurface->LoadAll() != 1)
		return false;
	Log("Enemy images loaded");

	if (m_pPlayerSurface->LoadAll() != 1)
		return false;
	Log("Player images loaded");

	if (m_pMenuSurface->LoadAll() != 1)
		return	false;
	Log("Menu images loaded");

	if (m_pNPCSurface->LoadAll() != 1)
		return	false;
	Log("NPC images loaded");

	if (m_pSpellSurface->LoadAll() != 1)
		return	false;
	Log("Spell images loaded");

	return	true;
}

bool	CSDLRender::LoadFonts()
{
	m_pHUDFont			=	TTF_OpenFont("LiberationMono-Regular.ttf",22);
	
	if (!m_pHUDFont)
		return	false;
	return	true;
}

void	CSDLRender::RenderFPS(int fps)
{
	SDL_Color	fontColor;
	fontColor.b = 49, fontColor.g = 0, fontColor.r = 98;

	char	text[25];

	_itoa_s(fps,text,10);

	SDL_Surface *tempSurface = NULL;

	tempSurface = TTF_RenderText_Solid(m_pHUDFont,text,fontColor);

	RenderImage(WINDOW_WIDTH-8-tempSurface->w,WINDOW_HEIGHT,tempSurface,m_pWindow);

	SDL_FreeSurface(tempSurface);
	tempSurface = NULL;
}
