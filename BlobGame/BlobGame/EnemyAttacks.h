
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

std::vector<Tile*> pistolRange(Tile* currentTile,int dirFacing,Level* level);
std::vector<Tile*> flameRange(Tile* currentTile,int dirFacing,Level* level);

inline void pistolAttack(Tile* currentTile,int dirFacing,Level* level,Enemy* attacker){
	std::vector<Tile*> tiles = pistolRange(currentTile,dirFacing,level);
	unsigned int time = Util::instance()->getElapsedTime();
	for(int i = 0;i < tiles.size();i++){
		MessageHandler::Instance()->createMessage(
			PISTOL_HIT,attacker,BlobGame::instance(),tiles[i],10+(5*i),
			"PistolAttack_" + attacker->getName() + "_" + intToString(time));
	}
}

inline std::vector<Tile*> pistolRange(Tile* currentTile,int dirFacing,Level* level){
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

inline void FlameAttack(Tile* currentTile,int dirFacing,Level* level,Enemy* attacker){
	std::vector<Tile*> tiles = flameRange(currentTile,dirFacing,level);
	unsigned int time = Util::instance()->getElapsedTime();
	for(int i = 0;i < tiles.size();i++){
		//MessageHandler::Instance()->createMessage(
		//	PISTOL_HIT,attacker,BlobGame::instance(),tiles[i],10+(5*i),
		//	"FlameAttack_" + attacker->getName() + "_" + intToString(time));
	}

}

std::vector<Tile*> flameRange(Tile* currentTile,int dirFacing,Level* level){
	//
}
