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
#include <vector>
#include "dataStructs.h"

class CMap;
class CMovement;
class CCollision;
class CEntityManager;
class CSpell;
class CQuestManager;
class CMouse;
class CAction;

class CLogic
{

private:
	
	CQuestManager	*m_pQuest;
	CMovement		*m_pMovement;
	CCollision		*m_pCollision;
	CAction			*m_pAction;

	_lockFlags		m_lockFlags;
	_logicFlags		m_logicFlags;

	int				m_nearNPCIndex; // CheckIfNPCNearby sets the value

	bool			m_drawTiles;

	double			deltaTime;

	bool			LoadAllEntities();
	bool			LoadAllSpells();
	bool			LoadEnemies();
	void			InitFlags();
	bool			SetMapEnemy();

	bool			CheckState(CEventMessage	*EventMessage);
	
	void			UpdateObjects();
	void			Pathfinding();
	bool			CheckMouseClick(const CEventMessage *EventMessage);
	void			CheckPlayerInput(const CEventMessage *EventMessage);
	void			Movement(const CEventMessage *EventMessage);
	void			Nearby(CEventMessage *EventMessage);
	void			Collision();
	bool			Action();
	void			FinalCheck(); // clean up ...

	void			UpdateConversationState();
	void			EndConversation();

	bool			CheckCollision(int x1,int y1,int x2,int y2,int size);
	bool			CheckPointCollision(int x1,int y1,int x2,int y2,int sizeX,int sizeY);
	bool			CheckIfNPCNearby();
	bool			CheckDistance(int x1,int y1,int x2,int y2,int distanceX,int distanceY);


	// cast the spell, if needed, create data needed for rendering and for timer
	void			PlayerSpellCast(const CEventMessage *EventMessage);


	// check all spell timers, see if they exceeded their duration, if yes delete the spell from CMap and delete the spellTimer
	void			CheckTimedOutSpells(); 

	// will return	false if player died, and check which monster died
	bool			CheckIfAlive();

public:

	_TextRenderInfo m_textRenderInfo;
	_renderFlags	m_renderFlags;

	CMouse			*m_pMouse;
	CMap			*m_pMap;
	CEntityManager  *m_pEntity;
	CSpell			*m_pSpell;

	bool			Init();
	void			Clean();

	bool			Run(CEventMessage *EventMessage,double deltaTime);

};