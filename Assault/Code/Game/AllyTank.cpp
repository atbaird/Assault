#include "Game/AllyTank.hpp"

AllyTank::AllyTank(Vector2 position) : EnemyEntity(ENTITY_ALLY_TANK)
{
	//ally tank constructor
	m_position = position;
	m_color = Rgba(0.2f, 0.2f, 1.f, 1.f);
	m_angularDisplacement = 0.f;
	m_collisionRadius = 0.5f;
	m_additiveSpeed = 0.f;
	m_maxMovementSpeed = 0.5f;
	m_mapIndexID = 0;
	m_alliance = 1;
	m_solid = true;
	tankTexture = Texture::CreateOrGetTexture("Data/Images/Tank.png");
	SpriteSheet tanktredsSpriteSheet = SpriteSheet("Data/Images/TankTreds animation.png", 8, 1);
	leftTred = SpriteAnim(tanktredsSpriteSheet, 1.f, SPRITE_ANIM_MODE_LOOPING, 0, 7);
	leftTred.Reverse();
	rightTred = SpriteAnim(tanktredsSpriteSheet, 1.f, SPRITE_ANIM_MODE_LOOPING, 0, 7);
	rightTred.Reverse();

	m_dieing = false;
	m_dead = false;

	m_targetPosition = Vector2();
	m_lineOfSightRadius = 5.f;
	m_defaultChangeCourseInSeconds = 3.f;
	m_changeCourseInSeconds = m_defaultChangeCourseInSeconds;
	m_turningSpeed = 60.f;

	invulnerable = false;
	MaxInvulnerableSeconds = 3.f;
	invulnerableForAmountOfSeconds = 0.f;

	max_health = 3;
	current_health = max_health;
}

AllyTank::~AllyTank()
{

}

//update and Render
void AllyTank::Update(float deltaSeconds)
{
	invulnerable = false;
	//updates the velocity and position.
	m_velocity.x = m_maxMovementSpeed * CosDegrees(m_angularDisplacement);
	m_velocity.y = m_maxMovementSpeed * SinDegrees(m_angularDisplacement);
	LivingEntity::Update(deltaSeconds);
}
void AllyTank::EnemyUpdate(float deltaSeconds, Vector2 targetPosition, bool hasLineOfSight)
{
	//turns the tank and moves it.
	enemyTurn(deltaSeconds, targetPosition, hasLineOfSight);
	leftTred.Update(deltaSeconds*3.f);
	rightTred.Update(deltaSeconds*3.f);
	Update(deltaSeconds);
}
const void AllyTank::Render(Vector2& scale)
{
	//renders the enemy tank with the tank texture, colored red.
	g_Renderer->Push();
	g_Renderer->Translate(Vector2(m_position.x*scale.x, m_position.y * scale.y));

	//Line of sight radius
	//g_Renderer->RenderPolygon(Vector2(0, 0), m_lineOfSightRadius, 60, 0.f, Rgba(1.f, 0.5f, 0.5f, 0.6f), true);

	g_Renderer->Rotate(m_angularDisplacement);

	AABB2 tankBounds = AABB2(Vector2(-m_collisionRadius * scale.x, -m_collisionRadius * scale.y), Vector2(m_collisionRadius * scale.x, m_collisionRadius * scale.y));

	g_Renderer->DrawTexturedAABB(tankBounds, tankTexture, Vector2(0.f, 0.f), Vector2(1.f, 1.f), m_color);
	leftTred.Render(AABB2(Vector2((m_collisionRadius / 2.f) * scale.x, (m_collisionRadius) * scale.y),
		Vector2(-(m_collisionRadius *(2.19f / 3.f)) * scale.x, (m_collisionRadius* (0.9f / 2.f)) * scale.y)), 0.f, m_color);
	rightTred.Render(AABB2(Vector2(-(m_collisionRadius / 2.f) * scale.x, (m_collisionRadius) * scale.y),
		Vector2(m_collisionRadius* (2.19f / 3.f) * scale.x, (m_collisionRadius * (0.9f / 2.f) * scale.y))), 180.f, m_color);
	//g_Renderer->RenderPolygon(Vector2(0, 0), m_collisionRadius, 20, 0.f, m_color);



	g_Renderer->Pop();
}