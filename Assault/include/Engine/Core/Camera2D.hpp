#pragma once
#include "Engine/Renderer/Renderer.hpp"

#ifndef CAMERA2D_HPP
#define CAMERA2D_HPP

class Camera2D
{
public:
	//Constructors and Destructor
	Camera2D(Vector2 position, bool isActive);
	virtual ~Camera2D();

	//Update
	void CameraUpdate(float deltaSeconds, Vector2 position, float angularDisplace); // for following something
	const void Render();

	//setters
	void ChangePosition(Vector2 position);
	void SetActive(bool isActive);
	void SetXYAngularDisplacement(float angularDisplace);

	//getters
	float getXYANgularDisplacement();

	//variables
	Vector2 m_position;
	Vector2 m_screen_size;
	float xy_Angular_Displacement;
	bool m_active;
};

#endif // !CAMERA2D_HPP
