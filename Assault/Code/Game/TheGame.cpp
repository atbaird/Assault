#include "Game/TheGame.hpp"

TheGame* g_TheGame = nullptr; 

const std::string TheGame::songName = "Data/Soundtrack/awake10_megaWall.mp3";
const std::string TheGame::pauseName = "Data/Soundtrack/whoosh2.wav";
const std::string TheGame::unpauseName = "Data/Soundtrack/rage.wav";

//--------------------------------------------
//Constructor & Destructor
TheGame::TheGame() 
: playerCamera(Vector2(0, 0), true),
g_WindowWidth(16.f),
g_WindowHeight(9.f),
quitNow(false),
g_paused(false),
m_releasedP(true),
m_menu(true),
m_isESCDown(false),
m_audio(nullptr)
{
	setUpControllers();


	//sound
	m_audio = new AudioSystem();
	m_songID = m_audio->CreateOrGetSound(songName, true);
	m_songHandle = m_audio->PlaySound(m_songID, 0.5f);
	m_pauseID = m_audio->CreateOrGetSound(pauseName, false);
	m_unpauseID = m_audio->CreateOrGetSound(unpauseName, false);
	//songPlaying = true;

	//map

	m_theMap = nullptr;
	generateMap();
	//constructor

	//testTexture and position.
	m_pressStartToContinueText = Texture::CreateOrGetTexture("Data/Images/Press start to continue.png");

	//pause screen
	g_pauseScreenBackgroundRelSize = Vector2(14.f, 7.f);
	g_pauseScreenColor = g_Renderer->Gray;
	g_pauseScreenColor.Alpha = 0.7f;

	m_pauseText = Texture::CreateOrGetTexture("Data/Images/Pause text.png");
	m_died = Texture::CreateOrGetTexture("Data/Images/You Died.png");
	m_pauseScreenBounds = AABB2(Vector2(1.f, 1.f), Vector2(15.f, 8.f)); //16 by 9, center = 8, 4.5
	m_pauseTextLocation = AABB2(Vector2(6.f, 5.f), Vector2(10.f, 9.f));
	m_pressStartPauseScreenBounds = AABB2(Vector2(6.f, 2.5f), Vector2(10.f, 6.5f));

	//loading screen
	m_loading = Texture::CreateOrGetTexture("Data/Images/Loading.png");
	m_loadingBounds = AABB2(Vector2(6.f,2.5f), Vector2(10.f, 6.5f));
	m_loadingTimer = 0.f;
	m_numOfPeriods = 0; // 0 = 1, 1 = 2... etc.

	m_period = Texture::CreateOrGetTexture("Data/Images/period.png");
	m_periodPosition1 = AABB2(Vector2(9.5f, 3.5f), Vector2(10.5f, 4.5f));
	m_periodPosition2 = AABB2(Vector2(10.5f, 3.5f), Vector2(11.5f, 4.5f));
	m_periodPosition3 = AABB2(Vector2(11.5f, 3.5f), Vector2(12.5f, 4.5f));


	//Attract mode
	m_gameControls = new Menu(g_controllers[0]);
}
TheGame::~TheGame()
{
	//destructor
	delete m_theMap;

	delete m_gameControls;

	//controllers
	for (int i = 0; i < 4; i++)
	{
		delete g_controllers[i];
		g_controllers[i] = nullptr;
	}

	delete g_controllers;
	g_controllers = nullptr;
}

const void TheGame::generateMap()
{
	if (m_theMap != nullptr)
	{
		delete m_theMap;
		m_theMap = nullptr;
	}
	m_theMap = new Map(100, 100, g_controllers,m_audio,false, "Data/Maps/Map01.png");
	m_theMap->setUpPlayer(new Tank());
}

const void TheGame::setUpControllers()
{
	//xbox controller
	g_controllers = new XboxController*[4];
	for (int i = 0; i < 4; i++)
	{
		g_controllers[i] = new XboxController(i);
	}
}

//Update and Render
void TheGame::Update(float deltaSeconds)
{
	m_audio->Update(deltaSeconds);
	if (m_menu == true)
	{
		m_gameControls->Update(g_theApp->isKeyDown(' '));
		if (m_gameControls->getPageNum() >= m_gameControls->getMaxPages())
		{
			m_menu = false;
		}
	}
	else if (m_theMap->isGameLoading() == true)
	{
		//timer is purely for animating the periods while the game is loading.
		m_loadingTimer += deltaSeconds;
		m_theMap->LoadingGame();
		if (m_loadingTimer >= 0.4f)
		{
			m_loadingTimer = 0.f;
			m_numOfPeriods += 1;
			if (m_numOfPeriods > 2)
			{
				m_numOfPeriods = 0;
			}
		}
	}
	else if (g_paused == false)
	{
		//every entity move! the game is not paused nor is it loading!
		if (m_releasedP == true && (g_theApp->isKeyDown('P') == true || g_controllers[0]->isStartButtonDown() == true))
		{
			g_paused = true;
			m_releasedP = false;
			m_audio->StopChannel(m_unpause_audio);
			m_pause_audio = m_audio->PlaySound(m_pauseID);
		}
		m_theMap->Update(deltaSeconds);

		Vector2 camPosition = m_theMap->g_positionPlayerLastAlive;

		float camAddAngle = m_theMap->g_playerLastAngularDisplacement - playerCamera.getXYANgularDisplacement();

		playerCamera.CameraUpdate(deltaSeconds, camPosition, camAddAngle);
	}
	else
	{
		//game is paused.
		m_theMap->Update(0.f);
		if (m_releasedP == true && (g_theApp->isKeyDown('P') == true || g_controllers[0]->isStartButtonDown() == true) && m_theMap->m_player != nullptr)
		{
			g_paused = false;
			m_releasedP = false;
			m_audio->StopChannel(m_pause_audio);
			m_unpause_audio = m_audio->PlaySound(m_unpauseID);
		}
	}
	//player dead
	if (m_theMap->m_playerDeadForThisManySeconds > 5.f)
	{
		g_paused = true;
		m_theMap->m_playerDeadForThisManySeconds += deltaSeconds;
	}
	if (m_theMap->m_playerDeadForThisManySeconds > 15.f)
	{
		quitNow = true;
	}
	//verify controller's still plugged in; had an issue where when player died, could not push start button.
	g_controllers[0]->amIPluggedIn();
	if (g_theApp->isKeyDown('P') == false && g_controllers[0]->isStartButtonDown() == false)
	{
		m_releasedP = true;
	}
	if (m_isMDown == false && g_theApp->isKeyDown('M') == true)
	{
		m_isMDown = true;
		if (m_songHandle != nullptr)
		{
			m_audio->StopChannel(m_songHandle);
			m_songHandle = nullptr;
		}
		else
		{
			m_songHandle = m_audio->PlaySound(m_songID, 0.5f);
		}
	}


	if (g_theApp->isKeyDown('M') == false)
	{
		m_isMDown = false;
	}

	if (m_releasedP == true && g_paused == true && m_theMap->m_player == nullptr && (g_theApp->isKeyDown('P') == true || g_controllers[0]->isStartButtonDown() == true))
	{
		generateMap();
		g_paused = false;
	}
	if (m_isESCDown == false && (g_controllers[0]->isBackButtonDown() == true || g_theApp->isKeyDown(VK_ESCAPE) == true))
	{
		if (m_menu == true)
		{
			quitNow = true;
		}
		else
		{
			m_menu = true;
			g_paused = true;
			m_gameControls->setCurrentPage(0);
		}
		m_isESCDown = true;
	}

	if (m_isESCDown == true && (g_controllers[0]->isBackButtonDown() == false && g_theApp->isKeyDown(VK_ESCAPE) == false))
	{
		m_isESCDown = false;
	}
}
const void TheGame::Render()
{
	//method is to render EVERYTHING within the game.
	SetUpCoordinateSystem();

	g_Renderer->clearScreen(g_Renderer->Black);
	
	//tiles
	m_theMap->Render();
	if (m_menu == true)
	{
		g_Renderer->SetOrtho(Vector2(0.f, 0.f), Vector2(16.f, 9.f));
		m_gameControls->Render();
	}
	else if (m_theMap->isGameLoading() == true)
	{
		g_Renderer->SetOrtho(Vector2(0.f, 0.f), Vector2(16.f, 9.f));

		//renders a light gray screen with the word "loading" and three periods which animate.
		g_Renderer->DrawAABB(AABB2(Vector2(0.f, 0.f), Vector2(16.f, 9.f)), Rgba(0.5f, 0.5f, 0.5f, 1.f));
		g_Renderer->DrawTexturedAABB(m_loadingBounds, m_loading, Vector2(0.f, 0.f), Vector2(1.f, 1.f), g_Renderer->White);
		g_Renderer->DrawTexturedAABB(m_periodPosition1, m_period, Vector2(0.4f, 0.4f), Vector2(0.6f, 0.6f), g_Renderer->White);
		if (m_numOfPeriods >= 1)
		{
			g_Renderer->DrawTexturedAABB(m_periodPosition2, m_period, Vector2(0.4f, 0.4f), Vector2(0.6f, 0.6f), g_Renderer->White);
		}
		if (m_numOfPeriods >= 2)
		{
			g_Renderer->DrawTexturedAABB(m_periodPosition3, m_period, Vector2(0.4f, 0.4f), Vector2(0.6f, 0.6f), g_Renderer->White);
		}
	}

	if (m_theMap->m_playerDeadForThisManySeconds >= 5.f)
	{
		g_paused = true;
	}
	
	if (m_menu == false && g_paused == true)
	{
		g_Renderer->SetOrtho(Vector2(0.f, 0.f), Vector2(16.f, 9.f));

		g_Renderer->DrawAABB(AABB2(Vector2(0.f,0.f), Vector2(16.f,9.f)), Rgba(0.f,0.f,0.f,0.3f));
		g_Renderer->DrawAABB(m_pauseScreenBounds, g_pauseScreenColor);
		//DrawTexturedAABB(const AABB2& bounds, const Texture* texture, const Vector2& texCoordMins, const Vector2& texCoordMaxs, const Rgba& tint);
		g_Renderer->DrawTexturedAABB(m_pressStartPauseScreenBounds, m_pressStartToContinueText, Vector2(0.f, 0.f), Vector2(1.f, 1.f), g_Renderer->White);
		if (m_theMap->m_player != nullptr)
		{
			g_Renderer->DrawTexturedAABB(m_pauseTextLocation, m_pauseText, Vector2(0.f, 0.f), Vector2(1.f, 1.f), g_Renderer->White);
		}
		else
		{
			g_Renderer->DrawTexturedAABB(m_pauseTextLocation, m_died, Vector2(0.f, 0.f), Vector2(1.f, 1.f), g_Renderer->White);
		}
	}
}



//Setters
void TheGame::SetUpCoordinateSystem()
{
	//handles the screen orientation so as it will follow the player around.
	Vector2 screenSize(16.f, 9.f);
	Vector2 cameraCenteredOnScreenPosition(screenSize.x * 0.5f, screenSize.y * 0.2f);
	Vector2 cameraPos =  playerCamera.m_position; // #FIXME: Camera2D will be its own class...
	float cameraDegrees = m_theMap->g_playerLastAngularDisplacement - 90.f;

	g_Renderer->SetOrtho(Vector2(0.f, 0.f), screenSize);
	g_Renderer->Translate(cameraCenteredOnScreenPosition);
	g_Renderer->Rotate(-cameraDegrees);
	g_Renderer->Translate((cameraPos) * -1.f);

}

bool TheGame::canQuitNow()
{
	return quitNow;
}