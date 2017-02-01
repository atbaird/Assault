#pragma once
#include "Engine/Math/Vector2.hpp"
#include <stdlib.h>

float DegreesToRadians(float);
float RadiansToDegrees(float);
void generateVertexPositionsForPolygon(const Vector2& center, float radius, int numSides, float degreesOffSet, Vector2*& pointsToReturn);
float GetRandomFloatZeroToOne();
float GetRandFloatMinToMax(float min, float max);
float CosDegrees(float degrees);
float SinDegrees(float degrees);
float ComputeShortestAngularDisplaement(float fromDegrees, float toDegrees);

const float pi = 3.14159265f;
