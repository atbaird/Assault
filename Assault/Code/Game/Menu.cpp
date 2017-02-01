#include "Game/Menu.hpp"

Menu::Menu(XboxController* controller)
: m_playerController(controller),
m_pageNum(0),
m_maxPages(2),
isSpaceOrXPresentlyDown(false),
justCreated(true),
m_WholeScreenBoundries(Vector2(0.f,0.f), Vector2(16.f,9.f))
{
	m_keyboard_controls = Texture::CreateOrGetTexture("Data/Images/Keyboard Controls Screen.png");
	m_xbox_controls = Texture::CreateOrGetTexture("Data/Images/Xbox Controls Screens.png");
}
Menu::~Menu()
{

}

const void Menu::Render()
{
	g_Renderer->DrawAABB(m_WholeScreenBoundries, g_Renderer->Gray);
	switch (m_pageNum)
	{
	case 0:
		g_Renderer->DrawTexturedAABB(m_WholeScreenBoundries, m_keyboard_controls, Vector2(0.f, 0.f), Vector2(1.f, 1.f), g_Renderer->White);
		break;
	case 1:
		g_Renderer->DrawTexturedAABB(m_WholeScreenBoundries, m_xbox_controls, Vector2(0.f, 0.f), Vector2(1.f, 1.f), g_Renderer->White);
		break;
	}
}
void Menu::Update(bool spaceDown)
{
	if (justCreated == false && isSpaceOrXPresentlyDown == false && (spaceDown == true || m_playerController->isXButtonDown() == true))
	{
		isSpaceOrXPresentlyDown = true;
		m_pageNum += 1;
	}

	if (spaceDown == false && m_playerController->isXButtonDown() == false)
	{
		isSpaceOrXPresentlyDown = false;
	}
	if (justCreated == true)
	{
		justCreated = false;
	}
}


int Menu::getPageNum()
{
	return m_pageNum;
}
int Menu::getMaxPages()
{
	return m_maxPages;
}
void Menu::setCurrentPage(int curPage)
{
	m_pageNum = curPage;
}