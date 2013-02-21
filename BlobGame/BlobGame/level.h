#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include <string>

class Tile;
class GameObject;

class Level
{
public:
	Level(unsigned int horizontalTiles, unsigned int verticalTiles, 
		unsigned int tileSize,const int tileTypes[],
		std::vector<GameObject*>* objects,
		GameObject*(factory)(unsigned int flags,Tile* tile));

	virtual ~Level();

	//Standard lifecycle methods
	virtual void update(double delta);

	std::string getName();
	void setName(std::string name);

	//Tile count methods
	unsigned int getNumberOfHorizontalTiles();
	unsigned int getNumberOfVerticalTiles();

	//Validate that the tile coordinates passed in are valid
	bool validateTileCoordinates(int coordinatesX, int coordinatesY);

	//Converts a position in screen space into a position in coordinate space
	int getTileCoordinateForPosition(int position);

	//Index methods
	int getTileIndexForPosition(int positionX, int positionY);
	int getTileIndexForCoordinates(int coordinatesX, int coordinatesY);
	int getTileIndexForTile(Tile* tile);

	//Tile methods
	Tile* getTileForPosition(int positionX, int positionY);
	Tile* getTileForCoordinates(int coordinatesX, int coordinatesY);
	Tile* getTileForTileIndex(int index);

	std::vector<Tile*>* getTiles();

	void changeTile(Tile* replacementTile);
protected:
	//Member variables
	std::vector<Tile*>* m_Tiles;
	unsigned int m_HorizontalTiles;
	unsigned int m_VerticalTiles;
	unsigned int m_TileSize;
	std::string m_Name;
};

#endif