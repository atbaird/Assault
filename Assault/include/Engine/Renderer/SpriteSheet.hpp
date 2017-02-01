#pragma once
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Math/AABB2.hpp"

#ifndef SPRITESHEET_HPP
#define SPRITESHEET_HPP

class SpriteSheet
{
public:
	SpriteSheet();
	SpriteSheet(const std::string& imageFilePath, int tilesWide, int tilesHigh);
	AABB2 GetTexCoordsForSpriteCoords(int spriteX, int spriteY) const; // mostly for atlases
	AABB2 GetTexCoordsForSpriteIndex(int spriteIndex) const; // mostly for sprite animations
	Texture* getSpriteSheetTexture() const;
	int GetNumSprites() const;
	void changeSpriteSheet(const std::string& imageFilePath, int tilesWide, int tilesHigh);
	//... and other member functions as needed

private:
	Texture* 	m_spriteSheetTexture;	// Image with grid-based layout of sub-images
	IntVector2	m_spriteLayout;		// # of sprites across, and down, on the sheet
	//... and other data member variables as needed

};

#endif // !SPRITESHEET_HPP
