#pragma once

#include "Engine/Renderer/Texture.hpp"
#include "Engine/Renderer/Renderer.hpp"
#ifndef ENTITY_HPP
#define ENTITY_HPP

enum EntityType
{
	ENTITY_INVALID = -1,
	ENTITY_PLAYER,
	ENTITY_TURRET,
	ENTITY_ENEMY_TANK,
	ENTITY_ALLY_TANK,
	ENTITY_BULLET,
	NUM_ENTITY_TYPES
};

class Entity
{
public:
	//Constructor and Destructor
	Entity(EntityType type);
	virtual ~Entity();
	
	//Update and Render
	virtual void Update(float deltaSeconds);
	virtual const void Render(Vector2& scale);

	//Setters
	void SetPosition(Vector2 newPosition);
	void SetSpeed(Vector2 newSpeed);
	void SetAngularDisplacement(float newAngularDisplacement);
	void SetColor(Rgba newColor);

	void SetDieingTrue();
	void SetDeadTrue();

	//Getters
	Vector2 GetPosition();
	Vector2 GetSpeed();
	float getMaxMovementSpeed();
	float getAngularDisplacement();
	float getCollisionRadius();
	Rgba GetColor();
	int getAlliance();

	bool GetDieing();
	bool GetDead();
	bool DoesThisDoTilePhysics() {return m_doTilePhysics;}

	//Calc

	float CalcShortestAngularDisplacement(float fromDegrees, float);

	//variables
	Vector2 m_position;
	Vector2 m_velocity;
	float m_angularDisplacement;
	float m_collisionRadius;
	float m_additiveSpeed;
	float m_maxMovementSpeed;
	int m_mapIndexID;
	int m_alliance; //used to prevent friendly fire, or shooting yourself.; any living entity with it will be able to damage anyone not on their same team when shooting
	//0 == enemy, 1 == player
	bool m_solid;

	bool m_dieing;
	bool m_dead;
	bool m_deathAnimExists;
	bool m_doTilePhysics;
	Rgba m_color;
	EntityType m_type;
};

#endif // !ENTITY_HPP
