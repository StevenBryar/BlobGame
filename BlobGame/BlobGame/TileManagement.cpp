#include "TileManagement.h"
#include "tile.h"
#include "unit.h"
#include "blob.h"
#include "gameObject.h"
#include "common.h"
#include "MessageHandler.h"
#include "BlobGameConstants.h"
#include "BlobGame.h"

void tileUpdate(std::vector<GameObject*> objects){
	Unit* unit = NULL;
	unsigned int tileTypes = 0;
	for(int i = 0;i < objects.size();i++){
		if(objects[i]->getType() == "Blob"){
			unit = (Unit*)objects[i];
			if(ContainsFlags(unit->getCurrentTile()->
			   getTileTypes(),Fire) && 
			   !ContainsFlags(unit->getStatus(),OnFire)){
				   unit->addStatus(OnFire);
				   MessageHandler::Instance()->createMessage(REMOVE_UNIT_STATUS,
					   unit,BlobGame::instance(),unit,250,"Fire");
			}
			if(ContainsFlags(unit->getCurrentTile()->
			   getTileTypes(),Slime)){
				   tileTypes = unit->getCurrentTile()->getTileTypes();
				   RemoveFlag(&tileTypes,Slime);
				   unit->getCurrentTile()->setTileTypes(tileTypes);
			}
		}
		else if(objects[i]->getType() == "Enemy"){
			unit = (Unit*)objects[i];
			if(ContainsFlags(unit->getCurrentTile()->
			   getTileTypes(),Fire) && 
			   !ContainsFlags(unit->getStatus(),OnFire)){
				   unit->addStatus(OnFire);
				   	MessageHandler::Instance()->createMessage(REMOVE_UNIT_STATUS,
					   unit,BlobGame::instance(),unit,250,"Fire");
			}
			if(ContainsFlags(unit->getCurrentTile()->
			   getTileTypes(),Slime) && 
			   !ContainsFlags(unit->getStatus(),Slowed)){
				   unit->addStatus(Slowed);
				   	MessageHandler::Instance()->createMessage(REMOVE_UNIT_STATUS,
					   unit,BlobGame::instance(),unit,150,"Slowed");
				   tileTypes = unit->getCurrentTile()->getTileTypes();
				   RemoveFlag(&tileTypes,Slime);
				   unit->getCurrentTile()->setTileTypes(tileTypes);
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