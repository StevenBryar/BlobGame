#include "tile.h"
#include "spriteManager.h"
#include "constants.h"
#include "BlobGameConstants.h"
#include "2dSprite.h"
#include "common.h"

int Tile::m_TileCount = 0;
Tile::Tile(unsigned int tileTypes) :
 m_TileTypes(tileTypes){
	 if((tileTypes & Ground) == Ground){
		setSprite(SpriteManager::instance()->
		 createSprite(this,"GroundTile.png",10));
	 }
	 if((tileTypes & Wall) == Wall){
		 setSprite(SpriteManager::instance()->
		 createSprite(this,"WallTile.png",10));
	 }
	 m_TileCount++;
	 m_Name = "Tile" + intToString(m_TileCount);
	 if(getSprite()){
		 getSprite()->setColor(70,70,70,255);
	 }
 }

Tile::~Tile(){}

std::string Tile::getType(){
	return "Tile";
}

void Tile::update(double delta){}

void Tile::setTileTypes(unsigned int types){m_TileTypes = types;}
unsigned int Tile::getTileTypes(){return m_TileTypes;}
