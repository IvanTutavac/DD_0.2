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
#include "..\Logic\Message\CMessage.h"
//#include "..\Logic\CMap.h"
#include "..\const.h"
#include "..\configuration.h"

//SDL_GrabMode g_grabMode;

// interface methods

bool	CSDLRender::VInit()
{
	//SDL_putenv("SDL_VIDEO_CENTERED=1"); // any value will center the window

	m_pWindow	=	NULL;

	//if ((m_pWindow = SDL_SetVideoMode(WINDOW_WIDTH,WINDOW_HEIGHT+HUD_HEIGHT,32,SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_HWACCEL)) == NULL)
		//return	false;
	m_pWindow = SDL_CreateWindow(APP_NAME,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,WINDOW_WIDTH,WINDOW_HEIGHT+HUD_HEIGHT,SDL_WINDOW_SHOWN);

	if (m_pWindow == NULL)
		return	false;

	Log("Window...");

	m_pRenderer = SDL_CreateRenderer(m_pWindow,-1,SDL_RENDERER_ACCELERATED);

	if (TTF_Init() == -1) // font init
		return	false;
	Log("TTF_Init ...");

	//if (g_grabMode)
		//SDL_SetWindowGrab(m_pWindow,(SDL_bool)g_grabMode);
	//else
	SDL_SetWindowGrab(m_pWindow,(SDL_bool)g_grabMode);

	if (!LoadClasses())
		return	false;
	Log("Images loaded");

	if (!LoadFonts())
		return	false;
	Log("Font loaded");

	UpdateClearHUDRect();

	m_clearTextBoxRect.x = 118,m_clearTextBoxRect.y = 238,m_clearTextBoxRect.w = 404,m_clearTextBoxRect.h = 84;

	return	true;
}

void	CSDLRender::VClean()
{
	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);

	for (size_t i = 0; i < m_pVImage.size(); i++)
	{
		m_pVImage[i]->DeleteAll();
		DD_DELETE(m_pVImage[i]);
	}
	
	TTF_CloseFont(m_pHUDFont);

	TTF_Quit();
}

void	CSDLRender::UpdateClearHUDRect()
{
	m_clearHUDRect.x = 0, m_clearHUDRect.y = WINDOW_HEIGHT,m_clearHUDRect.w = WINDOW_WIDTH,m_clearHUDRect.h = HUD_HEIGHT;
}

void	CSDLRender::VReadMessage(CMessage *Message)
{
	if (Message->m_renderMessage == RenderMessage::changeGrabMode)
	{
		SDL_SetWindowGrab(m_pWindow,(SDL_bool)g_grabMode);
	}
	else if (Message->m_renderMessage == RenderMessage::resizeWindow)
	{
		UpdateClearHUDRect();
		SDL_SetWindowSize(m_pWindow,WINDOW_WIDTH,WINDOW_HEIGHT+HUD_HEIGHT);
		SDL_SetWindowPosition(m_pWindow,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED);
	}

	Message->m_renderMessage = RenderMessage::nothing;
}

void	CSDLRender::VClearWindow()
{
	SDL_RenderClear(m_pRenderer);
	//VRenderImage(0,0,WINDOW_WIDTH,WINDOW_HEIGHT,TYPE_MENU,0);
}

void	CSDLRender::VClearHUD()
{
	SDL_RenderFillRect(m_pRenderer,&m_clearHUDRect);
	//SDL_FillRect(m_pWindow,&m_clearHUDRect,SDL_MapRGB(m_pWindow->format,0,0,0));
	//RenderImage(0,480,m_pMenuSurface->m_imageSurface[2],m_pWindow);
}

void	CSDLRender::VUpdateWindow()
{
	SDL_RenderPresent(m_pRenderer);
	//SDL_Flip(m_pWindow);
}

void	CSDLRender::VRenderOptions()
{
	VRenderButton("Grab mode:",32,32,25,25,112);

	if (g_grabMode == true)
		VRenderText("Yes",230,57,25,25,112);
	else
		VRenderText("No",230,57,25,25,112);

	VRenderButton("Limit FPS:",32,96,25,25,112);

	if (g_FPSLimit)
		VRenderText("Yes",230,121,25,25,112);
	else
		VRenderText("No",230,121,25,25,112);

	VRenderButton("640 x 480",32,160,25,25,112);
	VRenderButton("800 x 600",32,224,25,25,112);
	VRenderButton("1024 x 768",32,288,25,25,112);

	VRenderButton("Return",128,WINDOW_HEIGHT,25,25,112);
	VRenderButton("Exit",320,WINDOW_HEIGHT,25,25,112);
}

void	CSDLRender::VRenderImage(int x,int y,int w,int h,const int type,int i)
{
	RenderImage(x,y,w,h,m_pVImage[type]->m_vTexture[i].texture);
	//RenderImage(x,y,m_VImage[type].m_imageSurface[i],m_pWindow);
}

void	CSDLRender::VRenderImage(int x,int y,const int type,int i)
{
	RenderImage(x,y,m_pVImage[type]->m_vTexture[i].w,m_pVImage[type]->m_vTexture[i].h,m_pVImage[type]->m_vTexture[i].texture);
}

/*void	CSDLRender::VRenderImage(int x,int y,int cutX,int cutY,const int type,int i)
{
	//RenderImage(x,y,cutX,cutY,m_VImage[type]->m_imageSurface[i],m_pWindow);
}*/

void	CSDLRender::VRenderText(char *text,int x,int y,int r,int g,int b)
{
	SDL_Color	fontColor;
	fontColor.r =  r,fontColor.g = g,fontColor.b = b;

	SDL_Surface *tempSurface = NULL;

	tempSurface = TTF_RenderText_Solid(m_pHUDFont,text,fontColor);

	RenderImage(x,y,tempSurface->w,tempSurface->h,tempSurface);//,m_pWindow);

	SDL_FreeSurface(tempSurface);
	tempSurface = NULL;
}

void	CSDLRender::VRenderFPS(int fps)
{
	SDL_Color	fontColor;
	fontColor.b = 49, fontColor.g = 0, fontColor.r = 98;

	char	text[25];

	_itoa_s(fps,text,10);

	SDL_Surface *tempSurface = NULL;

	tempSurface = TTF_RenderText_Solid(m_pHUDFont,text,fontColor);

	RenderImage(WINDOW_WIDTH-8-tempSurface->w,WINDOW_HEIGHT,tempSurface->w,tempSurface->h,tempSurface);//,m_pWindow);

	SDL_FreeSurface(tempSurface);
	tempSurface = NULL;
}

void	CSDLRender::VRenderValue(int value,int x,int y,int r,int g,int b)
{
	SDL_Color	fontColor;
	fontColor.b = b, fontColor.g = g, fontColor.r = r;

	char	text[25];

	_itoa_s(value,text,10);

	SDL_Surface *surface = NULL;

	surface = TTF_RenderText_Solid(m_pHUDFont,text,fontColor);

	RenderImage(x,y,surface->w,surface->h,surface);//,m_pWindow);

	SDL_FreeSurface(surface);
	surface = NULL;
}

void	CSDLRender::VRenderButton(char *text,int x,int y,int r,int g,int b)
{
	//return;
	SDL_Color	fontColor;
	fontColor.r = r, fontColor.g = g, fontColor.b = b;

	SDL_Surface *surface = NULL;

	surface = TTF_RenderText_Solid(m_pHUDFont,text,fontColor);
	//SDL_Surface *recast = SDL_CreateRGBSurface( 0, surface->w, surface->h, 24, 0, 0, 0, 0 );
	//SDL_BlitSurface(surface,NULL,recast,NULL);
	//the button needs to have the text centered
	int h = m_pVImage[TYPE_MENU]->m_vTexture[1].h;
	int w = m_pVImage[TYPE_MENU]->m_vTexture[1].w;
	int k = (x + w/2 - surface->w/2);
	int l = (y + h/2 - surface->h/2);
	RenderImage(x,y,w,h,m_pVImage[TYPE_MENU]->m_vTexture[1].texture);//m_imageSurface[1],m_pWindow); // render button
	//RenderImage(k,l,surface);//,m_pWindow); // render text
	SDL_Texture *texture = SDL_CreateTextureFromSurface(m_pRenderer,surface);//recast);
	RenderImage(k,l,surface->w,surface->h,texture);
	//surface->refcount++;
	SDL_DestroyTexture(texture);
	//SDL_FreeSurface(recast);
	SDL_FreeSurface(surface);

	//surface = NULL;
}

bool	CSDLRender::VRenderTextBox(unsigned int &chars,bool &next,bool first)
{
	// if (first) we are showing conversations which can be selected, so after a nextLine is detected, we go to the nextLine
	// if (next) we need to get the new m_text by removing the last text box words so that the next text box can be drawn
	// nextLine can be used as a \n

	SDL_Color	fontColor;
	fontColor.r = 98, fontColor.g = 0,fontColor.b = 49;
	SDL_Surface *surface = NULL;

	bool	countChars	=	false; // when we select a conversation, if true count characters so that after clicking we can get the text for the next text box
	bool	draw		=	true; 

	if (next)
	{
		if (chars > m_text.length())
			return false;

		m_text = m_text.substr(chars);
		countChars = true;
	}

	if (chars == 0 && !first)
		countChars = true;

	char *tempText = NULL; 
	std::vector<char> tempText1(m_text.size()+1);

	std::copy(m_text.begin(),m_text.end(),tempText1.begin());
	tempText = &tempText1[0];

	char *context = NULL;
	char *word = strtok_s(tempText," ",&context);
	int	x = 120, y = 240;

	SDL_RenderFillRect(m_pRenderer,&m_clearTextBoxRect);
	//SDL_FillRect(m_pWindow,&m_clearTextBoxRect,SDL_MapRGB(m_pWindow->format,255,255,255));

	while (true)
	{
		if (word == NULL)
			break;

		surface	= TTF_RenderText_Solid(m_pHUDFont,word,fontColor);

		if (x+surface->w >= 520 && y == 300)// && !first) 
			break;

		if (countChars) // needs to be bellow the break check because we want to save the word that would otherwise be deleted 
			chars += (std::char_traits<char>::length(word) +1); // +1 for space

		if (!std::char_traits<char>::compare("nextLine",word,8)) 
		{
			draw = false; // don't draw nextLine and don't update x and y
			x = 120, y+=20; 
		}
		
		else if (x + surface->w >= 520)
			x = 120,y+=20;

		if (draw)
		{
			RenderImage(x,y,surface->w,surface->h,surface);//,m_pWindow);
			x+=surface->w+8;
		}

		draw = true;

		word = strtok_s(NULL," ",&context);
		
		SDL_FreeSurface(surface);
		surface = NULL;

	}
	
	SDL_FreeSurface(surface); // because of break
	surface = NULL;

	next = false;

	return	true;
}

// rest

bool	CSDLRender::LoadClasses()
{
	CImageSurface	*m_pTileSurface		=	DD_NEW	CTileImageSurface;
	CImageSurface	*m_pEnemySurface	=	DD_NEW	CEnemyImageSurface;
	CImageSurface	*m_pPlayerSurface	=	DD_NEW	CPlayerImageSurface;
	CImageSurface	*m_pMenuSurface		=	DD_NEW	CMenuImageSurface;
	CImageSurface	*m_pNPCSurface		=	DD_NEW	CNPCImageSurface;
	CImageSurface	*m_pSpellSurface	=	DD_NEW	CSpellImageSurface;

	if (m_pTileSurface->VLoadAll(m_pRenderer) != 1) // tile 1 will return 0 if it failed to load
		return	false;
	Log("Tiles loaded");

	if (m_pEnemySurface->VLoadAll(m_pRenderer) != 1)
		return false;
	Log("Enemy images loaded");

	if (m_pPlayerSurface->VLoadAll(m_pRenderer) != 1)
		return false;
	Log("Player images loaded");

	if (m_pMenuSurface->VLoadAll(m_pRenderer) != 1)
		return	false;
	Log("Menu images loaded");

	if (m_pNPCSurface->VLoadAll(m_pRenderer) != 1)
		return	false;
	Log("NPC images loaded");

	if (m_pSpellSurface->VLoadAll(m_pRenderer) != 1)
		return	false;
	Log("Spell images loaded");

	m_pVImage.push_back(m_pTileSurface);
	m_pVImage.push_back(m_pEnemySurface);
	m_pVImage.push_back(m_pPlayerSurface);
	m_pVImage.push_back(m_pMenuSurface);
	m_pVImage.push_back(m_pNPCSurface);
	m_pVImage.push_back(m_pSpellSurface);

	return	true;
}

bool	CSDLRender::LoadFonts()
{
	m_pHUDFont			=	TTF_OpenFont("LiberationMono-Regular.ttf",22);
	
	if (!m_pHUDFont)
		return	false;
	return	true;
}

void	CSDLRender::RenderImage(int x,int y,int w,int h,SDL_Texture *texture)
{
	SDL_Rect	tempRect;
	tempRect.x = x;
	tempRect.y = y;
	tempRect.w = w;
	tempRect.h = h;

	SDL_RenderCopy(m_pRenderer,texture,NULL,&tempRect);
}

void	CSDLRender::RenderImage(int x,int y,int w,int h,SDL_Surface *surface)
{
	SDL_Texture *texture = NULL;
	SDL_Rect	tempRect;

	texture = SDL_CreateTextureFromSurface(m_pRenderer,surface);

	tempRect.x = x;
	tempRect.y = y;
	tempRect.w = w;
	tempRect.h = h;

	SDL_RenderCopy(m_pRenderer,texture,NULL,&tempRect);
	SDL_DestroyTexture(texture);
}

void	CSDLRender::RenderImage(int x,int y,SDL_Surface *image,SDL_Surface *surface)
{
	SDL_Rect	tempRect;
	tempRect.x = x;
	tempRect.y = y;

	SDL_BlitSurface(image,0,surface,&tempRect);
}

void	CSDLRender::RenderImage(int x,int y,int cutX,int cutY,SDL_Surface *image,SDL_Surface *surface)
{
	SDL_Rect tempRect;
	tempRect.x = x;
	tempRect.y = y;

	SDL_Rect cutRect;
	cutRect.x = cutX;
	cutRect.y = cutY;

	SDL_BlitSurface(image,&cutRect,surface,&tempRect);
}


