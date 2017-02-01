#include "Game/Bullet.hpp"

//constructors and destructor
Bullet::Bullet()
: Entity(ENTITY_BULLET)
{
}

Bullet::Bullet(Entity* entitySpawningFrom)
: Entity(ENTITY_BULLET)
{
	float entityCollision = entitySpawningFrom->getCollisionRadius();
	m_angularDisplacement = entitySpawningFrom->getAngularDisplacement();
	m_collisionRadius = 0.05f;
	m_maxMovementSpeed = entitySpawningFrom->getMaxMovementSpeed() + 3.f;
	m_color = g_Renderer->White;
	m_alliance = entitySpawningFrom->getAlliance();

	m_bulletTexture = Texture::CreateOrGetTexture("Data/Images/Bullet.png");

	m_position = entitySpawningFrom->GetPosition() + Vector2(entityCollision * CosDegrees(m_angularDisplacement), entityCollision * SinDegrees(m_angularDisplacement));
	m_velocity = Vector2(m_maxMovementSpeed * CosDegrees(m_angularDisplacement), m_maxMovementSpeed * SinDegrees(m_angularDisplacement));
	damage = 1;
}
Bullet::~Bullet()
{

}

//Updates and Render
void Bullet::Update(float deltaSeconds)
{
	m_position = m_position + (m_velocity * deltaSeconds);
}
const void Bullet::Render(Vector2 scale)
{
	g_Renderer->Push();
	g_Renderer->Translate(Vector2(m_position.x*scale.x, m_position.y * scale.y));

	g_Renderer->Rotate(m_angularDisplacement);

	AABB2 bulletBounds = AABB2(Vector2(-m_collisionRadius*scale.x, -m_collisionRadius*scale.y), Vector2(m_collisionRadius*scale.x, m_collisionRadius*scale.y));

	g_Renderer->DrawTexturedAABB(bulletBounds, m_bulletTexture, Vector2(0.f, 0.f), Vector2(1.f, 1.f), m_color);

	//g_Renderer->RenderPolygon(Vector2(0, 0), m_collisionRadius, 20, m_angularDisplacement, m_color);
	g_Renderer->Pop();
}

const int Bullet::getDamage()
{
	return damage;
}