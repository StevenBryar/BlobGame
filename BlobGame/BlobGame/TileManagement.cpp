#include "TileManagement.h"
#include "tile.h"
#include "unit.h"
#include "blob.h"
#include "gameObject.h"
#include "common.h"
#include "MessageHandler.h"
#include "BlobGameConstants.h"
#include "BlobGame.h"

void tileUpdate(std::vector<GameObject*> objects,std::vector<Tile*> tiles){
	unsigned int tileTypes = 0;
	for(int i = 0;i < tiles.size();i++){
		if(ContainsFlags(tiles[i]->getTileTypes(),BlobOn)){
			tileTypes = tiles[i]->getTileTypes();
			RemoveFlag(&tileTypes,BlobOn);
			tiles[i]->setTileTypes(tileTypes);
		}
		if(ContainsFlags(tiles[i]->getTileTypes(),EnemyOn)){
			tileTypes = tiles[i]->getTileTypes();
			RemoveFlag(&tileTypes,EnemyOn);
			tiles[i]->setTileTypes(tileTypes);
		}
	}
	Unit* unit = NULL;
	for(int i = 0;i < objects.size();i++){
		if(objects[i]->getType() == "Blob"){
			unit = (Unit*)objects[i];
			if(!ContainsFlags(unit->getCurrentTile()->getTileTypes(),BlobOn)){
				unit->getCurrentTile()->setTileTypes(unit->getCurrentTile()->
				getTileTypes()|BlobOn);
			}
			if(ContainsFlags(unit->getCurrentTile()->
			   getTileTypes(),Fire) && 
			   !ContainsFlags(unit->getStatus(),OnFire)){
				   unit->addStatus(OnFire);
				   MessageHandler::Instance()->createMessage(REMOVE_UNIT_STATUS,
					   unit,BlobGame::instance(),unit,250,"Fire");
			}
			if(ContainsFlags(unit->getCurrentTile()->
				getTileTypes(),Slime) && !ContainsFlags(unit->getStatus(),createsSlime)){
				MessageHandler::Instance()->createMessage(REMOVE_TILE_EFFECT,unit,BlobGame::instance(),
														unit->getCurrentTile(),0,"Slime");
			}
		}
		else if(objects[i]->getType() == "Enemy"){
			unit = (Unit*)objects[i];
			if(!ContainsFlags(unit->getCurrentTile()->getTileTypes(),EnemyOn)){
				unit->getCurrentTile()->setTileTypes(unit->getCurrentTile()->
				getTileTypes()|EnemyOn);
			}
			if(ContainsFlags(unit->getCurrentTile()->
			   getTileTypes(),Fire) && 
			   !ContainsFlags(unit->getStatus(),OnFire)){
				   unit->addStatus(OnFire);
				   	MessageHandler::Instance()->createMessage(REMOVE_UNIT_STATUS,
					   unit,BlobGame::instance(),unit,250,"Fire");
			}
			if(ContainsFlags(unit->getCurrentTile()->
			   getTileTypes(),Slime) && 
			   !ContainsFlags(unit->getStatus(),Slowed) &&
			   !ContainsFlags(unit->getStatus(),createsSlime)){
				   unit->addStatus(Slowed);
				   	MessageHandler::Instance()->createMessage(REMOVE_UNIT_STATUS,
					   unit,BlobGame::instance(),unit,150,"Slowed");
					MessageHandler::Instance()->createMessage(REMOVE_TILE_EFFECT,
						unit,BlobGame::instance(),unit->getCurrentTile(),0,"Slime");
			}
			if(ContainsFlags(unit->getCurrentTile()->
			   getTileTypes(),Posion) && 
			   !ContainsFlags(unit->getStatus(),Posioned)){
				   unit->addStatus(Posioned);
				   MessageHandler::Instance()->createMessage(REMOVE_UNIT_STATUS,
					   unit,BlobGame::instance(),unit,300,"Posioned");
			}
		}
	}
}
int calcGScoreBlob(Tile* tile){
	int gScore = 0;

	if(ContainsFlags(tile->getTileTypes(),Fire)){
		gScore += 3;
	}

	return gScore;
}
int calcGScoreBasicEnemy(Tile* tile){
	int gScore = 0;

	if(ContainsFlags(tile->getTileTypes(),Fire)){
		gScore += 3;
	}
	if(ContainsFlags(tile->getTileTypes(),Posion)){
		gScore += 2;
	}
	if(ContainsFlags(tile->getTileTypes(),Slime)){
		gScore += 1;
	}

	return gScore;
}