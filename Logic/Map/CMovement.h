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

class CMap;

class CMovement
{

private:

	static const	int	m_left	=	1;
	static const	int	m_right	=	2;
	static const	int	m_up	=	1;
	static const	int	m_down	=	2;

	void	MoveRight(float &x,float speed,double deltaTime);
	void	MoveLeft(float &x,float speed,double deltaTime);
	void	MoveUp(float &y,float speed,double deltaTime);
	void	MoveDown(float &y,float speed,double deltaTime);

public:

	bool	Init();
	void	Clean();

	// mouse based spell movement
	// it will move all spells
	void	SpellMovement(CMap *MapPointer,float mouseX,float mouseY,double deltaTime);
	
	//mouse based camera movement
	//it will update cameraX and cameraY	
	void	CameraMovement(int mouseX,int mouseY,float &cameraX,float &cameraY,double deltaTime);
	
	// keyboard movement
	//it will move one entity
	void	MoveEntity(int leftRight,int upDown,float &x,float &y,float speed,double deltaTime);


};