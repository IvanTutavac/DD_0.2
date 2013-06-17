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
class CQuestManager;
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

enum RenderTextBoxState
{
	RTBS_nothing,
	RTBS_renderTextBox,
	RTBS_renderFirstTextBox
};

struct _renderFlags
{
	RenderState		state;	// use RS_
	RenderTextBoxState textBoxState; // use RTBS_

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

enum NPCConversation
{
	NPCC_nothing,
	NPCC_commonTalk,
	NPCC_questTalk
};

struct _logicFlags
{
	logicGameState	state; // use LGS_
	NPCConversation	npcConversation; // use NPCC_

	void			Reset();
};

struct _TextRenderInfo
{
	bool			nextTextBox;
	bool			setTextBox;
	bool			setFirstTextBox;
	bool			setCommonTextBox;
	int				selectedNPCIndex; 
	int				selectedConversationIndex;
	int				selectedTextIndex;
	unsigned int	chars;
};

class CLogic
{

private:
	
	std::vector<CEnemy> m_VEnemyList; // enemies which can be available 
	std::vector<CEnemy> m_VEnemy; // enemies on map
	std::vector<CEnemy> m_VCloseEnemy; // enemies near you
	//std::vector<CQuest*> m_VQuests; // contains all quests
	CQuestManager	*m_pQuest;

	std::vector<_spell> m_VAllSpells; // contains all spells

	_lockFlags		m_lockFlags;
	_logicFlags		m_logicFlags;

	int				m_UniqueSpellID; // used to set unique _spell.ID
	int				m_UniqueEnemyID;
	int				m_IDLimit;
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
	void			Action();
	void			FinalCheck(); // clean up ...

	void			UpdateConversationState();
	void			EndConversation();

	bool			CheckCollision(int x1,int y1,int x2,int y2,int size);
	bool			CheckPointCollision(int x1,int y1,int x2,int y2,int sizeX,int sizeY);
	void			PlayerEnemyCollision();
	void			PlayerNPCCollision();
	void			SpellCollision();
	bool			CheckIfNPCNearby();
	bool			CheckDistance(int x1,int y1,int x2,int y2,int distanceX,int distanceY);

	void			CheckInGameClickRelease(const CEventMessage *EventMessage);
	void			CheckTextBoxClick(const CEventMessage *EventMessage);
	void			CheckConversationSelectionClick(const CEventMessage *EventMessage);
	void			CheckTextSelectionClick(const CEventMessage *EventMessage);
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
	void			SetupSpellMap(int id/*_spell.ID*/,float x,float y,double duration /*_spell.duration*/,int speed /*_spell.speed*/);


	void			CameraMovement(const CEventMessage *EventMessage);
	void			SpellMovement(const CEventMessage *EventMessage);
	void			MoveEntity(const CEventMessage *EventMessage,float &x,float &y,float speed);
	void			MoveLeft(float &x,float speed,bool flag = true);
	void			MoveRight(float &x,float speed,bool flag = true);
	void			MoveUp(float &y,float speed,bool flag = true);
	void			MoveDown(float &y,float speed,bool flag = true);



	// check all spell timers, see if they exceeded their duration, if yes delete the spell from CMap and delete the spellTimer
	void			CheckTimedOutSpells(); 

	// will return	false if player died, and check which monster died
	bool			CheckIfAlive();

public:

	_TextRenderInfo m_textRenderInfo;
	_renderFlags	m_renderFlags;
	CMap			*m_pMap;
	CPlayer			*m_pPlayer;
	std::vector<CNPC> m_VNpc;

	bool			Init();
	void			Clean();

	bool			Run(CEventMessage *EventMessage,double deltaTime);

};