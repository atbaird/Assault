#pragma once

#include <vector>
#include <set>
#include "Game/Tile.hpp"
#include "Game/Bullet.hpp"
#include "Game/Tank.hpp"
#include "Game/EnemyTurret.hpp"
#include "Game/EnemyTank.hpp"
#include "Game/TheApp.hpp"
#include "Engine/Core/Audio.hpp"
#include "Game/Explosion.hpp"
#include "Game/AllyTank.hpp"


#ifndef TILEDEF_HPP
#define TILEDEF_HPP

struct TileDef
{
public:
	Texture* m_texture;
	bool m_isSolid;
};
#endif

#ifndef MAP_HPP
#define MAP_HPP

class Map
{
public:
	//constructors and destructor
	Map(int xSize, int ySize, XboxController** controllers, AudioSystem* audio, bool randomlyGenerate, std::string mapFileLocation);
	~Map();
	void InitializeTileDefinitions();
	void InitializeExplosionDefinitions();
	void loadMapFromImage(std::string mapFileLocation);
	TileType GetTileType(unsigned char red, unsigned char green, unsigned char blue);
	EntityType GetEntityType(unsigned char red, unsigned char green, unsigned char blue);
	

	//player set up
	void setUpPlayer(Tank* player);
	void randomGenerateMap();

	//getters
	TileCoords GetTileCoordsForTile(const Tile& tile) const;
	Tile* GetTileAtCoords(const TileCoords& tileCoords);
	Tile* GetTileAtIndex(int TileIndex);
	Tile* GetTileAtWorldCoords(const Vector2& worldCoordinates);
	Vector2 GetWorldCoordsForTileCoords(const TileCoords& tileCoords);
	TileCoords GetTileCoordsForWorldCoords(const Vector2& WorldCoords);
	TileCoords GetTCforIndex(int index);
	int GetIndexForTileCoords(TileCoords tileCoords);
	bool IsTileSolid(TileCoords tileLoc);

	//TileCoords HasLineOfSight(Vector2 start, Vector2 end, bool& hasLineOfSight);
	TileCoords FastHasLineOfSight(Vector2 start, Vector2 end, bool& hasLineOfSight);

	//setters
	void changeTileType(TileCoords& tileLocation, TileType newType);

	//remover
	void removeEntity(Entity* entityPointer);

	//adders
	void addEntity(Entity* newEntity);

	//randomGeneration
	Vector2 GetRandomNoneOccupiedCoords();

	//render
public:
	const void Render();
	const void MapRender(bool fogOfWarActive, Vector2 scale);
	const void EntityRender(bool fogOfWarActive, Vector2 scale);
	const void MiniMapRender();
	const void genericEntitiesRender(Entity** entities, const int& numExistingEntities, bool fogOfWarActive, Vector2 scale);

	//Update
	const void Update(float deltaSeconds);
	const void UpdateEntities(float deltaSeconds);
	const void UpdateBullets(float deltaSeconds);
	void doStandardEntityHitBulletUpdate(Entity* bullet, LivingEntity* targetHit, int damage);
	const void LoadingGame();
	const void Update();
	const void DoWallPhysics(Entity* entity, TileCoords& entityTileCoords, float& CollisionRadius, Vector2& entityPosition);
	const void DoNESWPhysics(Entity* entity, TileCoords& entityTileCoords, float& CollisionRadius, Vector2& entityPosition);
	const void DoDiagPhysics(Entity* entity, TileCoords& entityTileCoords, float& CollisionRadius, Vector2& entityPosition);

	//death and collision
	const void CheckForDeath();
	int checkBulletCollideWithEntities(Entity** entities, int& numOfEntities, Bullet* currentBullet);
	bool isGameLoading();
	bool checkBulletAndPlayerCollid(Bullet* currentBullet);
	void checkShoot();
	void checkShootEnemyEntities(EnemyEntity** entities, const int& numOfEntities);

	Vector2 calculateNearestFromEntities(Vector2 originPosition, Entity** entities, int numOfEntities);

	void checkCollisionsBetweenAllEntities();
	void checkCollisionBetweenEntityAndEntityGroup(Entity* entity, EnemyEntity** entities, const int& numberOfEntities, bool comparingToUnmovableEntities);
	void pushEntitiesOutOfEachOther(Entity*& a, Entity*& b);
	void pushEntityOutOfFixedEntity(Entity*& mobileEnt, Entity*& stationary);
	int checkCollisionBetweenEntityAndEntities(Entity* thisEntity, Entity** entities, int numOfEntities);

private:
	//generators
	void GenerateBullet(Entity* spawningFrom);
	void GenerateTurret();
	void GenerateEnemyTank();
	void GenerateAllyTank();
	void GenerateExplosion(Vector2 position, int index);

	//destroy
	void deleteTurret(int index);
	void deleteBullet(int index);
	void deleteEnemyTank(int index);
	void deleteAllyTank(int index);

	//entityToEntityPhysics


public:
	//VARIABLES
	//player related
	Tank* m_player;
	//keeps track of how long the player's been dead.
	float m_playerDeadForThisManySeconds;
	Rgba m_deathScreenColor;
	float m_amountOfAlphaToAddOverSeconds;
	Vector2 g_positionPlayerLastAlive;
	float g_playerLastAngularDisplacement;

private:
	//controllers
	XboxController** g_controllers;

	//tiledefinitions
	static TileDef s_tileDefinitions[NUM_TILE_TYPES];

	//other entities
	std::vector<Bullet*> m_bullets;
	EnemyTurret** m_turrets;
	EnemyTank** m_enemyTanks;
	AllyTank** m_allyTanks;
	AABB2 m_aAbb;
	Texture* m_testTexture;

	Vector2 m_playerStartPosition;
	Vector2 m_bottomLeftValForOrthoMiniMap;

	//enemy and other entity number tracking
	int m_maxBullets;
	int m_maxTurrets;
	int m_maxAllyTanks;
	int m_maxEnemyTanks;
	int m_numTurretsStart;
	int m_numEnemyTanksStart;
	int m_numAllyTanksStart;
	int m_numExistingBullets;
	int m_numExistingTurrets;
	int m_numExistingEnemyTanks;
	int m_numExistingAllyTanks;


	//space bar relase tracking
	bool m_spaceIsReleased;
	bool m_isFReleased;
	bool m_isRReleased;
	float m_spaceBarHeldAndSinceLastBulletFire;
	float m_timeBetweenPlayerBullets;
	float m_tileDistanceFromPlayerInOrderToPlaySound;
	float m_fogOfWar;
	float m_normalAmountToRenderFromPlayer;

	//start up
	bool m_generatedTurretThisFrame;
	bool m_doneGeneratingTurretsAtStart;
	bool m_randomlyGenerate;

	bool m_generatedEnemyTankThisFrame;
	bool m_doneGeneratingEnemyTanksAtStart;

	bool m_generatedAllyTankThisFrame;
	bool m_doneGeneratingAllyTanksAtStart;

	//loading game
	bool m_gameLoading;
	bool m_fogOfWarActive;
	bool m_miniMapFogOfWarActive;

	//tiles and general entities.
	std::vector<Tile> m_tiles;
	std::vector<TileCoords> unsolidTileLocs;
	TileCoords m_size;
	std::set<Entity*>m_entities;

	//sounds
	SoundID m_explosionsoundID;
	AudioChannelHandle m_explosionsound_audio;
	static const std::string m_explosionsound_Name;
	SoundID m_gunshotID;
	AudioChannelHandle m_gunshot_audio;
	static const std::string m_gunshot_Name;
	AudioSystem* m_audio;

	//Explosions
	std::vector<Explosion> m_explosions;
	std::vector<Explosion> m_activeExplosions;


	//TileIndexes = ints
	//WorldCoords = floats
};

#endif // !MAP_HPP
