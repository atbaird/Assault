#include "Game/LivingEntity.hpp"

//Constructor & Destructor
LivingEntity::LivingEntity(EntityType type) : Entity(type)
{
	//Constructor
	m_dieing = false;
	m_dead = false;
	current_health = 1;
	max_health = 1;
	invulnerable = false;
	MaxInvulnerableSeconds = 3.f;
	invulnerableForAmountOfSeconds = 0.f;
	invuln_show_model = true;
	SecondsSinceLastShowInvulnModel = 0.f;
	m_doTilePhysics = true;
}

LivingEntity::~LivingEntity()
{
//destructor
}

//Updaters and Render
void LivingEntity::Update(float deltaSeconds)
{
	//Default Update method for all living entities.
	m_position = m_position + (m_velocity * deltaSeconds);
	if (invulnerable == true)
	{
		invulnerableForAmountOfSeconds += deltaSeconds;
		SecondsSinceLastShowInvulnModel += deltaSeconds;
		
		if (SecondsSinceLastShowInvulnModel >= 0.2f)
		{
			invuln_show_model = !invuln_show_model;
			SecondsSinceLastShowInvulnModel = 0.f;
		}
		if (invulnerableForAmountOfSeconds >= MaxInvulnerableSeconds)
		{
			invulnerable = false;
			invuln_show_model = true;
		}
	}
	else
	{
		invulnerableForAmountOfSeconds = 0;
	}
}
void LivingEntity::UpdateVelocityFromKeyboard()
{

	//updates the x and y speeds based on whether or not those speeds are presently less than the maximum possible speed.
	m_velocity.x = CosDegrees(m_angularDisplacement) * (m_additiveSpeed * 1.5f);
	m_velocity.y = SinDegrees(m_angularDisplacement) * (m_additiveSpeed * 1.5f);
}
const void LivingEntity::Render(Vector2& scale)
{
	Entity::Render(scale);
	//default render method
}


//getters
int LivingEntity::getCurrentHealth()
{
	return current_health;
}

//damage and invulnerability
void LivingEntity::hit(int damageDone)
{
	current_health -= damageDone;
	float red = m_color.Red - (1.f / max_health);
	if (red < 0.f)
	{
		red = 0.f;
	}
	float green = m_color.Green - (1.f / max_health);
	if (green < 0.f)
	{
		green = 0.f;
	}
	float blue = m_color.Blue - (1.f / max_health);
	if (blue < 0.f)
	{
		blue = 0.f;
	}
	m_color = Rgba(red, green, blue, m_color.Alpha);

	if (current_health <= 0)
	{
		m_dieing = true;
	}
	else
	{
		invulnerable = true;
	}
}


bool LivingEntity::getInvulnerable()
{
	return invulnerable;
}