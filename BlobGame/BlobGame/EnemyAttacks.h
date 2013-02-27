
#include "BlobGame.h"
#include "BlobGameConstants.h"
#include "constants.h"
#include "Pathfinder.h"
#include "MessageHandler.h"
#include "Enemy.h"
#include "blob.h"
#include "tile.h"
#include "level.h"
#include "Util.h"
#include <string>
#include "common.h"

std::vector<Tile*> pistolRange(Tile* currentTile,const int& dirFacing,Level* level);
std::vector<Tile*> flameRange(Tile* currentTile,const int& dirFacing,Level* level);

inline void pistolAttack(Tile* currentTile,const int& dirFacing,Level* level,Enemy* attacker){
	std::vector<Tile*> tiles = pistolRange(currentTile,dirFacing,level);
	unsigned int time = Util::instance()->getElapsedTime();
	for(int i = 0;i < tiles.size();i++){
		MessageHandler::Instance()->createMessage(
			PISTOL_HIT,attacker,BlobGame::instance(),tiles[i],10+(5*i),
			"PistolAttack_" + attacker->getName() + "_" + intToString(time));
	}
}

inline std::vector<Tile*> pistolRange(Tile* currentTile,const int& dirFacing,Level* level){
	Tile* (*AdjacentTile)(Tile* tile,Level* level);
	std::vector<Tile*> tiles;
	switch(dirFacing){
	case FRONT:
		AdjacentTile = &AdjacentTileTop;
		break;
	case RIGHT:
		AdjacentTile = &AdjacentTileRight;
		break;
	case BACK:
		AdjacentTile = &AdjacentTileBottom;
		break;
	case LEFT:
		AdjacentTile = &AdjacentTileLeft;
		break;
	default:
		break;
	}
	Tile* tile = AdjacentTile(currentTile,level);
	for(int i = 0;i < PISTOL_RANGE;i++){
		if(tile){
			tiles.push_back(tile);
			if(ContainsFlags(tile->getTileTypes(),Wall) ||
			   ContainsFlags(tile->getTileTypes(),BlobOn) ||
			   ContainsFlags(tile->getTileTypes(),EnemyOn)){
				break;
			}
			tile = AdjacentTile(tile,level);
		}
	}
	return tiles;
}

inline void FlameAttack(Tile* currentTile,const int& dirFacing,Level* level,Enemy* attacker){
	std::vector<Tile*> tiles = flameRange(currentTile,dirFacing,level);
	unsigned int time = Util::instance()->getElapsedTime();
	for(int i = 0;i < tiles.size();i++){
		//MessageHandler::Instance()->createMessage(
		//	PISTOL_HIT,attacker,BlobGame::instance(),tiles[i],10+(5*i),
		//	"FlameAttack_" + attacker->getName() + "_" + intToString(time));
	}

}

std::vector<Tile*> flameRange(Tile* currentTile,const int& dirFacing,Level* level){
	int tilesToLeft = 1;
	int tilesToRight = 1;
	bool straightPathBlocked = false;
	Tile* tile = currentTile; 
	std::vector<Tile*> tilesInRange;
	Tile* (*AdjacentTile)(Tile* tile,Level* level);
	Tile* (*AdjacentTileL)(Tile* tile,Level* level);
	Tile* (*AdjacentTileR)(Tile* tile,Level* level);
	switch(dirFacing){
		case 0:
			AdjacentTile  = &AdjacentTileTop;
			AdjacentTileL = &AdjacentTileLeft;
			AdjacentTileR = &AdjacentTileRight;
			break;
		case 1:
			AdjacentTile  = &AdjacentTileRight;
			AdjacentTileL = &AdjacentTileTop;
			AdjacentTileR = &AdjacentTileBottom;
			break;
		case 2:
			AdjacentTile  = &AdjacentTileBottom;
			AdjacentTileL = &AdjacentTileRight;
			AdjacentTileR = &AdjacentTileLeft;
			break;
		case 3:
			AdjacentTile  = &AdjacentTileLeft;
			AdjacentTileL = &AdjacentTileBottom;
			AdjacentTileR = &AdjacentTileTop;
			break;
	}
	for(int i = 0;i < FLAME_RANGE;i++){
		Tile* tile2 = AdjacentTile(tile,level);
		tile = AdjacentTile(tile,level);
		if(i == 0 && ContainsFlags(tile2->getTileTypes(),Wall)){
			break;
		}
		else if(!ContainsFlags(tile2->getTileTypes(),Wall)){
			tilesInRange.push_back(tile2);
		}
		for(int j = tilesToLeft;j > 0;j--){

		}
	}
	return tilesInRange;
}
