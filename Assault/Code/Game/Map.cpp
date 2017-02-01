#include "Game/Map.hpp"

TileDef Map::s_tileDefinitions[NUM_TILE_TYPES];


const std::string Map::m_explosionsound_Name = "Data/Soundtrack/8bit_bomb_explosion.wav";
const std::string Map::m_gunshot_Name = "Data/Soundtrack/8bit_gunloop_explosion.wav";

//constructors and destructor
Map::Map(int xSize, int ySize, XboxController** controllers, AudioSystem* audio, bool randomlyGenerate, std::string mapFileLocation)
{
	//constructor
	m_gameLoading = true;

	m_bottomLeftValForOrthoMiniMap = Vector2(6.f, 3.375f);
	//sounds
	m_audio = audio;
	m_explosionsoundID = m_audio->CreateOrGetSound(m_explosionsound_Name, false);
	m_gunshotID = m_audio->CreateOrGetSound(m_gunshot_Name, false);
	m_fogOfWarActive = false;
	m_miniMapFogOfWarActive = false;

	//controllers
	g_controllers = controllers;

	//tile definitions
	InitializeTileDefinitions();
	//Explosion definitions
	InitializeExplosionDefinitions();

	m_testTexture = Texture::CreateOrGetTexture("Data/Images/Red Smiley.png");
	m_aAbb = AABB2(Vector2(2.f, 2.f), Vector2(3.f, 3.f));

	m_spaceBarHeldAndSinceLastBulletFire = 0.f;
	m_timeBetweenPlayerBullets = 1.f / 8.f;
	m_tileDistanceFromPlayerInOrderToPlaySound = 11.f;
	m_fogOfWar = m_tileDistanceFromPlayerInOrderToPlaySound - 5.f;
	m_normalAmountToRenderFromPlayer = 13.f;


	//bullets
	m_maxBullets = 500;
	m_bullets.resize(m_maxBullets);
	m_numExistingBullets = 0;
	for (int i = 0; i < m_maxBullets; i++)
	{
		m_bullets[i] = nullptr;
	}

	m_spaceIsReleased = true;
	m_isFReleased = true;
	m_isRReleased = true;

	//death
	m_playerDeadForThisManySeconds = 0.f;

	m_deathScreenColor = Rgba(0.f, 0.f, 0.f, 0.f);
	m_amountOfAlphaToAddOverSeconds = (0.1f / 10.f);

	if (randomlyGenerate == true)
	{
		m_size = TileCoords(xSize, ySize);
		m_tiles.resize(m_size.x * m_size.y);
		randomGenerateMap();

		//turrets
		m_maxTurrets = 20;
		m_numTurretsStart = 10;
		m_turrets = new EnemyTurret*[m_maxTurrets];
		m_numExistingTurrets = 0;
		for (int i = 0; i < m_maxTurrets; i++)
		{
			m_turrets[i] = nullptr;
		}
		m_generatedTurretThisFrame = false;
		m_doneGeneratingTurretsAtStart = false;

		//Enemy Tanks
		m_maxEnemyTanks = 20;
		m_numEnemyTanksStart = 10;
		m_numExistingEnemyTanks = 0;
		m_enemyTanks = new EnemyTank*[m_maxEnemyTanks];
		for (int i = 0; i < m_maxEnemyTanks; i++)
		{
			m_enemyTanks[i] = nullptr;
		}
		m_generatedEnemyTankThisFrame = false;
		m_doneGeneratingEnemyTanksAtStart = false;

		//ally tanks
		m_maxAllyTanks = 20;
		m_numAllyTanksStart = 10;
		m_numExistingAllyTanks = 0;
		m_allyTanks = new AllyTank*[m_maxAllyTanks];
		for (int i = 0; i < m_maxAllyTanks; i++)
		{
			m_allyTanks[i] = nullptr;
		}
		m_generatedAllyTankThisFrame = false;
		m_doneGeneratingAllyTanksAtStart = false;
	}
	else
	{
		m_numExistingEnemyTanks = 0;
		m_numExistingTurrets = 0;
		m_numTurretsStart = 0;
		m_numEnemyTanksStart = 0;
		m_numExistingAllyTanks = 0;
		loadMapFromImage(mapFileLocation);
	}
}
Map::~Map()
{
	//destructor
	g_controllers = nullptr;
	//player
	if (m_player != nullptr)
	{
		delete m_player;
		m_player = nullptr;
	}

	//bullets
	for (int i = 0; i < m_numExistingBullets; i++)
	{
		if (m_bullets[i] != nullptr)
		{
			delete m_bullets[i];
			m_bullets[i] = nullptr;
		}
	}

	//turrets
	for (int i = 0; i < m_numExistingTurrets; i++)
	{
		if (m_turrets[i] != nullptr)
		{
			delete m_turrets[i];
			m_turrets[i] = nullptr;
		}
	}
	delete m_turrets;

	//enemy Tanks
	for (int i = 0; i < m_numExistingEnemyTanks; i++)
	{
		if (m_enemyTanks[i] != nullptr)
		{
			delete m_enemyTanks[i];
			m_enemyTanks[i] = nullptr;
		}
	}
	delete m_enemyTanks;
	//ally Tanks
	for (int i = 0; i < m_numExistingAllyTanks; i++)
	{
		if (m_allyTanks[i] != nullptr)
		{
			delete m_allyTanks[i];
			m_allyTanks[i] = nullptr;
		}
	}
	delete m_allyTanks;

	//destructor
	m_entities.clear();
}
void Map::InitializeTileDefinitions()
{

	s_tileDefinitions[TILE_GRASS].m_texture = Texture::CreateOrGetTexture("Data/Images/tileable_grass_00.png");
	s_tileDefinitions[TILE_GRASS].m_isSolid = false;

	s_tileDefinitions[TILE_ROCK].m_texture = Texture::CreateOrGetTexture("Data/Images/tileable_rock_00.png");
	s_tileDefinitions[TILE_ROCK].m_isSolid = true;
}
void Map::InitializeExplosionDefinitions()
{
	m_explosions.resize(3);
	m_explosions[0] = Explosion(); //small
	m_explosions[0].setRadius(0.5f);
	m_explosions[1] = Explosion(); //medium
	m_explosions[1].setRadius(1.f);
	m_explosions[2] = Explosion(); //large
	m_explosions[2].setRadius(2.f);
}


void Map::setUpPlayer(Tank* player)
{
	//player
	m_player = player;

	//comment out the four lines below if you do not want a random start position for the player.
	m_aAbb = AABB2(m_playerStartPosition, Vector2(m_playerStartPosition.x + 1.f, m_playerStartPosition.y + 1.f));
	m_playerStartPosition.x += 0.5f;
	m_playerStartPosition.y += 0.5f;
	m_player->SetPosition(m_playerStartPosition);

	addEntity((Entity*)m_player);

	g_positionPlayerLastAlive = m_player->GetPosition();
	m_player->assignController(g_controllers[0]);
}
void Map::randomGenerateMap()
{
	//m_tiles
	for (int i = 0; i < m_size.y; i++)
	{
		int ySize = i * m_size.x;
		for (int j = 0; j < m_size.x; j++)
		{
			if (i == 0 || j == 0 || i == (m_size.y - 1) || j == (m_size.x - 1) || (rand() % 10 == 0))
			{
				m_tiles[ySize + j].setByTileType(TILE_ROCK);
			}
			else
			{
				m_tiles[ySize + j].setByTileType(TILE_GRASS);
			}
			m_tiles[ySize + j].setWorldCoords(Vector2((float)j, (float)i));

			if (s_tileDefinitions[m_tiles[ySize + j].GetTileType()].m_isSolid == false)
			{
				unsolidTileLocs.insert(unsolidTileLocs.begin() + unsolidTileLocs.size(), TileCoords(j, i));
			}
		}
	}
	m_playerStartPosition = GetRandomNoneOccupiedCoords();
}
void Map::loadMapFromImage(std::string mapFileLocation)
{
	//const Rgba GrassTileC = Rgba(0.f, 1.f, 0.f, 1.f); //technically this tile will always be the default
	//const Rgba RockTileC = Rgba(96.f/255.f, 57.f/255.f, 19.f/255.f, 1.f);
	//const Rgba TurretC = Rgba(1.f,0.f,0.f,1.f);
	//const Rgba EnemyTankC = Rgba(169.f/255.f, 24.f/255.f, 24.f/255.f, 1.f);
	//const Rgba playerC = Rgba(2.f / 255.f, 36.f / 255.f, 1.f, 1.f);
	//const float divider = 255.f;

	IntVector2 m_texelSize;
	int numComponents = 0; // Filled in for us to indicate how many color/alpha components the image had (e.g. 3=RGB, 4=RGBA)
	int numComponentsRequested = 0; // don't care; we support 3 (RGB) or 4 (RGBA)
	unsigned char* imageData = stbi_load(mapFileLocation.c_str(), &m_texelSize.x, &m_texelSize.y, &numComponents, numComponentsRequested);

	m_playerStartPosition = Vector2(-1.f, -1.f);
	std::vector<Vector2> turretPositions;
	std::vector<Vector2> EnemyTankPositions;
	std::vector<Vector2> AllyTankPositions;
	m_size.x = m_texelSize.x;
	m_size.y = m_texelSize.y;
	m_tiles.resize(m_texelSize.x * m_texelSize.y);

	for (int i = 0; i < m_texelSize.y; i++)
	{
		int ySize = (i*m_size.x);
		for (int j = 0; j < m_texelSize.x; j++)
		{
			int redByteIndex = (ySize + j) * numComponents;
			int greenByteIndex = redByteIndex + 1;
			int blueByteIndex = redByteIndex + 2;

			unsigned char RedByte = imageData[redByteIndex]; //int from [0, 255]
			unsigned char GreenByte = imageData[greenByteIndex];
			unsigned char BlueByte = imageData[blueByteIndex];

			m_tiles[ySize + j].setWorldCoords(Vector2((float)j, (float)i));
			TileType presentTileType = GetTileType(RedByte, GreenByte, BlueByte);
			
			if (i == 0 || j == 0 || j == m_texelSize.x - 1 || i == m_texelSize.y - 1)
			{
				m_tiles[ySize + j].setByTileType(TILE_ROCK);
			} 
			else if (presentTileType != TILE_INVALID)
			{
				m_tiles[ySize + j].setByTileType(presentTileType);
			}
			else
			{
				m_tiles[ySize + j].setByTileType(TILE_GRASS);
			}
			EntityType presentEntityType = GetEntityType(RedByte, GreenByte, BlueByte);
			
			if (presentEntityType == ENTITY_PLAYER)
			{
				m_playerStartPosition = Vector2((float)j + 0.5f, (float)i + 0.5f);
			}
			else if (presentEntityType == ENTITY_ENEMY_TANK)
			{
				EnemyTankPositions.insert(EnemyTankPositions.begin() + EnemyTankPositions.size(), Vector2((float)j + 0.5f, (float)i + 0.5f));
			}
			else if (presentEntityType == ENTITY_TURRET)
			{
				turretPositions.insert(turretPositions.begin() + turretPositions.size(), Vector2((float)j + 0.5f, (float)i + 0.5f));
			}
			else if (presentEntityType == ENTITY_ALLY_TANK)
			{
				AllyTankPositions.insert(AllyTankPositions.begin() + AllyTankPositions.size(), Vector2((float)j + 0.5f, (float)i + 0.5f));
			}
		}
	}
	m_maxTurrets = turretPositions.size();
	m_turrets = new EnemyTurret*[m_maxTurrets];
	for (size_t i = 0; i < turretPositions.size(); i++)
	{
		GenerateTurret();
		m_turrets[i]->SetPosition(turretPositions[i]);
	}
	
	m_maxEnemyTanks = EnemyTankPositions.size();
	m_enemyTanks = new EnemyTank*[m_maxEnemyTanks];
	for (size_t i = 0; i < EnemyTankPositions.size(); i++)
	{
		GenerateEnemyTank();
		m_enemyTanks[i]->SetPosition(EnemyTankPositions[i]);
	}

	m_maxAllyTanks = AllyTankPositions.size();
	m_allyTanks = new AllyTank*[m_maxAllyTanks];
	for (size_t i = 0; i < AllyTankPositions.size(); i++)
	{
		GenerateAllyTank();
		m_allyTanks[i]->SetPosition(AllyTankPositions[i]);
	}
	if (m_playerStartPosition.x == -1.f || m_playerStartPosition.y == -1.f)
	{
		m_playerStartPosition = GetRandomNoneOccupiedCoords();
	}
	stbi_image_free(imageData);
	m_gameLoading = false;
}

TileType Map::GetTileType(unsigned char red, unsigned char green, unsigned char blue)
{
	if (red == 96 && green == 57 && blue == 19)
	{
		return TILE_ROCK;
	}
	return TILE_GRASS;
}

EntityType Map::GetEntityType(unsigned char red, unsigned char green, unsigned char blue)
{
	if (red == 255 && green == 0 && blue == 0)
	{
		return ENTITY_TURRET;
	}
	else if (red == 169 && green == 24 && blue == 24)
	{
		return ENTITY_ENEMY_TANK;
	}
	else if (red == 2 && green == 36 && blue == 255)
	{
		return ENTITY_PLAYER;
	}
	else if (red == 2 && green == 192 && blue == 255)
	{
		return ENTITY_ALLY_TANK;
	}
	return ENTITY_INVALID;
}


//getters
TileCoords Map::GetTileCoordsForTile(const Tile& tile) const
{
	//Returns the Tilecoordinates for a given tile.
	Vector2 worldCoord = ((Tile &)(tile)).getWorldCoords();
	int x = (int)floor((int)worldCoord.x);
	int y = (int)floor((int)worldCoord.y);
	return TileCoords(x,y);
}
Tile* Map::GetTileAtCoords(const TileCoords& tileCoords)
{
	//returns the tile at a given set of Tilecoordinates, unless it does not exist
	if (tileCoords.x >= 0 && tileCoords.x < m_size.x && tileCoords.y >= 0 && tileCoords.y < m_size.y)
	{
		return &m_tiles[(tileCoords.y) + tileCoords.x];
	}
	return nullptr;
}
Tile* Map::GetTileAtIndex(int tileIndex)
{
	//Gets a tile by tile index, unless it does not exist
	if (tileIndex < m_size.x * m_size.y && tileIndex >= 0)
	{
		return &m_tiles[tileIndex];
	}
	return nullptr;
}
Tile* Map::GetTileAtWorldCoords(const Vector2& worldCoordinates)
{
	//gets a tile based on it's world coordinates, unless it does not exist
	float x = worldCoordinates.x;
	float y = worldCoordinates.y ;
	int realX = (int)floor((int)x);
	int realY = (int)floor((int)y);
	if (realX < m_size.x && realY < m_size.y)
	{
		return &m_tiles[(realY * m_size.x) + realX];
	}

	return nullptr;
}

Vector2 Map::GetWorldCoordsForTileCoords(const TileCoords& tileCoords)
{
	return Vector2((float)tileCoords.x, (float)tileCoords.y);
}
TileCoords Map::GetTileCoordsForWorldCoords(const Vector2& WorldCoords)
{
	//returns tilecoordinates based on world coordinates
	return TileCoords((int)floor(WorldCoords.x), (int)floor(WorldCoords.y));
}
TileCoords Map::GetTCforIndex(int index)
{
	//gets tile coordinates based on index.
	int tileY = (int)floor(index / m_size.x);
	int tileX = index % m_size.x;
	return (TileCoords(tileX, tileY));
}
int Map::GetIndexForTileCoords(TileCoords tileCoords)
{
	//returns the tile index for a tile, based on the tile's coordinates
	return ((m_size.x * tileCoords.y) + tileCoords.x);
}
bool Map::IsTileSolid(TileCoords tileLoc)
{
	if (tileLoc.x < m_size.x && tileLoc.x >= 0 && tileLoc.y < m_size.y && tileLoc.y >= 0)
	{
		return s_tileDefinitions[m_tiles[(tileLoc.y * m_size.x) + tileLoc.x].GetTileType()].m_isSolid;
	}
	return false;
}

/*TileCoords Map::HasLineOfSight(Vector2 start, Vector2 end, bool& hasLineOfSight)
{
	const int numSteps = 100;
	//Uses raycasting to check if theres a solid object in the way of start and end.
	Vector2 difference = end - start;
	Vector2 current = start;
	Vector2 step = difference * (1.f / (float) numSteps);
	for (int i = 0; i < numSteps; ++ i )
	{
		Tile* tileAtCurrent = GetTileAtWorldCoords(current);
		if (tileAtCurrent != nullptr && s_tileDefinitions[tileAtCurrent->GetTileType()].m_isSolid == true)
		{
			hasLineOfSight = false;
			return GetTileCoordsForWorldCoords(current);
		}
		current += step;
	}
	hasLineOfSight = true;
	return TileCoords(-1,-1);
}*/
TileCoords Map::FastHasLineOfSight(Vector2 start, Vector2 end, bool& hasLineOfSight)
{
	TileCoords tStart = GetTileCoordsForWorldCoords(start);
	if (IsTileSolid(tStart) == true)
	{
		hasLineOfSight = false;
		return tStart;
	}
	TileCoords tEnd = GetTileCoordsForWorldCoords(end);
	Vector2 rayDisplacement = end - start;
	Vector2 tDelta = Vector2(0.f, 0.f);
	IntVector2 tileStep = IntVector2(0,0);
	//x
	tDelta.x = 1.f / rayDisplacement.x;
	tileStep.x = 1;
	if (rayDisplacement.x < 0.f)
	{
		tileStep.x = -1;
	}
	IntVector2 offsetToLeadeingEdge = IntVector2(0, 0);
	offsetToLeadeingEdge.x = (tileStep.x + 1) / 2;
	Vector2 firstVerticalIntersection = Vector2(0.f, 0.f);
	firstVerticalIntersection.x = (float)(tStart.x + offsetToLeadeingEdge.x);
	Vector2 tOfNextCrossing = Vector2(0.f, 0.f);
	tOfNextCrossing.x = abs(firstVerticalIntersection.x - start.x) * tDelta.x;

	//y
	tDelta.y = 1.f / rayDisplacement.y;
	tileStep.y = 1;
	if (rayDisplacement.y < 0.f)
	{
		tileStep.y = -1;
	}
	offsetToLeadeingEdge.y = (tileStep.y + 1) / 2;
	firstVerticalIntersection.y = (float)(tStart.y + offsetToLeadeingEdge.y);
	tOfNextCrossing.y = abs(firstVerticalIntersection.y - start.y) * tDelta.y;

	TileCoords tCurrent = tStart;
	while (tCurrent != tEnd)
	{
		if (abs(tOfNextCrossing.x) < abs(tOfNextCrossing.y))
		{
			if (abs(tOfNextCrossing.x) > 1.f)
			{
				break;
			}
			tCurrent.x += tileStep.x;
			if (IsTileSolid(tCurrent) == true)
			{
				hasLineOfSight = false;
				return tCurrent;
			}
			tOfNextCrossing.x += tDelta.x;
		}
		else
		{
			if (abs(tOfNextCrossing.y) > 1.f)
			{
				break;
			}
			tCurrent.y += tileStep.y;
			if (IsTileSolid(tCurrent) == true)
			{
				hasLineOfSight = false;
				return tCurrent;
			}
			tOfNextCrossing.y += tDelta.y;
		}
	}
	hasLineOfSight = true;
	return TileCoords(-1, -1);
}

//setters
void Map::changeTileType(TileCoords& tileLocation, TileType newType)
{
	m_tiles[(tileLocation.y * m_size.x) + tileLocation.x].setByTileType(newType);
}

//remover
void Map::removeEntity(Entity* entityPointer)
{
	//removes an entity pointer.
	std::set<Entity*>::iterator it = m_entities.find(entityPointer);
	m_entities.erase(it);
}

//adders
void Map::addEntity(Entity* newEntity)
{
	//adds entities to the map's registry so it can correct any collisions with solid objects
	m_entities.insert(newEntity);
}

//randomGeneration
Vector2 Map::GetRandomNoneOccupiedCoords()
{
	//returns a Vector2 of a random unoccupied tile.
	std::vector<TileCoords> tempUnOccupied = unsolidTileLocs;
	std::set<Entity*> tempEntities = m_entities;
	for (int i = 0; (size_t)i < tempUnOccupied.size(); i++)
	{
		for (std::set<Entity*>::iterator it = tempEntities.begin(); it != tempEntities.end(); ++it)
		{
			Entity* entity = *it;
			TileCoords entityLoc = GetTileCoordsForWorldCoords(entity->GetPosition());
			if (tempUnOccupied[i].x == entityLoc.x && tempUnOccupied[i].y == entityLoc.y)
			{
				tempEntities.erase(it);
				tempUnOccupied.erase(tempUnOccupied.begin() + i);
				break;
			}
		}
	}
	if (tempUnOccupied.size() <= 0)
	{
		return Vector2(-1, -1);
	}
	else
	{
		int randomIndex = rand() % tempUnOccupied.size();
		return GetWorldCoordsForTileCoords(tempUnOccupied[randomIndex]);
	}
}

//render
const void Map::Render()
{
	//This method renders all the tiles as viewable.
	//Renders all the tiles within the map.

	if (m_gameLoading == false)
	{
		MapRender(true, Vector2(1.f,1.f));
		EntityRender(true, Vector2(1.f, 1.f));

		MiniMapRender();
		if (m_player == nullptr)
		{
			g_Renderer->SetOrtho(Vector2(0.f, 0.f), Vector2(16.f, 9.f));
			g_Renderer->DrawAABB(AABB2(Vector2(0.f, 0.f), Vector2(16.f, 9.f)), m_deathScreenColor);
		}
	}
}
const void Map::MapRender(bool fogOfWarActive,Vector2 scale)
{
	for (int i = 0; i < (m_size.y * m_size.x); i++)
	{
		Vector2 presentTilePosition = m_tiles[i].getWorldCoords();
		if (fogOfWarActive == false 
			|| (fogOfWarActive == true && m_fogOfWarActive == false && (g_positionPlayerLastAlive - presentTilePosition).GetLength() <= (m_normalAmountToRenderFromPlayer))
			|| (fogOfWarActive == true && m_fogOfWarActive == true && (g_positionPlayerLastAlive - presentTilePosition).GetLength() <= (m_fogOfWar +0.7f)))
		{
			Vector2 maxTilePosition = Vector2(presentTilePosition.x + 1.f, presentTilePosition.y + 1.f);
			presentTilePosition = Vector2(presentTilePosition.x * scale.x, presentTilePosition.y * scale.y);
			maxTilePosition = Vector2(maxTilePosition.x * scale.x, maxTilePosition.y * scale.y);
			bool tileSolid = s_tileDefinitions[m_tiles[i].GetTileType()].m_isSolid;
			AABB2 TileBounds = AABB2(presentTilePosition, maxTilePosition);

			Rgba color = g_Renderer->White;
			Texture* tileTexture = s_tileDefinitions[m_tiles[i].GetTileType()].m_texture;
			if ((tileTexture == nullptr))
			{
				color = g_Renderer->Blue;
				if (tileSolid == true)
				{
					color = Rgba(1.f, 1.f, 0.f, 1.f);
				}
				g_Renderer->DrawAABB(TileBounds, color);
			}
			else
			{
				g_Renderer->DrawTexturedAABB(TileBounds, tileTexture, Vector2(0.f, 0.f), Vector2(1.f, 1.f), color);
			}
		}
	}
}
const void Map::EntityRender(bool fogOfWarActive, Vector2 scale)
{
	//Rendering for all entities.
	if (fogOfWarActive == false
		|| (fogOfWarActive == true && m_fogOfWarActive == false && (g_positionPlayerLastAlive - m_aAbb.mins).GetLength() <= (m_normalAmountToRenderFromPlayer))
		|| (fogOfWarActive == true && m_fogOfWarActive == true && (g_positionPlayerLastAlive - m_aAbb.mins).GetLength() <= (m_fogOfWar + 0.7f)))
	{
		g_Renderer->DrawTexturedAABB(m_aAbb, m_testTexture, Vector2(0, 0), Vector2(1, 1), Rgba(1.f, 1.f, 1.f, 1.f));
	}
	
	

	//turrets
	genericEntitiesRender((Entity**)m_turrets, m_numExistingTurrets, fogOfWarActive, scale);
	//enemyTanks
	genericEntitiesRender((Entity**)m_enemyTanks, m_numExistingEnemyTanks, fogOfWarActive, scale);
	//allyTanks
	genericEntitiesRender((Entity**)m_allyTanks, m_numExistingAllyTanks, fogOfWarActive, scale);

	//player
	if (m_player != nullptr)
	{
		m_player->Render(scale);
	}
	//bullets
	//can't use genericEntitiesRender cause bullets are running on a vector
	for (int i = 0; i < m_numExistingBullets; i++)
	{
		if (m_bullets[i] != nullptr && (fogOfWarActive == false
			|| (fogOfWarActive == true && m_fogOfWarActive == false && (g_positionPlayerLastAlive - m_bullets[i]->GetPosition()).GetLength() <= (m_normalAmountToRenderFromPlayer))
			|| (fogOfWarActive == true && m_fogOfWarActive == true && (g_positionPlayerLastAlive - m_bullets[i]->GetPosition()).GetLength() <= (m_fogOfWar + 0.7f))))
		{
			m_bullets[i]->Render(scale);
		}
	}

	for (size_t i = 0; i < m_activeExplosions.size(); i++)
	{
		if (fogOfWarActive == false || (fogOfWarActive == false
			|| (fogOfWarActive == true && m_fogOfWarActive == false && (g_positionPlayerLastAlive - m_activeExplosions[i].getPosition()).GetLength() <= (m_normalAmountToRenderFromPlayer))
			|| (fogOfWarActive == true && m_fogOfWarActive == true && (g_positionPlayerLastAlive - m_activeExplosions[i].getPosition()).GetLength() <= (m_fogOfWar + 0.7f))))
		{
			m_activeExplosions[i].Render(scale);
		}
	}
}
const void Map::MiniMapRender()
{
	g_Renderer->SetOrtho(m_bottomLeftValForOrthoMiniMap, Vector2(0.f, 0.f));
	g_Renderer->Translate(Vector2(m_bottomLeftValForOrthoMiniMap.x - 1.f, m_bottomLeftValForOrthoMiniMap.y - 1.f));
	g_Renderer->DrawAABB(AABB2(m_bottomLeftValForOrthoMiniMap/3.f, Vector2(0.f, 0.f)), Rgba(0.f, 0.f, 0.f, 0.5f));
	Vector2 scale = Vector2(1.f / m_size.x, 1.f / m_size.y);
	MapRender(m_miniMapFogOfWarActive, scale);
	EntityRender(m_miniMapFogOfWarActive, scale);
}
const void Map::genericEntitiesRender(Entity** entities, const int& numExistingEntities, bool fogOfWarActive, Vector2 scale)
{
	for (int i = 0; i < numExistingEntities; i++)
	{
		if (entities[i] != nullptr && (fogOfWarActive == false
			|| (fogOfWarActive == true && m_fogOfWarActive == false && (g_positionPlayerLastAlive - entities[i]->GetPosition()).GetLength() <= (m_normalAmountToRenderFromPlayer))
			|| (fogOfWarActive == true && m_fogOfWarActive == true && (g_positionPlayerLastAlive - entities[i]->GetPosition()).GetLength() <= (m_fogOfWar + 0.7f))))
		{
			entities[i]->Render(scale);
		}
	}
}


//death and collision
const void Map::CheckForDeath()
{
	//checks if something's suppose to die, if so, deletes it.
	for (int i = 0; i < m_numExistingBullets; i++)
	{
		if (m_bullets[i]->GetDead() == true)
		{
			GenerateExplosion(m_bullets[i]->GetPosition(), 0);
			if ((g_positionPlayerLastAlive - m_bullets[i]->GetPosition()).GetLength() <= m_tileDistanceFromPlayerInOrderToPlaySound)
			{
				m_explosionsound_audio = m_audio->PlaySound(m_explosionsoundID, 0.5f);
			}
			//map collision detection
			deleteBullet(i);
			i--;
		}
	}
	for (int i = 0; i < m_numExistingTurrets; i++)
	{
		if (m_turrets[i]->GetDead() == true)
		{
			GenerateExplosion(m_turrets[i]->GetPosition(), 1);
			if ((g_positionPlayerLastAlive - m_turrets[i]->GetPosition()).GetLength() <= m_tileDistanceFromPlayerInOrderToPlaySound)
			{
				m_explosionsound_audio = m_audio->PlaySound(m_explosionsoundID, 0.7f);
			}
			deleteTurret(i);
			i--;
		}
	}
	for (int i = 0; i < m_numExistingEnemyTanks; i++)
	{
		if (m_enemyTanks[i]->GetDead() == true)
		{
			GenerateExplosion(m_enemyTanks[i]->GetPosition(), 1);
			if ((g_positionPlayerLastAlive - m_enemyTanks[i]->GetPosition()).GetLength() <= m_tileDistanceFromPlayerInOrderToPlaySound)
			{
				m_explosionsound_audio = m_audio->PlaySound(m_explosionsoundID, 0.7f);
			}
			deleteEnemyTank(i);
			i--;
		}
	}
	for (int i = 0; i < m_numExistingAllyTanks; i++)
	{
		if (m_allyTanks[i]->GetDead() == true)
		{
			GenerateExplosion(m_allyTanks[i]->GetPosition(), 1);
			if ((g_positionPlayerLastAlive - m_allyTanks[i]->GetPosition()).GetLength() <= m_tileDistanceFromPlayerInOrderToPlaySound)
			{
				m_explosionsound_audio = m_audio->PlaySound(m_explosionsoundID, 0.7f);
			}
			deleteAllyTank(i);
			i--;
		}
	}

	if (m_player != nullptr && m_player->GetDieing() == true)
	{
		GenerateExplosion(m_player->GetPosition(), 2);
		m_explosionsound_audio = m_audio->PlaySound(m_explosionsoundID, 1.f);
		removeEntity(m_player);
		delete m_player;
		m_player = nullptr;
	}
}
//bullet Collision Detection with Entities
int Map::checkBulletCollideWithEntities(Entity** entities, int& numOfEntities, Bullet* currentBullet)
{
	//checks with any entity arrays plugged into it for whether or not the current bullet collides with any of them.
	if (currentBullet == nullptr)
	{
		return -1;
	}
	//bullet information
	Vector2 bulletPosition = currentBullet->GetPosition();
	int bulletAlliance = currentBullet->getAlliance();
	float bulletCollisionRadius = currentBullet->getCollisionRadius();
	for (int i = 0; i < numOfEntities; i++)
	{
		if (entities[i] != nullptr)
		{
			//entity information (position, alliance, and collision radius)
			Vector2 entityPosition = entities[i]->GetPosition();
			int entityAlliance = entities[i]->getAlliance();
			float entityCollisionRadius = entities[i]->getCollisionRadius();

			//begin calculations
			Vector2 positionDiff = bulletPosition - entityPosition;
			float length = positionDiff.GetLength();
			float collisionAddition = bulletCollisionRadius + entityCollisionRadius;
			if (entityAlliance != bulletAlliance && length <= collisionAddition)
			{
				return i;
			}
		}
	}
	return -1;
}
bool Map::checkBulletAndPlayerCollid(Bullet* currentBullet)
{
	//checks for collision between bullet and the player
	if (m_player == nullptr)
	{
		return false;
	}
	//bullet information
	Vector2 bulletPosition = currentBullet->GetPosition();
	int bulletAlliance = currentBullet->getAlliance();
	float bulletCollisionRadius = currentBullet->getCollisionRadius();

	//player information
	Vector2 playerPosition = m_player->GetPosition();
	int playerAlliance = m_player->getAlliance();
	float playerCollisionRadius = m_player->getCollisionRadius();

	//begin calculations
	Vector2 positionDiff = bulletPosition - playerPosition;
	float length = positionDiff.GetLength();
	float collisionAddition = bulletCollisionRadius + playerCollisionRadius;

	if (playerAlliance != bulletAlliance && length <= collisionAddition)
	{
		return true;
	}
	return false;
}

void Map::checkShoot()
{
	//checks for all AI controlled entities whether or not they want to shoot, if they want to shoot, then shoot and reset their timer.
	checkShootEnemyEntities((EnemyEntity**) m_turrets, m_numExistingTurrets);

	checkShootEnemyEntities((EnemyEntity**)m_enemyTanks, m_numExistingEnemyTanks);

	checkShootEnemyEntities((EnemyEntity**)m_allyTanks, m_numExistingAllyTanks);
}


void Map::checkShootEnemyEntities(EnemyEntity** entities, const int& numOfEntities)
{
	for (int i = 0; i < numOfEntities; i++)
	{
		bool wantsToShoot = entities[i]->getShoot();
		if (wantsToShoot == true)
		{
			if ((g_positionPlayerLastAlive - entities[i]->GetPosition()).GetLength() <= m_tileDistanceFromPlayerInOrderToPlaySound)
			{
				m_gunshot_audio = m_audio->PlaySound(m_gunshotID, 0.5f);
			}
			GenerateBullet((Entity*)m_turrets[i]);
			entities[i]->resetShoot();
		}
	}
}


Vector2 Map::calculateNearestFromEntities(Vector2 originPosition, Entity** entities, int numOfEntities)
{
	Vector2 nearestPositionToOrigin = Vector2(-1.f, -1.f);
	Vector2 differenceFromOrigin = Vector2(-1.f, -1.f);

	for (int i = 0; i < numOfEntities; i++)
	{
		if (entities[i] != nullptr)
		{
			Vector2 position = entities[i]->GetPosition();
			Vector2 difference = originPosition - position;
			if ((nearestPositionToOrigin.x == -1 || nearestPositionToOrigin.y == -1) || differenceFromOrigin.GetLength() > difference.GetLength())
			{
				nearestPositionToOrigin = position;
				differenceFromOrigin = difference;
			}
		}
	}
	return nearestPositionToOrigin;
}

void Map::checkCollisionsBetweenAllEntities()
{
	for (int i = 0; i < m_numExistingEnemyTanks; i++)
	{
		if (m_enemyTanks[i] != nullptr)
		{
			checkCollisionBetweenEntityAndEntityGroup((Entity*)m_enemyTanks[i], (EnemyEntity**)m_enemyTanks, m_numExistingEnemyTanks, false);

			checkCollisionBetweenEntityAndEntityGroup((Entity*)m_enemyTanks[i], (EnemyEntity**)m_allyTanks, m_numExistingAllyTanks, false);

			checkCollisionBetweenEntityAndEntityGroup((Entity*)m_enemyTanks[i], (EnemyEntity**)m_turrets, m_numExistingTurrets, true);
		}
	}
	//first, enemyTank vs enemyTank
	for (int i = 0; i < m_numExistingAllyTanks; i++)
	{
		if (m_allyTanks[i] != nullptr)
		{
			checkCollisionBetweenEntityAndEntityGroup((Entity*)m_allyTanks[i], (EnemyEntity**)m_enemyTanks, m_numExistingEnemyTanks, false);

			checkCollisionBetweenEntityAndEntityGroup((Entity*)m_allyTanks[i], (EnemyEntity**)m_allyTanks, m_numExistingAllyTanks, false);

			checkCollisionBetweenEntityAndEntityGroup((Entity*)m_allyTanks[i], (EnemyEntity**)m_turrets, m_numExistingTurrets, true);
		}
	}

	//fourth player vs enemyTank
	if (m_player != nullptr)
	{
		checkCollisionBetweenEntityAndEntityGroup((Entity*)m_player, (EnemyEntity**)m_enemyTanks, m_numExistingEnemyTanks, false);

		checkCollisionBetweenEntityAndEntityGroup((Entity*)m_player, (EnemyEntity**)m_allyTanks, m_numExistingAllyTanks, false);

		checkCollisionBetweenEntityAndEntityGroup((Entity*)m_player, (EnemyEntity**)m_turrets, m_numExistingTurrets, true);
	}
	
}

void Map::checkCollisionBetweenEntityAndEntityGroup(Entity* entity, EnemyEntity** entities, const int& numberOfEntities, bool comparingToUnmovableEntities)
{
	int index = checkCollisionBetweenEntityAndEntities(entity, (Entity**)entities, numberOfEntities);
	if (index != -1 && comparingToUnmovableEntities == false)
	{
		pushEntitiesOutOfEachOther((Entity*&)entities[index], (Entity*&)entity);
	}
	else if (index != -1 && comparingToUnmovableEntities == true)
	{
		pushEntityOutOfFixedEntity((Entity*&)entity, (Entity*&)entities[index]);
	}
}


void Map::pushEntitiesOutOfEachOther(Entity*& a, Entity*& b)
{
	Vector2 aPosition = a->GetPosition();
	Vector2 bPosition = b->GetPosition();
	float combinedRadius = a->getCollisionRadius() + b->getCollisionRadius();
	Vector2 displacement = bPosition - aPosition;
	Vector2 displacementNonNormal = displacement;
	displacement.normalize();
	displacement = (displacement* combinedRadius);
	displacementNonNormal = displacementNonNormal - displacement;
	displacementNonNormal = displacementNonNormal * -1.f;
	displacement = displacementNonNormal * 0.5f;
	b->SetPosition(bPosition + displacement);
	a->SetPosition(aPosition - displacement);
}
void Map::pushEntityOutOfFixedEntity(Entity*& mobileEnt, Entity*& stationary)
{
	Vector2 aPosition = stationary->GetPosition();
	Vector2 bPosition = mobileEnt->GetPosition();
	float combinedRadius = stationary->getCollisionRadius() + mobileEnt->getCollisionRadius();
	Vector2 displacement = bPosition - aPosition;
	Vector2 displacementNonNormal = displacement;
	displacement.normalize();
	displacement = (displacement* combinedRadius);
	displacementNonNormal = displacementNonNormal - displacement;
	displacementNonNormal = displacementNonNormal * -1.f;
	displacement = displacementNonNormal;
	mobileEnt->SetPosition(bPosition + displacement);
}

int Map::checkCollisionBetweenEntityAndEntities(Entity* thisEntity, Entity** entities, int numOfEntities)
{
	if (thisEntity != nullptr)
	{
		Vector2 thisEntityPosition = thisEntity->GetPosition();
		float thisEntityCollisionRadius = thisEntity->getCollisionRadius();
		for (int i = 0; i < numOfEntities; i++)
		{
			if (entities[i] != nullptr && thisEntity != entities[i])
			{
				Vector2 entitiesPostion = entities[i]->GetPosition();
				float entitiesCollisionRadius = entities[i]->getCollisionRadius();
				Vector2 differenceBetweenThisEntityAndOther = thisEntityPosition - entitiesPostion;
				float addedCollisions = thisEntityCollisionRadius + entitiesCollisionRadius;
				float lengthOfDifference = differenceBetweenThisEntityAndOther.GetLength();
				if (lengthOfDifference < addedCollisions)
				{
					return i;
				}
			}
		}
	}

	return -1;
}

//destroy
void Map::deleteTurret(int index)
{
	//deletes a turret and removes it's existance from the game
	if (m_turrets[index] != nullptr)
	{
		removeEntity((Entity*)m_turrets[index]);
		delete m_turrets[index];
		m_turrets[index] = nullptr;
		m_turrets[index] = m_turrets[m_numExistingTurrets - 1];
		m_turrets[m_numExistingTurrets - 1] = nullptr;
		m_numExistingTurrets--;
	}
}

void Map::deleteBullet(int index)
{
	//deletes the bullet at index.
	if (m_bullets[index] != nullptr)
	{
		removeEntity((Entity*)m_bullets[index]);
		Bullet* temp =  m_bullets[index];
		m_bullets[index] = nullptr;
		m_bullets[index] = m_bullets[m_numExistingBullets - 1];
		m_bullets[m_numExistingBullets - 1] = nullptr;
		m_numExistingBullets--;

		delete temp;
	}
}
void Map::deleteEnemyTank(int index)
{
	//deletes a turret and removes it's existance from the game
	if (m_enemyTanks[index] != nullptr)
	{
		removeEntity((Entity*)m_enemyTanks[index]);
		delete m_enemyTanks[index];
		m_enemyTanks[index] = nullptr;
		m_enemyTanks[index] = m_enemyTanks[m_numExistingEnemyTanks - 1];
		m_enemyTanks[m_numExistingEnemyTanks - 1] = nullptr;
		m_numExistingEnemyTanks--;
	}
}
void Map::deleteAllyTank(int index)
{
	//deletes a turret and removes it's existance from the game
	if (m_allyTanks[index] != nullptr)
	{
		removeEntity((Entity*)m_allyTanks[index]);
		delete m_allyTanks[index];
		m_allyTanks[index] = nullptr;
		m_allyTanks[index] = m_allyTanks[m_numExistingAllyTanks - 1];
		m_allyTanks[m_numExistingAllyTanks - 1] = nullptr;
		m_numExistingAllyTanks--;
	}
}

//generators

void Map::GenerateBullet(Entity* spawningFrom)
{
	//generates a bullet based on information provided by Entity* spawningFrom
	if (m_numExistingBullets < m_maxBullets)
	{
		m_bullets[m_numExistingBullets] = new Bullet(spawningFrom);
		addEntity(m_bullets[m_numExistingBullets]);
		m_numExistingBullets++;
	}
}
void Map::GenerateTurret()
{
	//generates a turret at a random unoccupied location
	if (m_numExistingTurrets < m_maxTurrets)
	{
		Vector2 location = GetRandomNoneOccupiedCoords();
		location.x += 0.5f;
		location.y += 0.5f;
		m_turrets[m_numExistingTurrets] = new EnemyTurret(location);
		addEntity(m_turrets[m_numExistingTurrets]);
		m_numExistingTurrets++;
	}
}
void Map::GenerateEnemyTank()
{
	//generates an enemy tank at a random unoccupied location
	if (m_numExistingEnemyTanks < m_maxEnemyTanks)
	{
		Vector2 location = GetRandomNoneOccupiedCoords();
		location.x += 0.5f;
		location.y += 0.5f;
		m_enemyTanks[m_numExistingEnemyTanks] = new EnemyTank(location);
		addEntity(m_enemyTanks[m_numExistingEnemyTanks]);
		m_numExistingEnemyTanks++;
	}
}

void Map::GenerateAllyTank()
{
	//generates an enemy tank at a random unoccupied location
	if (m_numExistingAllyTanks < m_maxAllyTanks)
	{
		Vector2 location = GetRandomNoneOccupiedCoords();
		location.x += 0.5f;
		location.y += 0.5f;
		m_allyTanks[m_numExistingAllyTanks] = new AllyTank(location);
		addEntity(m_allyTanks[m_numExistingAllyTanks]);
		m_numExistingAllyTanks++;
	}
}

void Map::GenerateExplosion(Vector2 position, int index)
{
	Explosion explode = Explosion(m_explosions[index]);
	explode.SetPosition(position);
	m_activeExplosions.insert(m_activeExplosions.begin() + m_activeExplosions.size(), explode);
}

//Update
const void Map::Update(float deltaSeconds)
{
	//this method updates all entities within the game

	if (m_isFReleased == true && g_theApp->isKeyDown('F') == true)
	{
		m_isFReleased = false;
		m_fogOfWarActive = !m_fogOfWarActive;
	}
	if (m_isRReleased == true && g_theApp->isKeyDown('R') == true)
	{
		m_isRReleased = false;
		m_miniMapFogOfWarActive = !m_miniMapFogOfWarActive;
	}

	UpdateEntities(deltaSeconds);
	checkShoot();

	checkCollisionsBetweenAllEntities();
	//map
	Update();

	UpdateBullets(deltaSeconds);
	CheckForDeath();
	//track space back or xbox right trigger down.
	if ((g_theApp->isKeyDown(' ') == false && g_controllers[0]->getRightTriggerPosition() == 0.f))
	{
		m_spaceIsReleased = true;
		m_spaceBarHeldAndSinceLastBulletFire = m_spaceBarHeldAndSinceLastBulletFire + 1.f;
	}
	if (g_theApp->isKeyDown('F') == false)
	{
		m_isFReleased = true;
	}
	if (g_theApp->isKeyDown('R') == false)
	{
		m_isRReleased = true;
	}
}
const void Map::UpdateEntities(float deltaSeconds)
{
	//track player death
	if (m_player == nullptr)
	{
		m_playerDeadForThisManySeconds += deltaSeconds;
		if (m_deathScreenColor.Alpha < 0.3f)
		{
			m_deathScreenColor.Alpha += m_amountOfAlphaToAddOverSeconds;
		}
	}

	if (g_theApp->isKeyDown('K') == true && m_player != nullptr)
	{
		m_player->hit(10);
		m_player->SetDeadTrue();
	}

	//updates every entity within the game.
	if (m_player != nullptr)
	{
		m_player->PlayerUpdate(deltaSeconds, g_theApp->isKeyDown('W'), g_theApp->isKeyDown('A'), g_theApp->isKeyDown('D'), g_theApp->isKeyDown('S'));
		g_positionPlayerLastAlive = m_player->GetPosition();
		g_playerLastAngularDisplacement = m_player->getAngularDisplacement();
	}

	//shooting
	if ((g_theApp->isKeyDown(' ') == true || g_controllers[0]->getRightTriggerPosition() > 0.f))
	{
		m_spaceBarHeldAndSinceLastBulletFire += deltaSeconds;
		if ((m_spaceBarHeldAndSinceLastBulletFire >= m_timeBetweenPlayerBullets))
		{
			m_gunshot_audio = m_audio->PlaySound(m_gunshotID, 0.5f);
			GenerateBullet((Entity*)m_player);
			m_spaceIsReleased = false;
			m_spaceBarHeldAndSinceLastBulletFire = 0.f;
		}
	}

	//camPosition.x = (WindowWidth / 2) - camPosition.x;
	//camPosition.y = (WindowHeight / 4) - camPosition.y;


	Vector2 playerPosition = g_positionPlayerLastAlive;
	//EnemyTurrets
	for (int i = 0; i < m_numExistingTurrets; i++)
	{
		if (m_turrets[i] != nullptr)
		{
			Vector2 turretPostion = m_turrets[i]->GetPosition();
			float turretLineOfSight = m_turrets[i]->getLineOfSight();
			Vector2 positionOfTarget = playerPosition;
			Vector2 nearestAllyTankPosition = calculateNearestFromEntities(turretPostion, (Entity**)m_allyTanks, m_numExistingAllyTanks);
			if ((nearestAllyTankPosition.x != -1 && nearestAllyTankPosition.y != -1) 
				&& (turretPostion - playerPosition).GetLength() >(turretPostion - nearestAllyTankPosition).GetLength())
			{
				positionOfTarget = nearestAllyTankPosition;
			}

			Vector2 difference = turretPostion - positionOfTarget;

			bool blocked = false;
			if (difference.GetLength() <= turretLineOfSight)
			{
				FastHasLineOfSight(turretPostion, positionOfTarget, blocked);
			}
			if (positionOfTarget == playerPosition && m_player == nullptr)
			{
				blocked = false;
			}
			m_turrets[i]->EnemyUpdate(deltaSeconds, positionOfTarget, blocked);
		}
	}
	//Enemy Tanks
	for (int i = 0; i < m_numExistingEnemyTanks; i++)
	{
		if (m_enemyTanks[i] != nullptr)
		{
			Vector2 enemyTankPostion = m_enemyTanks[i]->GetPosition();
			float enemyTankLineOfSight = m_enemyTanks[i]->getLineOfSight();
			Vector2 positionOfTarget = playerPosition;
			Vector2 nearestAllyTankPosition = calculateNearestFromEntities(enemyTankPostion, (Entity**)m_allyTanks, m_numExistingAllyTanks);
			if ((nearestAllyTankPosition.x != -1 && nearestAllyTankPosition.y != -1) 
				&& (enemyTankPostion - playerPosition).GetLength() >(enemyTankPostion - nearestAllyTankPosition).GetLength())
			{
				positionOfTarget = nearestAllyTankPosition;
			}

			Vector2 difference = enemyTankPostion - positionOfTarget;


			bool blocked = false;
			if (difference.GetLength() <= enemyTankLineOfSight)
			{
				FastHasLineOfSight(enemyTankPostion, positionOfTarget, blocked);
			}
			if (positionOfTarget == playerPosition && m_player == nullptr)
			{
				blocked = false;
			}
			m_enemyTanks[i]->EnemyUpdate(deltaSeconds, positionOfTarget, blocked);
		}
	}
	//Ally Tanks
	for (int i = 0; i < m_numExistingAllyTanks; i++)
	{
		if (m_allyTanks[i] != nullptr)
		{
			Vector2 allyTankPostion = m_allyTanks[i]->GetPosition();
			float allyTankLineOfSight = m_allyTanks[i]->getLineOfSight();
			Vector2 nearestEnemyTankPosition = calculateNearestFromEntities(allyTankPostion, (Entity**)m_enemyTanks, m_numExistingEnemyTanks);
			Vector2 nearestEnemyTurretPosition = calculateNearestFromEntities(allyTankPostion, (Entity**)m_turrets, m_numExistingTurrets);
			float nearestEnemyTankDistance = (allyTankPostion - nearestEnemyTankPosition).GetLength();
			float nearestEnemyTurretDistance = (allyTankPostion - nearestEnemyTurretPosition).GetLength();

			Vector2 positionOfTarget = nearestEnemyTankPosition;
			if ((nearestEnemyTankPosition.x == -1 || nearestEnemyTankPosition.y == -1) 
				|| (nearestEnemyTankDistance > nearestEnemyTurretDistance))
			{
				positionOfTarget = nearestEnemyTurretPosition;
			}
			Vector2 difference = allyTankPostion - positionOfTarget;

			bool blocked = false;
			if (difference.GetLength() <= allyTankLineOfSight)
			{
				FastHasLineOfSight(allyTankPostion, positionOfTarget, blocked);
			}
			m_allyTanks[i]->EnemyUpdate(deltaSeconds, positionOfTarget, blocked);
		}
	}

	//explosions
	for (size_t i = 0; i < m_activeExplosions.size(); i++)
	{
		m_activeExplosions[i].Update(deltaSeconds);
		if (m_activeExplosions[i].isDone() == true)
		{
			m_activeExplosions.erase(m_activeExplosions.begin() + i);
			i--;
		}
	}
}

const void Map::UpdateBullets(float deltaSeconds)
{
	//bullet collision with walls
	for (int i = 0; i < m_numExistingBullets; i++)
	{
		if (m_bullets[i] != nullptr)
		{
			if (m_bullets[i]->GetDead() == false)
			{
				Vector2 positionBeforeUpdate = m_bullets[i]->GetPosition();
				//collect general bullet info
				m_bullets[i]->Update(deltaSeconds);
				Vector2 positionAfterUpdate = m_bullets[i]->GetPosition();
				//bool hitWall;
				//HasLineOfSight(positionBeforeUpdate, positionAfterUpdate, hitWall);
				//hitWall = !hitWall;
				int turretIndex = checkBulletCollideWithEntities(((Entity**)(m_turrets)), m_numExistingTurrets, m_bullets[i]);
				int enemyTankIndex = checkBulletCollideWithEntities(((Entity**)m_enemyTanks), m_numExistingEnemyTanks, m_bullets[i]);
				int allyTankIndex = checkBulletCollideWithEntities(((Entity**)m_allyTanks), m_numExistingAllyTanks, m_bullets[i]);
				int damage = m_bullets[i]->getDamage();
				if (turretIndex != -1) //(hitWall == false && turretIndex != -1)
				{
					doStandardEntityHitBulletUpdate(m_bullets[i], m_turrets[turretIndex], damage);
				}
				else if (enemyTankIndex != -1) //(hitWall == false && enemyTankIndex != -1)
				{
					doStandardEntityHitBulletUpdate(m_bullets[i], m_enemyTanks[enemyTankIndex], damage);
				}
				else if (allyTankIndex != -1) //(hitWall == false && enemyTankIndex != -1)
				{
					doStandardEntityHitBulletUpdate(m_bullets[i], m_allyTanks[allyTankIndex], damage);
				}
				else if (checkBulletAndPlayerCollid(m_bullets[i]) == true) //test player
				{
					m_bullets[i]->SetDieingTrue();
					if (m_player->getInvulnerable() == false)
					{
						m_player->hit(damage);
					}
				}
				//else if (hitWall == true)
				//{
				//	m_bullets[i]->SetDieingTrue();
				//}
			}
		}
	}
}

void Map::doStandardEntityHitBulletUpdate(Entity* bullet, LivingEntity* targetHit, int damage)
{
	//test enemy turret for hit
	bullet->SetDieingTrue();
	targetHit->hit(damage);
	if (targetHit->GetDieing() == true && targetHit->m_deathAnimExists == false)
	{
		targetHit->SetDieingTrue();
	}
}

const void Map::LoadingGame()
{
	//generate turrets on start
	if (m_doneGeneratingTurretsAtStart == false && m_generatedTurretThisFrame == false)
	{
		m_generatedTurretThisFrame = true;
		GenerateTurret();
		if (m_numExistingTurrets >= m_numTurretsStart)
		{
			m_doneGeneratingTurretsAtStart = true;
		}
	}
	//generate enemy tanks on start
	if (m_doneGeneratingEnemyTanksAtStart == false && m_generatedEnemyTankThisFrame == false)
	{
		m_generatedEnemyTankThisFrame = true;
		GenerateEnemyTank();
		if (m_numExistingEnemyTanks >= m_numEnemyTanksStart)
		{
			m_doneGeneratingEnemyTanksAtStart = true;
		}
	}
	//generate ally tanks on start
	if (m_doneGeneratingAllyTanksAtStart == false && m_generatedAllyTankThisFrame == false)
	{
		m_generatedAllyTankThisFrame = true;
		GenerateAllyTank();
		if (m_numExistingAllyTanks >= m_numAllyTanksStart)
		{
			m_doneGeneratingAllyTanksAtStart = true;
		}
	}
	//these are used if decide to add in a cheat to add a turret or enemy tank later on.
	m_generatedTurretThisFrame = false;
	m_generatedEnemyTankThisFrame = false;
	m_generatedAllyTankThisFrame = false;

	if (m_doneGeneratingEnemyTanksAtStart == true && m_doneGeneratingTurretsAtStart == true && m_doneGeneratingAllyTanksAtStart == true)
	{
		m_gameLoading = false;
	}
}
bool Map::isGameLoading()
{
	return m_gameLoading;
}

const void Map::Update()
{
	//This method is for collision detection and such, with the tiles.
	for (std::set<Entity*>::iterator it = m_entities.begin(); it != m_entities.end(); ++it)
	{
		//bool deleteMe = false;
		Entity* entity = *it;
		if (entity != nullptr)
		{
			Vector2 entityPosition = entity->GetPosition();
			TileCoords entityTileCoords = GetTileCoordsForWorldCoords(entityPosition);
			float CollisionRadius = entity->getCollisionRadius();

			
			if (entity->DoesThisDoTilePhysics() == false)
			{
				if (IsTileSolid(entityTileCoords) == true)
				{
					entity->SetDeadTrue();
					if ((g_positionPlayerLastAlive - entity->GetPosition()).GetLength() < m_tileDistanceFromPlayerInOrderToPlaySound)
					{
						m_explosionsound_audio = m_audio->PlaySound(m_explosionsoundID, 0.2f);
					}
				}
			}
			else
			{
				DoWallPhysics(entity, entityTileCoords, CollisionRadius, entityPosition);
			}
		}
	}
}


const void Map::DoWallPhysics(Entity* entity, TileCoords& entityTileCoords, float& CollisionRadius, Vector2& entityPosition)
{
	DoNESWPhysics(entity, entityTileCoords, CollisionRadius, entityPosition);
	DoDiagPhysics(entity, entityTileCoords, CollisionRadius, entityPosition);
}

const void Map::DoNESWPhysics(Entity* entity, TileCoords& entityTileCoords, float& CollisionRadius, Vector2& entityPosition)
{
	//N,E,S,W direction
	TileCoords North = TileCoords(entityTileCoords.x, entityTileCoords.y + 1);
	if (IsTileSolid(North) == true)
	{
		float yDifference = (entityPosition.y + CollisionRadius) - North.y;
		if (yDifference > 0)
		{
			entityPosition = Vector2(entityPosition.x, entityPosition.y - yDifference);
			entity->SetPosition(entityPosition);
		}
	}
	TileCoords East = TileCoords(entityTileCoords.x + 1, entityTileCoords.y);
	if (IsTileSolid(East) == true)
	{
		float xDifference = (entityPosition.x + CollisionRadius) - East.x;
		if (xDifference > 0)
		{
			entityPosition = Vector2(entityPosition.x - xDifference, entityPosition.y);
			entity->SetPosition(entityPosition);
		}
	}
	TileCoords South = TileCoords(entityTileCoords.x, entityTileCoords.y - 1);
	if (IsTileSolid(South) == true)
	{
		float yDifference = (entityPosition.y - CollisionRadius) - (South.y + 1.f);
		if (yDifference < 0)
		{
			entityPosition = Vector2(entityPosition.x, entityPosition.y - yDifference);
			entity->SetPosition(entityPosition);
		}
	}
	TileCoords West = TileCoords(entityTileCoords.x - 1, entityTileCoords.y);
	if (IsTileSolid(West) == true)
	{
		float xDifference = (entityPosition.x - CollisionRadius) - (West.x + 1.f);
		if (xDifference < 0)
		{
			entityPosition = Vector2(entityPosition.x - xDifference, entityPosition.y);
			entity->SetPosition(entityPosition);
		}
	}
}
const void Map::DoDiagPhysics(Entity* entity, TileCoords& entityTileCoords, float& CollisionRadius, Vector2& entityPosition)
{
	//NorthWest, NorthEast, SouthEast, SouthWest directions
	TileCoords NorthWest = TileCoords(entityTileCoords.x - 1, entityTileCoords.y + 1);
	if (IsTileSolid(NorthWest) == true)
	{
		//computer the corner position.
		Vector2 corner = Vector2((float)(NorthWest.x + 1.f), (float)(NorthWest.y));
		Vector2 displacementFromCorner = entityPosition - corner;
		float distanceBetweenThem = displacementFromCorner.GetLength();
		if (distanceBetweenThem < CollisionRadius)
		{
			float overlapDepth = CollisionRadius - distanceBetweenThem;
			displacementFromCorner.normalize();
			entityPosition = entityPosition + (displacementFromCorner * overlapDepth);
			entity->SetPosition(entityPosition);
		}
	}


	TileCoords NorthEast = TileCoords(entityTileCoords.x + 1, entityTileCoords.y + 1);
	if (IsTileSolid(NorthEast) == true)
	{
		//computer the corner position.
		Vector2 corner = Vector2((float)(NorthEast.x), (float)(NorthEast.y));
		Vector2 displacementFromCorner = entityPosition - corner;
		float distanceBetweenThem = displacementFromCorner.GetLength();
		if (distanceBetweenThem < CollisionRadius)
		{
			float overlapDepth = CollisionRadius - distanceBetweenThem;
			displacementFromCorner.normalize();
			entityPosition = entityPosition + (displacementFromCorner * overlapDepth);
			entity->SetPosition(entityPosition);
		}
	}

	TileCoords SouthEast = TileCoords(entityTileCoords.x + 1, entityTileCoords.y - 1);
	if (IsTileSolid(SouthEast) == true)
	{
		Vector2 corner = Vector2((float)(SouthEast.x), (float)(SouthEast.y + 1.f));
		Vector2 displacementFromCorner = entityPosition - corner;
		float distanceBetweenThem = displacementFromCorner.GetLength();
		if (distanceBetweenThem < CollisionRadius)
		{
			float overlapDepth = CollisionRadius - distanceBetweenThem;
			displacementFromCorner.normalize();
			entityPosition = entityPosition + (displacementFromCorner * overlapDepth);
			entity->SetPosition(entityPosition);
		}
	}

	TileCoords SouthWest = TileCoords(entityTileCoords.x - 1, entityTileCoords.y - 1);
	if (IsTileSolid(SouthWest) == true)
	{
		Vector2 corner = Vector2((float)(SouthWest.x + 1.f), (float)(SouthWest.y + 1.f));
		Vector2 displacementFromCorner = entityPosition - corner;
		float distanceBetweenThem = displacementFromCorner.GetLength();
		if (distanceBetweenThem < CollisionRadius)
		{
			float overlapDepth = CollisionRadius - distanceBetweenThem;
			displacementFromCorner.normalize();
			entityPosition = entityPosition + (displacementFromCorner * overlapDepth);
			entity->SetPosition(entityPosition);
		}
	}
}