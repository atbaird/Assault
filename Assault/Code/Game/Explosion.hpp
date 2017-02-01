#pragma once

#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Renderer/SpriteAnim.hpp"

#ifndef EXPLOSION_HPP
#define EXPLOSION_HPP

class Explosion
{
public:
	Explosion();
	Explosion(const Explosion& other);
	~Explosion();

	void SetPosition(Vector2 position) { m_position = position; }
	void setRadius(float radius) { m_radius = radius; }
	Vector2 getPosition() { return m_position; }
	float getRadius() { return m_radius; }
	bool isDone() { return m_animation.IsFinished(); }
	void Update(float deltaSeconds);
	const void Render(Vector2 scale);

	Vector2 m_position;
	float m_radius;
	SpriteAnim m_animation;
	Rgba m_color;
	static const std::string m_explosion_file_location;
};

#endif