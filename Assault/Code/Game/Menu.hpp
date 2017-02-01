#pragma once
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Core/XboxController.hpp"

#ifndef MENU_HPP
#define MENU_HPP

class Menu
{
public:
	Menu(XboxController* controller);
	~Menu();
	void Update(bool spaceDown);
	const void Render();
	int getPageNum();
	int getMaxPages();
	void setCurrentPage(int curPage);

private:
	int m_pageNum;
	int m_maxPages;
	XboxController* m_playerController;
	AABB2 m_WholeScreenBoundries;
	bool justCreated;
	bool isSpaceOrXPresentlyDown;
	Texture* m_keyboard_controls;
	Texture* m_xbox_controls;
};
#endif // !MENU_HPP
