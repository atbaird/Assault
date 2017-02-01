#pragma once
#include "Game/Entity.hpp"

#ifndef BULLET_HPP
#define BULLET_HPP

class Bullet : public Entity
{
public:
	//constructors and destructor
	Bullet();
	Bullet(Entity* entitySpawningFrom);
	~Bullet();

	//Updates and Render
	void Update(float deltaSeconds);
	const void Render(Vector2 scale);
	const int getDamage();

	Texture* m_bulletTexture;
	int damage;
	//in the bullet's case, m_alliance will be used to tell who fired it.
};

#endif // !BULLET_HPP
