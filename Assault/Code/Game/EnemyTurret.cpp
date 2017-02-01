#include "Game/EnemyTurret.hpp"

//constructors and destructor
EnemyTurret::EnemyTurret(Vector2 position)
: EnemyEntity(ENTITY_TURRET)
{
	m_position = position;
	m_color = Rgba(1.f, 0.f, 0.f, 1.f);
	m_angularDisplacement = 0.f;
	m_collisionRadius = 0.5f;
	m_additiveSpeed = 0.f;
	m_maxMovementSpeed = 0.f;
	m_mapIndexID = 0;
	m_solid = true;

	m_dieing = false;
	m_dead = false;

	m_targetPosition = Vector2((float)(rand() % 100), (float)(rand() % 100));
	m_lineOfSightRadius = 10.f;
	m_defaultChangeCourseInSeconds = 3.f;
	m_changeCourseInSeconds = m_defaultChangeCourseInSeconds;
	m_turningSpeed = 45.f;
}

EnemyTurret::~EnemyTurret()
{
	//destructor
}

//updates and render
void EnemyTurret::Update(float deltaSeconds)
{
	//Enemy Turret does not need to move.
	deltaSeconds = deltaSeconds;
}
void EnemyTurret::EnemyUpdate(float deltaSeconds, Vector2 playerPosition, bool hasLineOfSight)
{
	//turrets are suppose to only turn in place.
	invulnerable = false;
	enemyTurn(deltaSeconds, playerPosition, hasLineOfSight);
}
const void EnemyTurret::Render(Vector2& scale)
{
	//renders the turret as a circle with a line to indicate direction facing
	g_Renderer->Push();
	g_Renderer->Translate(Vector2(m_position.x*scale.x, m_position.y * scale.y));

	//Line of sight radius
	//g_Renderer->RenderPolygon(Vector2(0, 0), m_lineOfSightRadius, 60, 0.f, Rgba(1.f, 1.f, 0.f, 0.6f), true);

	g_Renderer->Rotate(m_angularDisplacement);

	AABB2 turretBounds = AABB2(Vector2(-m_collisionRadius* scale.x, -m_collisionRadius*scale.y), Vector2(m_collisionRadius*scale.x, m_collisionRadius*scale.y));

	Vector2 lineEnd = Vector2(m_collisionRadius*scale.x, 0.f);

	g_Renderer->RenderPolygon(Vector2(0, 0), m_collisionRadius*scale.x, 20, 0.f, m_color);
	g_Renderer->RenderLine(Vector2(0, 0), lineEnd, m_color, 5.f); //line indicator of direction.


	g_Renderer->Pop();
}