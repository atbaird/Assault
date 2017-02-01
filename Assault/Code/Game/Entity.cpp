#include "Game/Entity.hpp"

//Constructor and Destructor
Entity::Entity(EntityType type)
{
	//constructor
	m_position = Vector2();
	m_velocity = Vector2();
	m_angularDisplacement = 0.f;
	m_collisionRadius = 0.f;
	m_additiveSpeed = 0.f;
	m_maxMovementSpeed = 0.f;
	m_solid = false;
	m_color = g_Renderer->White;
	m_mapIndexID = 0;
	m_alliance = 0;
	m_dead = false;
	m_dieing = false;
	m_deathAnimExists = false;
	m_doTilePhysics = false;
	m_type = type;
}
Entity::~Entity()
{
	//destructor
}

//Update and Render
void Entity::Update(float deltaSeconds)
{
	//default update method.
	m_position = m_position + (m_velocity * deltaSeconds);
}
const void Entity::Render(Vector2& scale)
{
	scale = scale;
	//default render method
}

//Setters
void Entity::SetPosition(Vector2 newPosition)
{
	//sets the entity's position to the given newPosition
	m_position = newPosition;
}
void Entity::SetSpeed(Vector2 newSpeed)
{
	//Sets the Entity's speed to the given newSpeed
	m_velocity = newSpeed;
}
void Entity::SetAngularDisplacement(float newAngularDisplacement)
{
	//sets the angular displacement to the given newAngularDisplacement.
	m_angularDisplacement = newAngularDisplacement;
}
void Entity::SetColor(Rgba newColor)
{
	//sets the entity's wireframe color to the given newColor
	m_color = newColor;
}

//Getters
Vector2 Entity::GetPosition()
{
	//returns this entity's positon
	return m_position;
}
Vector2 Entity::GetSpeed()
{
	//returns this entity's speed
	return m_velocity;
}
float Entity::getMaxMovementSpeed()
{
	return m_maxMovementSpeed;
}
float Entity::getAngularDisplacement()
{
	//returns this entity's angular displacement
	return m_angularDisplacement;
}
float Entity::getCollisionRadius()
{
	//returns the collision radius of this entity
	return m_collisionRadius;
}
Rgba Entity::GetColor()
{
	//returns this entity's color.
	return m_color;
}
int Entity::getAlliance()
{
	return m_alliance;
}

//calc
float CalcShortestAngularDisplacement(float fromDegrees, float toDegrees)
{
	float angularDisplacement = toDegrees - fromDegrees;
	while (angularDisplacement > 180.f)
	{
		angularDisplacement -= 360.f;
	}
	while (angularDisplacement < -180.f)
	{
		angularDisplacement += 360.f;
	}
	return angularDisplacement;
}


//setters
void Entity::SetDieingTrue()
{
	//sets dieing to true
	m_dieing = true;
	if (m_deathAnimExists == false)
	{
		m_dead = true;
	}
}
void Entity::SetDeadTrue()
{
	//sets dead to true
	m_dead = true;
}

//getters
bool Entity::GetDieing()
{
	return m_dieing;
}
bool Entity::GetDead()
{
	return m_dead;
}