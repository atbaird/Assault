#pragma once

#include "LivingEntity.hpp"

class EnemyEntity : public LivingEntity
{
public:
	//constructor and destructor
	EnemyEntity(EntityType type);
	virtual ~EnemyEntity();

	//updates
	virtual void EnemyUpdate(float deltaSeconds, Vector2 playerPosition, bool blocked);
	virtual void Update(float deltaSeconds);
	virtual const void Render(Vector2& scale);

	float getLineOfSight();
	bool getShoot();
	void resetShoot();
	void enemyTurn(float& deltaSeconds, Vector2& playerPosition, bool& hasLineOfSight);

	//variables
	float m_changeCourseInSeconds;
	float m_defaultChangeCourseInSeconds;
	float m_lineOfSightRadius;
	Vector2 m_targetPosition;
	float m_turningSpeed;
	bool m_shoot;
	float m_maxAmountOfTimeBeforeShoot;
	float m_currentAmountOfTimeBeforeCanShoot;
};