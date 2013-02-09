#include "level.h"
#include <stdlib.h>
#include "Tile.h"
#include "gameObject.h"
#include "common.h"
#include "Constants.h"
#include "BlobGameConstants.h"
#include "unit.h"
#include "blob.h"
#include "EnemyAttacks.h"
#include "Enemy.h"
#include "spriteManager.h"
#include "2dSprite.h"
#include "Abilities.h"

Level::Level(unsigned int horizontalTiles, unsigned int verticalTiles, 
				unsigned int tileSize,const int tileTypes[],
				std::vector<GameObject*>* objects,
				GameObject*(factory)(unsigned int flags,Tile* tile)){
					m_Tiles = new std::vector<Tile*>();
	m_HorizontalTiles = horizontalTiles;
	m_VerticalTiles = verticalTiles;
	m_TileSize = tileSize;

	int tileIndex = 0;
	float tileX = 0.0f;
	float tileY = 0.0f;
	//Cycle through all the tiles and create them
	for(int v = 0; v < getNumberOfVerticalTiles(); v++){
		for(int h = 0; h < getNumberOfHorizontalTiles(); h++){
			m_Tiles->push_back(new Tile
					(tileTypes[tileIndex]));
			//Set the tile position and size
			(*m_Tiles)[tileIndex]->setPosition(tileX, tileY);
			(*m_Tiles)[tileIndex]->setSize(TILE_SIZE, TILE_SIZE);
			(*m_Tiles)[tileIndex]->setOrigin((*m_Tiles)[tileIndex]->getWidth()/2,
											(*m_Tiles)[tileIndex]->getHeight()/2);
			//Increment the tile index
			tileIndex++;
			//And increment the tile x position
			tileX += TILE_SIZE;
		}
		//Increment the tile y position and reset the tile x position, since we started a new row
		tileY += TILE_SIZE;
		tileX = 0.0f;
	}
	//cycle through tileTypes array.
	//look for gameObject indicators
	//create gameObjects needed.
	if(objects){
		for(int i = 0;i < getNumberOfVerticalTiles()*
							getNumberOfHorizontalTiles();i++){
			GameObject* gameObject;
			if(gameObject = factory(tileTypes[i],(*m_Tiles)[i])){
				unsigned int tiletypes = (*m_Tiles)[i]->getTileTypes();
				RemoveFlag(&tiletypes,TILE_SPAWN_FLAGS);
				(*m_Tiles)[i]->setTileTypes(tiletypes);
				objects->push_back(gameObject);
			}
		}
	}
}

Level::~Level(){
	SafeVectorDelete<Tile>(*m_Tiles);
	SafePtrRelease(m_Tiles);
}

void Level::update(double delta){
	//Update all the game tiles
	for(int i = 0; i < getNumberOfHorizontalTiles() * 
		getNumberOfVerticalTiles(); i++)
		if((*m_Tiles)[i] != NULL)
			(*m_Tiles)[i]->update(delta);
}

bool Level::validateTileCoordinates(int coordinatesX, int coordinatesY){
    if(coordinatesX < 0 || coordinatesY < 0 || coordinatesX >= 
		getNumberOfHorizontalTiles() || coordinatesY >= getNumberOfVerticalTiles()) {
        return false;
    } 
    return true;
}
std::vector<Tile*>* Level::getTiles(){
	return m_Tiles;
}

unsigned int Level::getNumberOfHorizontalTiles(){
	return m_HorizontalTiles;
}

unsigned int Level::getNumberOfVerticalTiles(){
	return m_VerticalTiles;
}

int Level::getTileCoordinateForPosition(int position){
	return (position / m_TileSize);
}

int Level::getTileIndexForPosition(int positionX, int positionY){
	int coordinatesX = getTileCoordinateForPosition(positionX);
	int coordinatesY = getTileCoordinateForPosition(positionY);
	return getTileIndexForCoordinates(coordinatesX, coordinatesY);
}

int Level::getTileIndexForCoordinates(int coordinatesX, int coordinatesY){
	//Validate the coordinates, then calculate the array index
	if(validateTileCoordinates(coordinatesX, coordinatesY) == true){
		return coordinatesX + (coordinatesY * getNumberOfHorizontalTiles());
	}

	return -1;
}

int Level::getTileIndexForTile(Tile* tile){
	return getTileIndexForPosition(tile->getPositionX(), tile->getPositionY());
}

Tile* Level::getTileForPosition(int positionX, int positionY){
	return getTileForTileIndex(getTileIndexForPosition(positionX, positionY));
}

Tile* Level::getTileForCoordinates(int coordinatesX, int coordinatesY){
	return getTileForTileIndex(getTileIndexForCoordinates(coordinatesX, coordinatesY));
}

Tile* Level::getTileForTileIndex(int index){
	if(index >= 0 && index < (getNumberOfHorizontalTiles() * getNumberOfVerticalTiles())){
		return (*m_Tiles)[index];
	}

	return NULL;
}

