#include "Engine/Renderer/SpriteSheet.hpp"

SpriteSheet::SpriteSheet()
: m_spriteLayout(0, 0)
{
	m_spriteSheetTexture = nullptr;
}
SpriteSheet::SpriteSheet(const std::string& imageFilePath, int tilesWide, int tilesHigh)
: m_spriteLayout(tilesWide, tilesHigh)
{
	m_spriteSheetTexture = Texture().CreateOrGetTexture(imageFilePath);
}
AABB2 SpriteSheet::GetTexCoordsForSpriteCoords(int spriteX, int spriteY) const // mostly for atlases
{
	AABB2 coordinates = AABB2();

	float AverageWidth = (1.f / m_spriteLayout.x);
	float AverageHeight = (1.f / m_spriteLayout.y);

	coordinates.mins = Vector2(AverageWidth * spriteX, AverageHeight * spriteY);
	coordinates.maxs = Vector2(coordinates.mins.x + AverageWidth, coordinates.mins.y + AverageHeight);

	return coordinates;
}
AABB2 SpriteSheet::GetTexCoordsForSpriteIndex(int spriteIndex) const // mostly for sprite animations
{
	AABB2 coordinates = AABB2();

	float AverageWidth = (1.f / m_spriteLayout.x);
	float AverageHeight = (1.f / m_spriteLayout.y);


	int spriteY = (int)(floor(spriteIndex / (m_spriteLayout.x)));
	int spriteX = spriteIndex;
	if (spriteY != 0)
	{
		spriteX = (spriteIndex % (int)(spriteY * m_spriteLayout.x));
	}

	coordinates.mins = Vector2(AverageWidth * spriteX, AverageHeight * spriteY);
	coordinates.maxs = Vector2(coordinates.mins.x + AverageWidth, coordinates.mins.y + AverageHeight);

	return coordinates;
}

Texture* SpriteSheet::getSpriteSheetTexture() const
{
	return m_spriteSheetTexture;
}
int SpriteSheet::GetNumSprites() const
{
	return m_spriteLayout.x * m_spriteLayout.y;
}

void SpriteSheet::changeSpriteSheet(const std::string& imageFilePath, int tilesWide, int tilesHigh)
{
	m_spriteSheetTexture = Texture().CreateOrGetTexture(imageFilePath);
	m_spriteLayout = IntVector2(tilesWide, tilesHigh);
}