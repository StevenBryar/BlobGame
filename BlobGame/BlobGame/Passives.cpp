#include "Passives.h"
#include "unit.h"
#include "blob.h"
#include "BlobGame.h"
#include "BlobGameConstants.h"
#include "tile.h"
#include "common.h"
#include "spriteManager.h"
#include "2dSprite.h"
#include "PathFinder.h"
#include "Util.h"

void createSlime(Unit* unit){
	Tile* tile = unit->getCurrentTile();
	if(!ContainsFlags(tile->getTileTypes(),Slime)){
		tile->setTileTypes(tile->getTileTypes()|Slime);
		Sprite2d* sprite = SpriteManager::instance()->createSprite(NULL,"PathGuy.png",0);
		sprite->setPosition(tile->getPositionX(),tile->getPositionY());
		sprite->setName("Slime_" + tile->getName());
		BlobGame::instance()->addEffect(sprite);
	}
}
void gatherResources(Unit* unit){
	static int timeToPowerUp = 150;
	if(unit->getType() == "Blob"){
		Blob* blob = (Blob*)unit;
		std::vector<Tile*> tiles = getSquareVision(BlobGame::instance()->getLevel(),
									1,blob->getCurrentTile());
		Blob* adjacentBlob;
		for(int i = 0;i < tiles.size();i++){
			if((adjacentBlob = BlobGame::instance()->blobOnTile(tiles[i])) &&
				adjacentBlob != blob){
				blob->setEnergy(blob->getEnergy()+adjacentBlob->getEnergy());
				adjacentBlob->setEnergy(0);
			}
		}
		BlobGame::instance()->setTotalEnergy(BlobGame::instance()->getTotalEnergy()+blob->getEnergy());
		blob->setEnergy(0);
		if(timeToPowerUp <= 0){
			BlobGame::instance()->setTotalPower(BlobGame::instance()->getTotalPower()+3);
			timeToPowerUp = 200;
		}
		else{
			timeToPowerUp -= Util::instance()->getDelta();
		}
	}
}
void consume(Unit* unit){
	if(unit->getTarget() && 
		unit->getType() == "Blob"){
		if(unit->getCurrentTile() != unit->getTarget()->getCurrentTile()){
			unit->getTarget()->setCurrentTile(unit->getCurrentTile());
		}
		unit->getTarget()->setPosition(unit->getPositionX(),unit->getPositionY());
		if(unit->getAttackTimer() > 0){
			unit->setAttackTimer(unit->getAttackTimer()-(unit->getAttackSpeed()*Util::instance()->getDelta()));
		}
		else{
			int damage = unit->getDamage()-unit->getTarget()->getArmour();
			unit->getTarget()->hit((damage > 0 ? damage : 1));
			unit->setAttackTimer(BASE_BLOB_CONSUME_TIME);
			if(unit->getTarget()->getHealth() <= 0){
				Blob* blob = (Blob*)unit;
				blob->setEnergy(blob->getEnergy()+ENEMY_ENERGY_VALUE);
				BlobGame::instance()->deleteObject(blob->getTarget());
				blob->setTarget(NULL);
				unsigned int state = blob->getStatus();
				RemoveFlag(&state,Consume);
				blob->setStatus(state);
				blob->removePassive(&consume);
				return;
			}
		}
	}
}