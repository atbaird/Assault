#include "Engine/Renderer/Renderer.hpp"

Renderer* g_Renderer = nullptr;

//default colors
const Rgba Renderer::White = Rgba(1.f, 1.f, 1.f, 1.f);
const Rgba Renderer::Black = Rgba(0.f, 0.f, 0.f, 1.f);
const Rgba Renderer::Blue = Rgba(0.f, 0.f, 1.f, 1.f);
const Rgba Renderer::Gray = Rgba(0.5f, 0.5f, 0.5f, 1.f);
const Rgba Renderer::Red = Rgba(1.f, 0.f, 0.f, 1.f);
const Rgba Renderer::Green = Rgba(0.f, 1.f, 0.f, 1.f);

Renderer::Renderer()
{

}

void Renderer::DrawAABB(const AABB2& bounds, const Rgba& color)
{
	//draws a square.
	glColor4f(color.Red, color.Green, color.Blue, color.Alpha);
	glBegin(GL_QUADS);
	{
		glVertex2f(bounds.mins.x, bounds.mins.y);
		glVertex2f(bounds.maxs.x, bounds.mins.y);
		glVertex2f(bounds.maxs.x, bounds.maxs.y);
		glVertex2f(bounds.mins.x, bounds.maxs.y);
	}
	glEnd();
}

void Renderer::DrawTexturedAABB(const AABB2& bounds, const Texture* texture, const Vector2& texCoordMins, const Vector2& texCoordMaxs, const Rgba& tint)
{
	//draws a textured AABB, aka square,
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture->m_openglTextureID);

	glColor4f(tint.Red, tint.Green, tint.Blue, tint.Alpha);
	glBegin(GL_QUADS);
	{

		glTexCoord2f(texCoordMins.x, texCoordMaxs.y);
		glVertex2f(bounds.mins.x, bounds.mins.y);

		glTexCoord2f(texCoordMaxs.x, texCoordMaxs.y);
		glVertex2f(bounds.maxs.x, bounds.mins.y);

		glTexCoord2f(texCoordMaxs.x, texCoordMins.y);
		glVertex2f(bounds.maxs.x, bounds.maxs.y);

		glTexCoord2f(texCoordMins.x, texCoordMins.y);
		glVertex2f(bounds.mins.x, bounds.maxs.y);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void Renderer::DrawTexturedAndBumpMappedAABB(const AABB2& bounds, const AABB2& textBounds, const Texture* texture, const Texture* normalMap, const Rgba& tint)
{
	//draws a textured AABB, aka square,
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture->m_openglTextureID);

	//need normals of bounds.
	Vector2 mins2 = Vector2(bounds.mins.x, bounds.maxs.y);
	Vector2 maxs2 = Vector2(bounds.maxs.x, bounds.mins.y);
	float normalMax1 = ((Vector2)(bounds.maxs)).normalize();
	float normalMax2 = (maxs2).normalize();
	float normalMins1 = ((Vector2)(bounds.mins)).normalize();
	float normalMins2 = (mins2).normalize();

	glColor4f(tint.Red, tint.Green, tint.Blue, tint.Alpha);
	glBegin(GL_QUADS);
	{

		glTexCoord2f(textBounds.mins.x, textBounds.maxs.y);
		glVertex2f(bounds.mins.x, bounds.mins.y);

		glTexCoord2f(textBounds.maxs.x, textBounds.maxs.y);
		glVertex2f(bounds.maxs.x, bounds.mins.y);

		glTexCoord2f(textBounds.maxs.x, textBounds.mins.y);
		glVertex2f(bounds.maxs.x, bounds.maxs.y);

		glTexCoord2f(textBounds.mins.x, textBounds.mins.y);
		glVertex2f(bounds.mins.x, bounds.maxs.y);
	}
	glEnd();


	glBindTexture(GL_TEXTURE_2D, normalMap->m_openglTextureID);
	glBegin(GL_QUADS);
	{

		glTexCoord2f(textBounds.mins.x, textBounds.maxs.y);
		glVertex2f(bounds.mins.x, bounds.mins.y);

		glTexCoord2f(textBounds.maxs.x, textBounds.maxs.y);
		glVertex2f(bounds.maxs.x, bounds.mins.y);

		glTexCoord2f(textBounds.maxs.x, textBounds.mins.y);
		glVertex2f(bounds.maxs.x, bounds.maxs.y);

		glTexCoord2f(textBounds.mins.x, textBounds.mins.y);
		glVertex2f(bounds.mins.x, bounds.maxs.y);
	}
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void Renderer::DrawQuad(const Vector2& position0, const Vector2& position1, const Vector2& position2, const Vector2& position3, const Rgba& color)
{
	//Literally just draws a square of color
	glColor4f(color.Red, color.Green, color.Blue, color.Alpha);
	glBegin(GL_QUADS);
	{
		glVertex2f(position0.x, position0.y);
		glVertex2f(position1.x, position1.y);
		glVertex2f(position2.x, position2.y);
		glVertex2f(position3.x, position3.y);
	}
	glEnd();
}

void Renderer::RenderPolygon(const Vector2& center, float radius, int numSides, float degreesOffSet, Rgba color, bool isDotted )
{
	//renders a polygon shape of the given number of sides
	//const float radiansTotal = 2.f * ExtendMath->pi;
	const float radiansTotal = 2.f * pi;
	const float radiansPerSide = radiansTotal / numSides;
	float red = 0.f;
	float green = 0.f;
	float blue = 0.f;
	float alpha = 0.f;
	color.GetRGBA(red, green, blue, alpha);
	glColor4f(red, green, blue, alpha);
	GLenum primitiveMode = isDotted ? GL_LINES : GL_LINE_LOOP;
	glBegin(primitiveMode);
	{
		for (float radians = 0.f; radians < radiansTotal; radians += radiansPerSide) {
			//float rotatedRadians = (0.5f * pi) + radians;
			float rotatedRadians = radians + DegreesToRadians(degreesOffSet);
			//float rotatedRadians = radians + DegreesToRadians(degreesOffSet);
			float x = (cos(rotatedRadians) * radius) + center.x;
			float y = (sin(rotatedRadians) * radius) + center.y;
			glVertex2f(x, y);
		}
	}
	glEnd();
}

void Renderer::RenderLine(const Vector2& startPoint, const Vector2& endPoint, Rgba color, float lineThickness)
{
	//draws a straight line from the startPoint to the endPoint
	float angle = atan2(endPoint.y - startPoint.y, endPoint.x - startPoint.x);
	glColor4f(color.Red, color.Green, color.Blue, color.Alpha);
	glLineWidth(lineThickness);
	glBegin(GL_LINE_LOOP);
	{
		glVertex2f(startPoint.x, startPoint.y);
		glVertex2f(endPoint.x, endPoint.y);
	}
	glEnd();
}

void Renderer::CreateOpenGLWindow(HINSTANCE& applicationInstanceHandle, const char*& APP_NAME, const int& OFFSET_FROM_WINDOWS_DESKTOP, const int& WINDOW_PHYSICAL_WIDTH,
	const int& WINDOW_PHYSICAL_HEIGHT, HWND& g_hWnd, HDC& g_displayDeviceContext, HGLRC g_openGLRenderingContext, const double& VIEW_LEFT, const double& VIEW_RIGHT,
	const double& VIEW_TOP, const double& VIEW_BOTTOM, WNDCLASSEX& windowClassDescription)
{
	//method handles all the disgustingly bad parts of making the app window, just pass it the variables and it you won't have to see how ugly this code really is.

	const DWORD windowStyleFlags = WS_CAPTION | WS_BORDER | WS_THICKFRAME | WS_SYSMENU | WS_OVERLAPPED;
	const DWORD windowStyleExFlags = WS_EX_APPWINDOW;

	RECT desktopRect;
	HWND desktopWindowHandle = GetDesktopWindow();
	GetClientRect(desktopWindowHandle, &desktopRect);

	RECT windowRect = { OFFSET_FROM_WINDOWS_DESKTOP, OFFSET_FROM_WINDOWS_DESKTOP, OFFSET_FROM_WINDOWS_DESKTOP + WINDOW_PHYSICAL_WIDTH,
		OFFSET_FROM_WINDOWS_DESKTOP + WINDOW_PHYSICAL_HEIGHT };
	AdjustWindowRectEx(&windowRect, windowStyleFlags, FALSE, windowStyleExFlags);

	WCHAR windowTitle[1024];
	MultiByteToWideChar(GetACP(), 0, APP_NAME, -1, windowTitle, sizeof(windowTitle) / sizeof(windowTitle[0]));
	g_hWnd = CreateWindowEx(
		windowStyleExFlags,
		windowClassDescription.lpszClassName,
		windowTitle,
		windowStyleFlags,
		windowRect.left,
		windowRect.top,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL,
		NULL,
		applicationInstanceHandle,
		NULL);

	ShowWindow(g_hWnd, SW_SHOW);
	SetForegroundWindow(g_hWnd);
	SetFocus(g_hWnd);

	g_displayDeviceContext = GetDC(g_hWnd);

	HCURSOR cursor = LoadCursor(NULL, IDC_ARROW);
	SetCursor(cursor);

	PIXELFORMATDESCRIPTOR pixelFormatDescriptor;
	memset(&pixelFormatDescriptor, 0, sizeof(pixelFormatDescriptor));
	pixelFormatDescriptor.nSize = sizeof(pixelFormatDescriptor);
	pixelFormatDescriptor.nVersion = 1;
	pixelFormatDescriptor.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pixelFormatDescriptor.iPixelType = PFD_TYPE_RGBA;
	pixelFormatDescriptor.cColorBits = 24;
	pixelFormatDescriptor.cDepthBits = 24;
	pixelFormatDescriptor.cAccumBits = 0;
	pixelFormatDescriptor.cStencilBits = 8;

	int pixelFormatCode = ChoosePixelFormat(g_displayDeviceContext, &pixelFormatDescriptor);
	SetPixelFormat(g_displayDeviceContext, pixelFormatCode, &pixelFormatDescriptor);
	g_openGLRenderingContext = wglCreateContext(g_displayDeviceContext);
	wglMakeCurrent(g_displayDeviceContext, g_openGLRenderingContext);

	//Ortho = orthographic; aka, not 3D. used for 2D games to set up coordinate system.
	//glOrtho(0.f, 1000.f, 10.f, 0.f,0.f,1.f);
	glOrtho(VIEW_LEFT, VIEW_RIGHT, VIEW_BOTTOM, VIEW_TOP, 0.f, 1.f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLineWidth(2.f);
	glEnable(GL_LINE_SMOOTH);
}
void Renderer::SetOrtho(const Vector2& bottomLeft, const Vector2& topRight)
{
	//sets the general screen location.
	glLoadIdentity();
	glOrtho(bottomLeft.x, topRight.x, bottomLeft.y, topRight.y, 0.f, 1.f);
}

void Renderer::Translate(Vector2 translateBy)
{
	//translates the screen position
	glTranslatef(translateBy.x, translateBy.y, 0.f);
}

void Renderer::Rotate(float degrees)
{
	//rotates the screen by degrees
	glRotatef(degrees, 0.f, 0.f, 1.f);
}

void Renderer::Push()
{
	//Pushes in the present screen orientation
	glPushMatrix();
}
void Renderer::Pop()
{
	//pops out the present screen orientation
	glPopMatrix();
}

void Renderer::screenShake(float shakeMagnitude)
{
	//suppose to be used to shake the screen.
	float trueDisplacementX = 10.f + (rand() * shakeMagnitude);
	float trueDisplacementY = 10.f + (rand() * shakeMagnitude);
	glTranslatef(trueDisplacementX, trueDisplacementY,0.f);
}
void Renderer::updateCameraPosition(const double& VIEW_LEFT, const double& VIEW_RIGHT, const double& VIEW_BOTTOM, const double& VIEW_TOP)
{
	//updates the camera's position
	glOrtho(VIEW_LEFT, VIEW_RIGHT, VIEW_BOTTOM, VIEW_TOP, 0.f, 1.f);
}

void Renderer::clearScreen(Rgba color)
{
	//generates a background screen color so as to prevent a hall of mirrors effect.
	glClearColor(color.Red, color.Green, color.Blue, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);
}