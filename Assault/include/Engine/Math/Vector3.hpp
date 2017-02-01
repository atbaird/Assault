#pragma once
#include <Math.h>

#ifndef VECTOR3_HPP
#define VECTOR3_HPP

class Vector3
{
public:
	Vector3();
	Vector3(const Vector3& other);
	Vector3(float x, float y, float z);

	const float normalize();

	const float dotProduct(Vector3& other);

	const Vector3 operator*(const float&);
	const Vector3 operator/(const float&);

	float m_x;
	float m_y;
	float m_z;
};
#endif // !VECTOR3_HPP
