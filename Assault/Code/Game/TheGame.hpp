#pragma once

#include "Engine/Core/Camera2D.hpp"
#include "Game/Map.hpp"
#include "Game/Menu.hpp"

#ifndef THEGAME_HPP
#define THEGAME_HPP
//-----------------------------------------------------
class TheGame;

//-----------------------------------------------------
extern TheGame* g_TheGame;

//-----------------------------------------------
class TheGame
{
public:
	//Constructor & Destructor
	TheGame();
	~TheGame();

	const void setUpControllers();

	//Update and Render
	void Update(float deltaSeconds);
	const void Render();

	const void generateMap();

	bool canQuitNow();


	//Setters
	void SetUpCoordinateSystem();

	//variables
protected:
	Texture* m_pauseText;
	Texture* m_pressStartToContinueText;
	Texture* m_loading;
	Texture* m_period;
	Texture* m_died;
	AABB2 m_pressStartPauseScreenBounds;
	AABB2 m_pauseScreenBounds;
	AABB2 m_pauseTextLocation;
	AABB2 m_loadingBounds;
	AABB2 m_periodPosition1;
	AABB2 m_periodPosition2;
	AABB2 m_periodPosition3;
	Map* m_theMap;
	Menu* m_gameControls;

	float m_loadingTimer;
	int m_numOfPeriods;

	float g_WindowWidth;
	float g_WindowHeight;
	bool g_paused;
	bool m_releasedP;
	bool m_menu;
	bool quitNow;
	Vector2 g_pauseScreenBackgroundRelSize;
	Rgba g_pauseScreenColor;
	Camera2D playerCamera;
	XboxController** g_controllers;

	//sound
	SoundID m_songID;
	AudioSystem* m_audio;

	SoundID m_pauseID;
	AudioChannelHandle m_pause_audio;
	SoundID m_unpauseID;
	AudioChannelHandle m_unpause_audio;


	//bool songPlaying;
	AudioChannelHandle m_songHandle;
	static const std::string songName;
	static const std::string pauseName;
	static const std::string unpauseName;
	bool m_isMDown;
	bool m_isESCDown;
};

#endif //THEGAME_HPP