#pragma once
#include "Engine/Math/Vector2.hpp"

#ifndef AABB2_HPP
#define AABB2_HPP

class AABB2
{
public:
	//constructors
	AABB2();
	AABB2(float minx, float miny, float maxx, float maxy);
	AABB2(const AABB2& other);
	explicit AABB2(const Vector2& initialMins, const Vector2& initialMaxs);

	//getters
	void GetMinsMaxs(Vector2* out_mins, Vector2* out_maxs) const;

	//mutators
	void setMinsMaxs(const Vector2& newMins, const Vector2& newMaxs);
	
	//variables
	Vector2 mins;
	Vector2 maxs;
	//use this to store the minimum and maximum corners for the tank.
	static const AABB2 AABB2::ZERO_TO_ONE;
};

#endif // !AABB2_HPP
