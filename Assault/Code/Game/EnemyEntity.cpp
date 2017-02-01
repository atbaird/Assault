#include "Game/EnemyEntity.hpp"

//Constructor & destructor
EnemyEntity::EnemyEntity(EntityType type) : LivingEntity(type)
{
	//constructor
	m_targetPosition = Vector2();
	m_changeCourseInSeconds = 3.f;
	m_defaultChangeCourseInSeconds = 3.f;
	m_lineOfSightRadius = 5.f;
	m_turningSpeed = 10.f;
	m_shoot = false;
	m_maxAmountOfTimeBeforeShoot = 3.f;
	m_currentAmountOfTimeBeforeCanShoot = m_maxAmountOfTimeBeforeShoot;
}
EnemyEntity::~EnemyEntity()
{
	//destructor
}

//Updates and Render
void EnemyEntity::EnemyUpdate(float deltaSeconds, Vector2 playerPosition, bool blocked)
{
	//Special Enemy Update method, ment for having the enemy adjust to track the player position.
	blocked = blocked;
	playerPosition = playerPosition;
	Update(deltaSeconds);
}

void EnemyEntity::Update(float deltaSeconds)
{
	//default update method
	LivingEntity::Update(deltaSeconds);
}
const void EnemyEntity::Render(Vector2& scale)
{
	//default render method
	LivingEntity::Render(scale);
}

float EnemyEntity::getLineOfSight()
{
	return m_lineOfSightRadius;
}
bool EnemyEntity::getShoot()
{
	//returns whether the entity wants to shoot
	return m_shoot;
}
void EnemyEntity::resetShoot()
{
	//sets want to shoot back to false.
	m_shoot = false;
}

void EnemyEntity::enemyTurn(float& deltaSeconds, Vector2& playerPosition, bool& hasLineOfSight)
{
	//handles a slow turn towards a target.
	Vector2 displacementToPlayer = playerPosition - m_position;
	m_currentAmountOfTimeBeforeCanShoot -= deltaSeconds;
	if (hasLineOfSight == true && displacementToPlayer.GetLength() <= m_lineOfSightRadius)
	{
		//turns towards the player
		m_targetPosition = playerPosition;
		m_changeCourseInSeconds = m_defaultChangeCourseInSeconds;
		if (displacementToPlayer.y != 0.f || displacementToPlayer.x != 0.f)
		{
			float goalAngle = RadiansToDegrees(atan2(displacementToPlayer.y, displacementToPlayer.x));
			float angleToGoal = ComputeShortestAngularDisplaement(m_angularDisplacement, goalAngle);
			float sign = 1.f;
			if (angleToGoal < 0.f)
				sign = -1.f;
			float turnBy = m_turningSpeed * deltaSeconds;
			if (abs(angleToGoal) < turnBy)
			{
				m_angularDisplacement = goalAngle;
				if (m_currentAmountOfTimeBeforeCanShoot <= 0.f)
				{
					m_shoot = true;
					m_currentAmountOfTimeBeforeCanShoot = m_maxAmountOfTimeBeforeShoot;
				}
			}
			else
			{
				m_angularDisplacement += (sign * turnBy);
			}
		}
	}
	else
	{
		//face random target.
		Vector2 displacementToTarget = m_targetPosition - m_position;
		if (displacementToTarget.y != 0.f || displacementToTarget.x != 0.f)
		{
			float goalAngle = RadiansToDegrees(atan2(displacementToTarget.y, displacementToTarget.x));
			float angleToGoal = ComputeShortestAngularDisplaement(m_angularDisplacement, goalAngle);
			float sign = 1.f;
			if (angleToGoal < 0.f)
				sign = -1.f;
			float turnBy = m_turningSpeed * deltaSeconds;
			if (abs(angleToGoal) < turnBy)
			{
				m_angularDisplacement = goalAngle;
			}
			else
			{
				m_angularDisplacement += (sign * turnBy);
			}
		}
		m_changeCourseInSeconds -= deltaSeconds;
		if (m_changeCourseInSeconds <= 0)
		{
			float newTargetAngle = (float)((rand() % 360) - 180);
			m_targetPosition = Vector2((m_position.x + CosDegrees(newTargetAngle)), (m_position.y + SinDegrees(newTargetAngle)));
			m_changeCourseInSeconds = m_defaultChangeCourseInSeconds;
		}
	}
}