#include "Game/Tile.hpp"


const std::string Tile::s_rockTypeFile = "Data/Images/tileable_rock_00.png";
const std::string Tile::s_grassTypeFile = "Data/Images/tileable_grass_00.png";
//Constructors and Deconstructors
Tile::Tile()
: m_worldCoords(Vector2(0.f, 0.f))
, m_TileType(TILE_INVALID)
{
	//constructor
	
}
Tile::~Tile()
{
	//deconstructor
}

void Tile::Render() const
{
	//Rgba tint = GetColorForTileType(m_type);
	//AABB2 
}

//Getters
TileType Tile::GetTileType() const
{
	return m_TileType;
}
Vector2 Tile::getWorldCoords() const
{
	//returns the tile's worldCoordinates
	return m_worldCoords;
}

//Setters

void Tile::setByTileType(TileType newType)
{
	m_TileType = newType;
}

void Tile::setWorldCoords(Vector2 position)
{
	//sets the world coordinates
	m_worldCoords = position;
}