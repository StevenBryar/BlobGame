#ifndef Tile_H
#define Tile_H

#include "GameObject.h"

class Tile : public GameObject{
public:
	Tile(unsigned int tileTypes);
	~Tile();

	void update(double delta);

	void setTileTypes(unsigned int types);
    unsigned int getTileTypes();
	std::string getType();
protected:
	unsigned int m_TileTypes;
	static int m_TileCount;
};
#endif