#pragma once

#ifndef THEAPP_HPP
#define THEAPP_HPP

//-----------------------------------------------------
const int NUM_KEYS = 256;

//-----------------------------------------------------
class TheApp;

//-----------------------------------------------------
extern TheApp* g_theApp;

//-----------------------------------------------------
class TheApp
{
public:
	//Constructor and Destructor
	TheApp();
	~TheApp();

	//setters
	void setKeyDownStatus(unsigned char asKey, bool isNowDown);

	//getters
	bool isKeyDown(unsigned char asKey);

	//variables
private:

	bool m_isKeyDown[NUM_KEYS];

};

#endif // !THEAPP_HPP