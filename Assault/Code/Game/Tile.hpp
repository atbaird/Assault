#pragma once

#include "Engine/Renderer/Renderer.hpp"
#include "Game/TileCoords.hpp"
#include "Engine/Math/Vector2.hpp"

#ifndef TILE_HPP
#define TILE_HPP

enum TileType
{
	TILE_INVALID = -1,
	TILE_GRASS,
	TILE_ROCK,
	NUM_TILE_TYPES
};

struct Tile
{
public:
	//Constructors and Destructor
	Tile();
	virtual ~Tile();

	void Render() const;

public:
	//Getters
	TileType GetTileType() const;
	Vector2 getWorldCoords() const;

	//setters
	void setByTileType(TileType newType);
	void setTileID(int id);
	void setWorldCoords(Vector2 position);

	//variables
	TileType m_TileType;
	Vector2 m_worldCoords;
private:
	static const std::string s_rockTypeFile;
	static const std::string s_grassTypeFile;
};

#endif // !TILE_HPP
