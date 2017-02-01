#pragma once
#include "Engine/Renderer/SpriteSheet.hpp"
#include "Engine/Renderer/Renderer.hpp"

#ifndef SPRITEANIM_HPP
#define SPRITEANIM_HPP

enum SpriteAnimMode
{
	SPRITE_ANIM_MODE_PLAY_TO_END = 0, //play from time=0 to durationSeconds, then finish
	SPRITE_ANIM_MODE_LOOPING,     //Play from time=0 to end then repeat (never finish)
	NUM_SPRITE_ANIM_MODES
};

class SpriteAnim
{
public:
	SpriteAnim();
	SpriteAnim(const SpriteSheet& spriteSheet, float durationSeconds, SpriteAnimMode mode, int startSpriteIndex, int endSpriteIndex);

	void Update(float deltaSeconds);
	const void Render(AABB2 positionToDraw, float m_angularDisplacementDegrees, Rgba tint);
	AABB2 GetCurrentTexCoords() const; //based on the current elapsed time
	Texture* GetTexture() const;
	void Pause();
	void Resume();
	void Reset();
	void Reverse();
	bool IsFinished() const { return m_isFinished; };
	bool IsPlaying() const { return m_isPlaying; };
	bool IsReversed() const { return m_reverse; };
	float GetDurationSeconds() const { return m_durationSeconds; };
	float GetSecondsElapsed() const { return m_elapsedSeconds; };
	float GetSecondsRemaining() const;
	float GetFractionElapsed() const;
	float GetFractionRemaining() const;
	float SetSecondsElapsed(float secondsElapsed); // jump to specific time
	float SetFractionElapsed(float fractionElapsed); //e.g. 0.33f for one-third in

	SpriteSheet m_spriteSheet;
	SpriteAnimMode m_mode;
	float m_durationSeconds;
	float m_elapsedSeconds;
	float m_elapsedSecondsSinceLastFrameSwitch;
	float m_durationSecondsBetweenFrames;
	int m_startSpriteIndex;
	int m_endSpriteIndex;
	int m_currentSpriteIndex;
	bool m_isFinished;
	bool m_isPlaying;
	bool m_reverse;
};

#endif