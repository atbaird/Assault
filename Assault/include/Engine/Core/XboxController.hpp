#pragma once

#include <Windows.h>
#include <Xinput.h> // include the Xinput API
#include <Math.h>
#include "Engine/Math/Vector2.hpp"
#pragma comment( lib, "xinput9_1_0" ) // Link in the xinput.lib static library #Eiserloh: Explicitly use XInput v9.1.0, since v1.4 is not supported under Windows 7

//XboxController base inputs returns:
//Analog Sticks return numbers between -32768 and +32768
//triggers return numbers between 0 and 255

//what this class will return instead for analog sticks and triggers:
//-1 to 1 for analog, 0 to 1 for triggers.

class XboxController
{
public:
	//constructors
	XboxController();
	XboxController(int controllerNumber);

	//plugged in
	int amIPluggedIn();
	DWORD amIPluggedInDWORD();

	//updates
	void Update(float deltaSeconds);


	//getters
	Vector2 getLeftAnalogPosition();
	Vector2 getRightAnalogPosition();
	float getRightTriggerPosition();
	float getLeftTriggerPosition();
	bool isLeftBumperDown();
	bool isRightBumperDown();

	bool isDPadUpDown();
	bool isDPadDownDown();
	bool isDPadLeftDown();
	bool isDPadRightDown();

	bool isAButtonDown();
	bool isBButtonDown();
	bool isXButtonDown();
	bool isYButtonDown();
	bool isStartButtonDown();
	bool isBackButtonDown();
	bool isLeftAnalogPressedDown();
	bool isRightAnalogPressedDown();

	//variables
private:
	int whatControllerAmI;
	XINPUT_STATE xboxControllerState;
};