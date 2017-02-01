#pragma once
#include "Game/EnemyEntity.hpp"

#ifndef ENEMYTURRET_HPP
#define ENEMYTURRET_HPP

class EnemyTurret : public EnemyEntity
{
public:
	//constructor and destructor
	EnemyTurret(Vector2 position);
	~EnemyTurret();

	//updates and render
	void Update(float deltaSeconds);
	void EnemyUpdate(float deltaSeconds, Vector2 playerPosition, bool hasLineOfSight);
	const void Render(Vector2& scale);
};
#endif // !ENEMYTURRET_HPP
