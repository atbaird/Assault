#pragma once
#include "Game/EnemyEntity.hpp"

#ifndef ENEMYTANK_HPP
#define ENEMYTANK_HPP

class EnemyTank : public EnemyEntity
{
public:
	//constructor and destructor
	EnemyTank(Vector2 position);
	~EnemyTank();

	//updates and render
	void Update(float deltaSeconds);
	void EnemyUpdate(float deltaSeconds, Vector2 playerPosition, bool hasLineOfSight);
	const void Render(Vector2& scale);

	//variables
	Texture* tankTexture;

	SpriteAnim leftTred;
	SpriteAnim rightTred;
};

#endif // !ENEMYTANK_HPP
