#include "Game/TheApp.hpp"

//--------------------------------------------
TheApp* g_theApp = nullptr;


//--------------------------------------------
//constructor and destructor
TheApp::TheApp()
{
	//Set all keys to "not down"
	for (int keyIndex = 0; keyIndex < NUM_KEYS; ++keyIndex)
	{
		m_isKeyDown[keyIndex] = false;
	}
}
TheApp::~TheApp()
{
	//Destructor method
}


//-----------------------------------
//setters
void TheApp::setKeyDownStatus(unsigned char asKey, bool isNowDown)
{
	//this method is to be used to update whether a particular key is being held down or not.
	m_isKeyDown[asKey] = isNowDown;
}

//----------------------------------
//getters
bool TheApp::isKeyDown(unsigned char asKey)
{
	//this method returns whether a particular key is currently being held down or not
	return m_isKeyDown[asKey];
}