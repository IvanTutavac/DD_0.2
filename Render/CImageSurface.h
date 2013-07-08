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

#include "SDL.h"
#include <vector>

class CImageSurface
{
protected:
	int	N;

public:

	virtual ~CImageSurface(){}

	//std::vector<SDL_Surface*>	m_imageSurface; // contains images
	std::vector<SDL_Texture*> m_pVTexture;
	virtual	int		VLoadAll(SDL_Renderer *m_pRenderer)=0;
	bool			DeleteAll();
	int				Return();

	SDL_Surface		*LoadPNGImage(char *name);
	SDL_Surface		*LoadPNGImage(char *name,bool alpha);
	SDL_Texture		*LoadTexture(char *name,SDL_Renderer *m_pRenderer);
};

class  CTileImageSurface : public CImageSurface
{
public:
	int		VLoadAll(SDL_Renderer *m_pRenderer) override;
};

class  CEnemyImageSurface : public CImageSurface
{
public:
	int		VLoadAll(SDL_Renderer *m_pRenderer) override;
};

class CPlayerImageSurface : public CImageSurface
{
public:
	int		VLoadAll(SDL_Renderer *m_pRenderer) override;
};

class  CMenuImageSurface : public CImageSurface
{
public:
	int		VLoadAll(SDL_Renderer *m_pRenderer) override;
};

class CNPCImageSurface : public CImageSurface
{
public:
	int		VLoadAll(SDL_Renderer *m_pRenderer) override;
};

class CSpellImageSurface : public CImageSurface
{
public:
	int		VLoadAll(SDL_Renderer *m_pRenderer) override;
};
