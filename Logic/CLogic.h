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
class CEntity;
class CEnemy;
class CPlayer;
class CQuest;
class CNPC;
//class CTimer;

struct _lockFlags
{
	bool			movement;
	bool			cameraMovement;
	bool			scroll;

	void			AllowAll();
	void			DisableAll();
};

enum RenderState
{
	RS_nothing,
	RS_renderMap,
	RS_renderMainMenu,
	RS_renderGameExit,
	RS_renderMapEditor,
	RS_renderAllTiles,
	RS_renderOptions
};

struct _renderFlags
{
	RenderState		state;	// use RS_

	bool			renderTextBox;
	bool			renderGameExit;
	bool			renderTalkMessage;

	void			Reset();
};

enum logicGameState
{
	LGS_nothing,
	LGS_exit,
	LGS_intro,
	LGS_mainMenu,
	LGS_mainOptions, // options accessed from mainMenu 
	LGS_options, // options accessed from inGame
	LGS_inGame,
	LGS_mapEditor,
	LGS_allTiles
};

struct _logicFlags
{
	logicGameState	state; // use LGS_

	void			Reset();
};

class CLogic
{

private:

	std::vector<CEnemy*> m_VEnemy; 
	std::vector<CEnemy*> m_VCloseEnemy;
	std::vector<CQuest*> m_VQuests; // contains all quests

	std::vector<_spell> m_VAllSpells; // contains all spells

	_lockFlags		m_lockFlags;
	_logicFlags		m_logicFlags;

	int				m_UniqueSpellID; // used to set unique _spell.ID
	int				m_IDLimit;

	bool			m_drawTiles;

	double			deltaTime;

	bool			LoadAllEntities();
	bool			LoadAllSpells();
	void			InitFlags();

	bool			CheckState(CEventMessage	*EventMessage);
	
	void			UpdateObjects();
	void			Pathfinding();
	bool			CheckMouseClick(const CEventMessage *EventMessage);
	void			CheckPlayerInput(const CEventMessage *EventMessage);
	void			Movement(const CEventMessage *EventMessage);
	void			Nearby(const CEventMessage *EventMessage);
	void			Collision();
	void			Action();
	void			FinalCheck(); // clean up ...


	bool			CheckCollision(int x1,int y1,int x2,int y2,int size);
	// check point collision with an object that has x2,y2,sizeX and sizeY
	bool			CheckPointCollision(int x1,int y1,int x2,int y2,int sizeX,int sizeY);
	void			PlayerEnemyCollision();
	void			PlayerNPCCollision();
	bool			CheckIfNPCNearby();
	bool			CheckDistance(int x1,int y1,int x2,int y2,int distanceX,int distanceY);


	bool			CheckMenuClick(const CEventMessage *EventMessage);
	void			CheckOptionsClick(const CEventMessage *EventMessage);
	void			CheckMapEditorClickRelease(const CEventMessage *EventMessage);
	void			CheckMapEditorClickPress(const CEventMessage *EventMessage);
	void			CheckAllTilesClick(const CEventMessage *EventMessage);
	bool			InitMapEditor();
	bool			InitMap();


	// cast the spell, if needed, create data needed for rendering and for timer
	void			PlayerSpellCast(const CEventMessage *EventMessage);

	// create spell data needed for movement ( render and timer data )
	void			SetupSpellMap(int id/*_spell.ID*/,int x,int y,int duration /*_spell.duration*/,int speed /*_spell.speed*/);


	void			CameraMovement(const CEventMessage *EventMessage);
	void			SpellMovement(const CEventMessage *EventMessage);
	void			MoveEntity(const CEventMessage *EventMessage,float &x,float &y,int speed);
	void			MoveLeft(float &x,float speed,bool flag = true);
	void			MoveRight(float &x,float speed,bool flag = true);
	void			MoveUp(float &y,float speed,bool flag = true);
	void			MoveDown(float &y,float speed,bool flag = true);



	// check all spell timers, see if they exceeded their duration, if yes delete the spell from CMap and delete the spellTimer
	void			CheckTimedOutSpells();

public:

	bool			m_nextTextBox;
	bool			m_setTextBox;
	_renderFlags	m_renderFlags;
	CMap			*m_pMap;
	CPlayer			*m_pPlayer;
	std::vector<CNPC*> m_VNpc;

	bool			Init();
	void			Clean();

	bool			Run(CEventMessage *EventMessage,double deltaTime);

};