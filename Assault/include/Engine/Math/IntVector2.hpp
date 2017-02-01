#pragma once
#include <Math.h>

#ifndef INTVECTOR2_HPP
#define INTVECTOR2_HPP

class IntVector2
{
public:
	//constructors
	IntVector2();
	IntVector2(int newX, int newY);

	float getLength();


	const bool operator==(const IntVector2&);
	const bool operator!=(const IntVector2&);

	//variables
	int x;
	int y;
};


#endif // !INTVECTOR2_HPP