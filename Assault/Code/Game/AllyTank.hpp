#pragma once
#include "Game/EnemyEntity.hpp"

#ifndef ALLYTANK_HPP
#define ALLYTANK_HPP

class AllyTank : public EnemyEntity
{
public:
	AllyTank(Vector2 position);
	~AllyTank();

	//update and Render
	void Update(float deltaSeconds);
	void EnemyUpdate(float deltaSeconds, Vector2 targetPosition, bool hasLineOfSight);
	const void Render(Vector2& scale);

	//variables
	Texture* tankTexture;
	SpriteAnim leftTred;
	SpriteAnim rightTred;
};

#endif