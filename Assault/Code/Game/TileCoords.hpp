#pragma once

#include "Engine/Math/IntVector2.hpp"

#ifndef TILECOORDS_HPP
#define TILECOORDS_HPP

class TileCoords : public IntVector2
{
	//ATTENTION: This class is just to act as a casing for IntVector2
	//It is LITERALLY just IntVector2, and intended so theres that extra indication as to what this is intended to be used for.
public:
	TileCoords();
	TileCoords(int xVal, int yVal);
};

#endif // !TILECOORDS_HPP
