#pragma once
#include "Game/Entity.hpp"
#include "Engine/Renderer/SpriteAnim.hpp"

#ifndef LIVINGENTITY_HPP
#define LIVINGENTITY_HPP

class LivingEntity : public Entity
{
public:
	//Constructor and Destructor
	LivingEntity(EntityType type);
	virtual ~LivingEntity();

	//Update and Render
	virtual void Update(float deltaSeconds);
	virtual void UpdateVelocityFromKeyboard();
	virtual const void Render(Vector2& scale);
	int getCurrentHealth();
	virtual void hit(int damageDone);

	bool getInvulnerable();

	//variables
	int max_health;
	int current_health;
	bool invulnerable;
	bool invuln_show_model;
	float MaxInvulnerableSeconds;
	float invulnerableForAmountOfSeconds;
	float SecondsSinceLastShowInvulnModel;
};

#endif // !LIVINGENTITY_HPP