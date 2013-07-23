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

#include "CImage.h"
#include "SDL_image.h"
#include "..\const.h"
#include "..\Log.h"

bool	CImage::Init(SDL_Renderer *Renderer,const int imgStart,const int imgFinish)
{
	m_n = imgStart;

	char	buffer[14] = "",name[14] = "",b[] ="img/";

	for (int i = imgStart; i <= imgFinish; i++)
	{
		strcpy_s(name,b);
		_itoa_s(i,buffer,10);
		strcat_s(name,buffer);
		strcat_s(name,".png");

		if (!CreateTexture(name,Renderer))
			return	false;

		buffer[0] = 0;
		name[0] = 0;
	}

	return	true;
}

void	CImage::Clean()
{
	for (size_t i = 0; i < m_vTexture.size(); i++)
	{
		SDL_DestroyTexture(m_vTexture[i].texture);
	}

	m_vTexture.clear();
}

bool	CImage::CreateTexture(char *name,SDL_Renderer *Renderer)
{
	SDL_Surface *surface = nullptr;

	surface = IMG_Load(name);

	if (surface == nullptr)
	{
		WriteToLog(SURFACE);

		return	false;
	}

	_texture texture;

	texture.w = surface->w;
	texture.h = surface->h;
	texture.texture = SDL_CreateTextureFromSurface(Renderer,surface);

	if (texture.texture == nullptr)
	{
		WriteToLog(TEXTURE);

		return	false;
	}

	m_vTexture.push_back(texture);

	SDL_FreeSurface(surface);

	m_n++;

	return true;
}

void	CImage::WriteToLog(const int type)
{
	char text[32];

	_itoa_s(m_n,text,10);

	if (type == TEXTURE)
	{
		strcat_s(text,".texture was not created");
	}
	else if (type == SURFACE)
	{
		strcat_s(text,".img failed to load");
	}

	Log(text);
}
