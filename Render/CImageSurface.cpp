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

#include "CImageSurface.h"
#include "SDL_image.h"
#include "..\const.h"
#include "..\Log.h"

/*-----------------------------
		CImageSurface
-----------------------------*/

bool	CImageSurface::DeleteAll()
{
	for( std::vector<SDL_Surface*>::iterator it = m_imageSurface.begin(); it!=m_imageSurface.end(); ++it)
		SDL_FreeSurface(*it);
	return	true;
}

int		CImageSurface::Return()
{
	if (m_imageSurface.back() == NULL)
	{
		Log(std::to_string(N)+".img failed to load");
		return N;
	}
	N++;
	return	1;
}

SDL_Surface	*CImageSurface::LoadPNGImage(char *name)
{
	SDL_Surface		*temp	=	NULL;
	SDL_Surface		*image	=	NULL;

	temp	=	IMG_Load(name);
	if (!temp)
		return	NULL;

	image	=	SDL_DisplayFormat(temp);

	SDL_FreeSurface(temp);

	return	image;
}

SDL_Surface	*CImageSurface::LoadPNGImage(char *name,bool alpha)
{
	SDL_Surface	*temp	=	NULL;
	SDL_Surface	*image	=	NULL;

	temp	=	IMG_Load(name);
	if (!temp)
		return	NULL;

	if (alpha)
		image	=	SDL_DisplayFormatAlpha(temp);
	else
		image	=	SDL_DisplayFormat(temp);

	SDL_FreeSurface(temp);

	return	image;
}

/*-----------------------------
		CTileImageSurface
-----------------------------*/

int	CTileImageSurface::LoadAll()
{
	N = 0;
	char	a[14] = "",c[14]="",b[]="img/";
	SDL_Surface	*tempImage = NULL;
	for (int i = TILE_NUMBER_START; i<=TILE_NUMBER_FINISH;i++)
	{
		strcpy_s(c,b);
		_itoa_s(i,a,10);
		strcat_s(c,a);
		strcat_s(c,".png");
		m_imageSurface.push_back(LoadPNGImage(c)); 
		if (N == 1)
		{
			if (m_imageSurface[1] == NULL)
			{
				Log(std::to_string(N)+".img failed to load");
				return 0;
			}
		}
		else if (Return() != 1)
			return	N;
		a[0]=0;
		c[0]=0;
	}
	return	1;
}

/*-----------------------------
		CEnemyImageSurface
-----------------------------*/

int	CEnemyImageSurface::LoadAll()
{
	N = 4000;
	m_imageSurface.push_back(LoadPNGImage("img/4000.png",true));
	if (Return() != 1)
		return	N;

	return	1;
}

/*-----------------------------
		CPlayerImageSurface
-----------------------------*/

int	CPlayerImageSurface::LoadAll()
{
	N = 950;
	m_imageSurface.push_back(LoadPNGImage("img/950.png",true));
	if (Return() != 1)
		return N;
	return	1;
}

/*-----------------------------
		CMenuImageSurface
-----------------------------*/

int	CMenuImageSurface::LoadAll()
{
	N = 1000;
	m_imageSurface.push_back(LoadPNGImage("img/1000.png"));
	if (Return() != 1)
		return N;
	m_imageSurface.push_back(LoadPNGImage("img/1001.png"));
	if (Return() != 1)
		return N;
	m_imageSurface.push_back(LoadPNGImage("img/1002.png"));
	if (Return() != 1)
		return N;
	
	return	1;
}


/*-----------------------------
		CNPCSurface
-----------------------------*/

int	CNPCImageSurface::LoadAll()
{
	N = 2000;
	m_imageSurface.push_back(LoadPNGImage("img/2000.png",true));
	if (Return() != 1)
		return N;

	return	1;
}

/*-----------------------------
		CSpellSurface
-----------------------------*/

int	CSpellImageSurface::LoadAll()
{
	N = 3000;
	m_imageSurface.push_back(LoadPNGImage("img/3000.png",true));
	if (Return() != 1)
		return N;
	m_imageSurface.push_back(LoadPNGImage("img/3001.png",true));
	if (Return() != 1)
		return N;

	return	1;
}

