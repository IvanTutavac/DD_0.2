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

const int SPELL_LVL_LIMIT = 2;

enum SpellType
{
	ST_hpRemoval,
	ST_hpHeal,
	ST_stun
};

struct _spellData
{
	int		value; // used for dmg, heal and so on
	int		value1;
	int		value2;
	int		cost; // mana cost
	double	duration; // how long will the spell be alive on map if moving type, else can be used for buff duration and so on
	double	cd; // cooldown, reduced by deltaTime... when <= 0 we can cast a new spell
	double	tempCD; // used to restore cd
	int		speed;
	int		n; // number of entities the spell will affect
	int		range; // can be used for aoe range, movement range ...
	int		skillPoints; // skill points needed to acquire the spell
};

struct _spell
{
	int		ID;  // spell number, also img spell number... used to set _location.imgID 
	char	name[20];
	bool	moving; // used to check whether a spell will move
	bool	casted; // whether we can cast a spell that has cd > 0 ( has not yet been casted )

	_spellData spell[SPELL_LVL_LIMIT]; // _EntitySpell.lvl 

	SpellType	type; // use ST_
};

enum TextState 
{            
	TS_nothing,
	TS_introduction, // text started
	TS_option,		 // yes no
	TS_yes,
	TS_no,
	TS_wait,
	TS_finished
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

struct _lockFlags
{
	bool			movement;
	bool			cameraMovement;
	bool			scroll;

	void			AllowAll();
	void			DisableAll();
};

struct _renderFlags
{
	RenderState		state;	// use RS_
	RenderTextBoxState textBoxState; // use RTBS_

	bool			renderGameExit;
	bool			renderTalkMessage;

	void			Reset();
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



