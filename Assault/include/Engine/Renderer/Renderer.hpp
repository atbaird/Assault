#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/gl.h>
#include <stdlib.h> 
#include "Engine/Math/ExtendedMath.hpp"
#include "Engine/Renderer/Rgba.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Renderer/Texture.hpp"

//-----------------------------------------------------
class Renderer;

//-----------------------------------------------------
extern Renderer* g_Renderer;

class Renderer
{
public:
	//constructor
	static const Rgba White;
	static const Rgba Black;
	static const Rgba Blue;
	static const Rgba Gray;
	static const Rgba Red;
	static const Rgba Green;
	Renderer();

	//Render shape
	void DrawQuad(const Vector2& position0, const Vector2& position1, const Vector2& position2, const Vector2& position3, const Rgba& color);
	void DrawAABB(const AABB2& bounds, const Rgba& color);
	void DrawTexturedAABB(const AABB2& bounds, const Texture* texture, const Vector2& texCoordMins, const Vector2& texCoordMaxs, const Rgba& tint);
	void DrawTexturedAndBumpMappedAABB(const AABB2& bounds, const AABB2& textBounds, const Texture* texture, const Texture* normalMap, const Rgba& tint);
	void RenderPolygon(const Vector2& center, float radius, int numSides, float degreesOffSet, Rgba color, bool isDotted = false);
	void RenderLine(const Vector2& startPoint, const Vector2& endPoint, Rgba color, float lineThickness);

	//Window and screen
	void CreateOpenGLWindow(HINSTANCE& applicationInstanceHandle, const char*& APP_NAME, const int& OFFSET_FROM_WINDOWS_DESKTOP, const int& WINDOW_PHYSICAL_WIDTH,
		const int& WINDOW_PHYSICAL_HEIGHT, HWND& g_hWnd, HDC& g_displayDeviceContext, HGLRC g_openGLRenderingContext, const double& VIEW_LEFT, const double& VIEW_RIGHT, 
		const double& VIEW_TOP, const double& VIEW_BOTTOM, WNDCLASSEX& windowClassDescription);
	void SetOrtho(const Vector2& bottomLeft, const Vector2& topRight);
	void clearScreen(Rgba color);
	void Translate(Vector2 translateBy);
	void Rotate(float degrees);
	void Push();
	void Pop();
	void screenShake(float shakeMagnitude);
	void updateCameraPosition(const double& VIEW_LEFT, const double& VIEW_RIGHT, const double& VIEW_BOTTOM, const double& VIEW_TOP);
};