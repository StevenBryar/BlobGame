
#include "blob.h"
#include "unit.h"
#include "tile.h"
#include "level.h"
#include "BlobGameConstants.h"
#include "MessageHandler.h"
#include "MessageListener.h"
#include "PathFinder.h"
#include "BlobGame.h"
#include "GruntBlob.h"
#include "common.h"
#include "Enemy.h"
#include "2dSprite.h"
#include "spriteManager.h"
#include "MyMath.h"
#include "Abilities.h"
#include "UnitFactory.h"

bool teleport(Blob* caster,const int& cursorWorldX,const int& cursorWorldY){
	Tile* tile = BlobGame::instance()->getLevel()->
			getTileForPosition(cursorWorldX,cursorWorldY);
	if(tile && !ContainsFlags(tile->getTileTypes(),BLOB_UNWALKABLES)){
		caster->setCurrentTile(tile);
		caster->fullStop();
		return true;
	}
	return false;
}
bool groupTeleport(Blob* caster,const int& cursorWorldX,const int& cursorWorldY){
	Tile* targetTile = BlobGame::instance()->getLevel()->
			getTileForPosition(cursorWorldX,cursorWorldY);
	if(targetTile && !ContainsFlags(targetTile->getTileTypes(),BLOB_UNWALKABLES)){
		Tile* startTile = caster->getCurrentTile();
		Tile* targetTiles[9] = {
			targetTile,
			AdjacentTileTopLeft(targetTile,BlobGame::instance()->getLevel()),
			AdjacentTileTop(targetTile,BlobGame::instance()->getLevel()),
			AdjacentTileTopRight(targetTile,BlobGame::instance()->getLevel()),
			AdjacentTileLeft(targetTile,BlobGame::instance()->getLevel()),
			AdjacentTileRight(targetTile,BlobGame::instance()->getLevel()),
			AdjacentTileBottomLeft(targetTile,BlobGame::instance()->getLevel()),
			AdjacentTileBottom(targetTile,BlobGame::instance()->getLevel()),
			AdjacentTileBottomRight(targetTile,BlobGame::instance()->getLevel())
		};
		Tile* startTiles[9] = {
			startTile,
			AdjacentTileTopLeft(startTile,BlobGame::instance()->getLevel()),
			AdjacentTileTop(startTile,BlobGame::instance()->getLevel()),
			AdjacentTileTopRight(startTile,BlobGame::instance()->getLevel()),
			AdjacentTileLeft(startTile,BlobGame::instance()->getLevel()),
			AdjacentTileRight(startTile,BlobGame::instance()->getLevel()),
			AdjacentTileBottomLeft(startTile,BlobGame::instance()->getLevel()),
			AdjacentTileBottom(startTile,BlobGame::instance()->getLevel()),
			AdjacentTileBottomRight(startTile,BlobGame::instance()->getLevel())
		};
		Blob* blob = NULL;
		for(int i = 0;i < 9;i++){
			if(targetTiles[i] 
			&& !ContainsFlags(targetTiles[i]->getTileTypes(),BLOB_UNWALKABLES)
				&& (blob = BlobGame::instance()->blobOnTile(startTiles[i]))){
					teleport(blob,targetTiles[i]->getPositionX(),targetTiles[i]->getPositionY());
			}
		}
		return true;
	}
	return false;
}
bool gooTrap(Blob* caster,const int& cursorWorldX,const int& cursorWorldY){
	Tile* targetTile = BlobGame::instance()->getLevel()->
						getTileForPosition(cursorWorldX,cursorWorldY);
	Enemy* enemy;
	if((enemy = BlobGame::instance()->enemyOnTile(targetTile))){
		enemy->idle();
		MessageHandler::Instance()->createMessage(5,caster,
			BlobGame::instance(),NULL,GOO_TRAP_TIME,enemy->getName());
		//add goo effect texture to game effects.
		return true;
	}
	return false;
}
bool morphSpeed(Blob* caster,const int& cursorWorldX,const int& cursorWorldY){
	//send change speed message to dispatch(will be sent caster when effect wears off.
	caster->setMoveSpeed(caster->getMoveSpeed()+MORPH_SPEED_INCREASE);
	MessageHandler::Instance()->createMessage(0,caster,caster,NULL,150);
	return true;
}
bool split(Blob* caster,const int& cursorWorldX,const int& cursorWorldY){
	Tile* startTile = caster->getCurrentTile();
	Tile* targetTiles[8] = {
		AdjacentTileLeft(startTile,BlobGame::instance()->getLevel()),
		AdjacentTileRight(startTile,BlobGame::instance()->getLevel()),
		AdjacentTileTop(startTile,BlobGame::instance()->getLevel()),
		AdjacentTileBottom(startTile,BlobGame::instance()->getLevel()),
		AdjacentTileTopLeft(startTile,BlobGame::instance()->getLevel()),
		AdjacentTileTopRight(startTile,BlobGame::instance()->getLevel()),
		AdjacentTileBottomLeft(startTile,BlobGame::instance()->getLevel()),
		AdjacentTileBottomRight(startTile,BlobGame::instance()->getLevel())
	};
	Blob* blob;
	bool didAbility = false;
	for(int i = 0,j = 0;i < 8 && j < 2;i++){
		if(targetTiles[i] && 
			!ContainsFlags(targetTiles[i]->getTileTypes(),BLOB_UNWALKABLES) &&	
				!BlobGame::instance()->blobOnTile(targetTiles[i])){
			blob = (Blob*)createGrunt(targetTiles[i]);
			MessageHandler::Instance()->createMessage(6,blob,BlobGame::instance(),NULL,1000,blob->getName());
			blob->setLevel(BlobGame::instance()->getLevel());
			BlobGame::instance()->addObject((GameObject*)blob);
			j++;
			didAbility = true;
		}
	}
	return didAbility;
}
bool teleToCaster(Blob* caster,const int& cursorWorldX,const int& cursorWorldY){
	Blob* blob;
	Tile* tile = BlobGame::instance()->getLevel()->
			getTileForPosition(cursorWorldX,cursorWorldY);
	if((blob = BlobGame::instance()->blobOnTile(tile))){
		Tile* startTile = caster->getCurrentTile();
		Tile* targetTiles[8] = {
			AdjacentTileLeft(startTile,BlobGame::instance()->getLevel()),
			AdjacentTileRight(startTile,BlobGame::instance()->getLevel()),
			AdjacentTileTop(startTile,BlobGame::instance()->getLevel()),
			AdjacentTileBottom(startTile,BlobGame::instance()->getLevel()),
			AdjacentTileTopLeft(startTile,BlobGame::instance()->getLevel()),
			AdjacentTileTopRight(startTile,BlobGame::instance()->getLevel()),
			AdjacentTileBottomLeft(startTile,BlobGame::instance()->getLevel()),
			AdjacentTileBottomRight(startTile,BlobGame::instance()->getLevel())
		};
		for(int i = 0;i < 8;i++){
			if(targetTiles[i] && 
				!ContainsFlags(targetTiles[i]->getTileTypes(),BLOB_UNWALKABLES)){
					teleport(blob,targetTiles[i]->getPositionX(),targetTiles[i]->getPositionY());
					return true;
			}
		}
	}
	return false;
}
bool gasAttack(Blob* caster,const int& cursorWorldX,const int& cursorWorldY){
	Tile* startTile = caster->getCurrentTile();
	Sprite2d* sprite;
	Tile* targetTiles[9] = {
		startTile,
		AdjacentTileLeft(startTile,BlobGame::instance()->getLevel()),
		AdjacentTileRight(startTile,BlobGame::instance()->getLevel()),
		AdjacentTileTop(startTile,BlobGame::instance()->getLevel()),
		AdjacentTileBottom(startTile,BlobGame::instance()->getLevel()),
		AdjacentTileTopLeft(startTile,BlobGame::instance()->getLevel()),
		AdjacentTileTopRight(startTile,BlobGame::instance()->getLevel()),
		AdjacentTileBottomLeft(startTile,BlobGame::instance()->getLevel()),
		AdjacentTileBottomRight(startTile,BlobGame::instance()->getLevel())
	};
	bool didAbility = false;
	for(int i = 0;i < 9;i++){
		if(targetTiles[i]){
			sprite = SpriteManager::instance()->createSprite(NULL,"PosionCloud.png",0);
			sprite->setColor(sprite->getColor().X,sprite->getColor().Y,sprite->getColor().Z,100);
			targetTiles[i]->setTileTypes((targetTiles[i]->getTileTypes() | Posion));
			sprite->setPosition(targetTiles[i]->getPositionX(),targetTiles[i]->getPositionY());
			sprite->setName("Posion_" + targetTiles[i]->getName());
			BlobGame::instance()->addEffect(sprite);
			MessageHandler::Instance()->createMessage(REMOVE_TILE_EFFECT,caster,BlobGame::instance(),
														targetTiles[i],500,"Posion");
			didAbility = true;
		}
	}
	return didAbility;
}
bool spawnGrunt(Blob* caster,const int& cursorWorldX,const int& cursorWorldY){
	std::vector<Tile*> tiles = getSquareVision(BlobGame::instance()->getLevel(),1,caster->getCurrentTile());
	for(int i = 0;i < tiles.size();i++){
		if(!ContainsFlags(tiles[i]->getTileTypes(),Wall) &&
			!BlobGame::instance()->blobOnTile(tiles[i]) &&
			BlobGame::instance()->getTotalEnergy() >= GRUNT_ENERGY_COST &&
			BlobGame::instance()->getTotalPower() >= GRUNT_POWER_COST){
				Blob* blob = (Blob*)createGrunt(tiles[i]);
				BlobGame::instance()->addObject((GameObject*)blob);
				blob->setLevel(BlobGame::instance()->getLevel());
				BlobGame::instance()->setTotalEnergy(
				BlobGame::instance()->getTotalEnergy()-GRUNT_ENERGY_COST);
				BlobGame::instance()->setTotalPower(
				BlobGame::instance()->getTotalPower()-GRUNT_POWER_COST);
				return true;
		}
	}
	return false;
}
bool spawnTele(Blob* caster,const int& cursorWorldX,const int& cursorWorldY){
	std::vector<Tile*> tiles = getSquareVision(BlobGame::instance()->getLevel(),1,caster->getCurrentTile());
	for(int i = 0;i < tiles.size();i++){
		if(!ContainsFlags(tiles[i]->getTileTypes(),Wall) &&
			!BlobGame::instance()->blobOnTile(tiles[i]) &&
			BlobGame::instance()->getTotalEnergy() >= TELE_ENERGY_COST &&
			BlobGame::instance()->getTotalPower() >= TELE_POWER_COST){
				Blob* blob = (Blob*)createTele(tiles[i]);
				BlobGame::instance()->addObject((GameObject*)blob);
				blob->setLevel(BlobGame::instance()->getLevel());
				return true;
		}
	}
	return false;
}
bool spawnGlob(Blob* caster,const int& cursorWorldX,const int& cursorWorldY){
	std::vector<Tile*> tiles = getSquareVision(BlobGame::instance()->getLevel(),1,caster->getCurrentTile());
	for(int i = 0;i < tiles.size();i++){
		if(!ContainsFlags(tiles[i]->getTileTypes(),Wall) &&
			!BlobGame::instance()->blobOnTile(tiles[i]) &&
			BlobGame::instance()->getTotalEnergy() >= GLOB_ENERGY_COST &&
			BlobGame::instance()->getTotalPower() >= GLOB_POWER_COST){
				Blob* blob = (Blob*)createGlob(tiles[i]);
				BlobGame::instance()->addObject((GameObject*)blob);
				blob->setLevel(BlobGame::instance()->getLevel());
				return true;
		}
	}
	return false;
}