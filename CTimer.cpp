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

#include "CTimer.h"
#include <profileapi.h>

CTimer::CTimer()
{
	this->startTime.QuadPart	=	0;
	this->endTime.QuadPart		=	0;
	QueryPerformanceFrequency(&frequency);
}

void	CTimer::Start()
{
	QueryPerformanceCounter(&startTime);
}

void	CTimer::End()
{
	QueryPerformanceCounter(&endTime);
}

double	CTimer::GetTimeEx()
{
	LARGE_INTEGER	tempTime;
	QueryPerformanceCounter(&tempTime);
	return	((double)tempTime.QuadPart * (1000000 / (double)frequency.QuadPart));
}

double	CTimer::GetTime()
{
	LARGE_INTEGER	temp;
	temp.QuadPart = endTime.QuadPart - startTime.QuadPart;
	return ((double)temp.QuadPart / (double)frequency.QuadPart);
}

double CTimer::GetMicro()
{
	LARGE_INTEGER	temp;
	temp.QuadPart	=	endTime.QuadPart - startTime.QuadPart;
	return	((double)temp.QuadPart * (1000000 / (double)frequency.QuadPart));
	//return	((double)frequency.QuadPart / ((double)temp.QuadPart) / 1000000);
}