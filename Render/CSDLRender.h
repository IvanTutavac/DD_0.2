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
#include "CRender.h"
#include "SDL.h"
#include <SDL_ttf.h>
#include "CImage.h"
#include <vector>

//class CImageSurface;

class CSDLRender : public CRender
{

private:

	//SDL_Surface		*m_pWindow;

	SDL_Renderer	*m_pRenderer;
	SDL_Window		*m_pWindow;
	TTF_Font		*m_pHUDFont;
	
	std::vector<CImage> m_vImage;

	SDL_Rect		m_clearHUDRect;
	SDL_Rect		m_clearTextBoxRect;


	bool	LoadImages();
	bool	LoadFonts();

	void	RenderImage(int x,int y,int w,int h,SDL_Texture *texture);
	// a texture will be created from the surface and then rendered
	void	RenderImage(int x,int y,int w,int h,SDL_Surface *surface);
	void	RenderImage(int x,int y,SDL_Surface *image,SDL_Surface *surface);
	void	RenderImage(int x,int y,int cutX,int cutY,SDL_Surface *image,SDL_Surface *surface);

	bool	CheckIfInWindow(int cameraX,int cameraY,int &x,int &y);
	void	UpdateClearHUDRect();

protected:

	void	VRenderFPS(int fps) override;
	void	VRenderImage(int x,int y,int w,int h,const int type,int i) override;
	//void	VRenderImage(int x,int y,int cutX,int cutY,const int type, int i) override;

	// h and w will be read from m_vTexture
	void	VRenderImage(int x,int y,const int type,int i) override;

public:

	bool	VInit() override;
	void	VClean() override;

	void	VClearWindow() override;
	void	VClearHUD() override;

	void	VUpdateWindow() override;
	
	void	VRenderText(const char *text,int x,int y,int r,int g,int b) override;
	void	VRenderValue(int value,int x,int y,int r,int g,int b) override;
	void	VRenderButton(char *text,int x,int y,int r,int g,int b) override;
	
	// will return false if there's no more text to render, logic reads the result 
	bool	VRenderTextBox(unsigned int &words,bool &next,bool first) override;
	void	VReadMessage(CMessage *Message) override;

};