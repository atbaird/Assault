#pragma once
#include <Math.h>

//-------------------------------------------------------------------------------------------
class Vector2{
	//variables
	public:
		float x;
		float y;

	public:
		//constructors
		Vector2();
		Vector2(const Vector2&);
		Vector2(float, float);

		//setters
		void SetXY(float x, float y);

		//Getters
		void GetXY(float& x_out, float& y_out) const;
		float GetLength() const;

		//functions
		const float normalize();
		const Vector2 operator+(const Vector2&);
		const Vector2 operator-(const Vector2&);
		const Vector2 operator*(const float&);
		const Vector2 operator/(const float&);
		const bool operator==(const Vector2&);
		const void operator+=(const Vector2&);

		//special functions
		const Vector2 crossProduct(const Vector2&);
		const Vector2 projection(const Vector2&);
		const float dotProduct(const Vector2&);
		const bool orthogonalTest(const Vector2&);
};