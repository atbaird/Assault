#include "Game/Explosion.hpp"

const std::string Explosion::m_explosion_file_location = "Data/Images/Explosion_5x5.png";

Explosion::Explosion()
: m_position(Vector2(0.f,0.f)),
m_radius(1.f),
m_color(1.f,1.f,1.f,1.f)
{
	SpriteSheet spriteSheet = SpriteSheet(m_explosion_file_location, 5, 5);
	m_animation = SpriteAnim(spriteSheet, 1.f, SPRITE_ANIM_MODE_PLAY_TO_END, 0, 25);
}
Explosion::Explosion(const Explosion& other)
: m_position(other.m_position),
m_radius(other.m_radius),
m_color(other.m_color)
{
	SpriteSheet spriteSheet = SpriteSheet(m_explosion_file_location, 5, 5);
	m_animation = SpriteAnim(spriteSheet, 1.f, SPRITE_ANIM_MODE_PLAY_TO_END, 0, 25);
}
Explosion::~Explosion()
{

}

void Explosion::Update(float deltaSeconds)
{
	m_animation.Update(deltaSeconds);
}
const void Explosion::Render(Vector2 scale)
{
	g_Renderer->Push();
	g_Renderer->Translate(Vector2(m_position.x*scale.x, m_position.y * scale.y));
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	m_animation.Render(AABB2(Vector2(-m_radius * scale.x, -m_radius*scale.y), Vector2(m_radius*scale.x, m_radius*scale.y)), 0.f, m_color);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	g_Renderer->Pop();
}