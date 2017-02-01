#pragma once

#include "Game/LivingEntity.hpp"
#include "Engine/Core/XboxController.hpp"

#ifndef TANK_HPP
#define TANK_HPP

class Tank : public LivingEntity
{
public:
	//Constructor and Destructor
	Tank();
	~Tank();

	//Update and Render
	void PlayerUpdate(float deltaSeconds, bool W, bool A, bool D,bool S);
	void setPlayerID(int id);
	void assignController(XboxController* controller);
	void Update(float deltaSeconds);
	void UpdateVelocityFromKeyboard();
	const void Render(Vector2& scale);

	//controls
	bool keyboardControls(float& deltaSeconds, bool& W, bool& A, bool& D, bool& S);
	bool xboxControls(float deltaSeconds);

	//variables
	Texture* tankTexture;
	XboxController* m_controller;
	int m_id;
	float m_deltaSecondsForRightTreds;
	float m_deltaSecondsForLeftTreds;
	SpriteAnim leftTred;
	SpriteAnim rightTred;
};

#endif // !TANK_HPP
