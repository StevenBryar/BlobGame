#include "GruntBlob.h"
#include "Abilities.h"
#include "unit.h"
#include "PathNode.h"
#include "PathFinder.h"
#include "BlobGame.h"
#include "level.h"
#include "common.h"
#include "constants.h"
#include "BlobGameConstants.h"
#include "tile.h"
#include "spriteManager.h"
#include "gameObject.h"
#include "MyMath.h"
#include "Util.h"
#include "MessageHandler.h"
#include "Enemy.h"

int GruntBlob::m_GruntCount = 0;
GruntBlob::GruntBlob(){
	Ability* a = new Ability("Teleport",200,true,&teleport);
	m_Abilities->insert(std::pair<keyType,Ability*>(Q,a));
	a = new Ability("MorphSpeed",250,false,&morphSpeed);
	m_Abilities->insert(std::pair<keyType,Ability*>(W,a));
	a = new Ability("GroupTeleport",250,true,&groupTeleport);
	m_Abilities->insert(std::pair<keyType,Ability*>(E,a));
	a = new Ability("GooTrap",250,true,&gooTrap);
	m_Abilities->insert(std::pair<keyType,Ability*>(R,a));
		a = new Ability("Split",250,false,&split);
	m_Abilities->insert(std::pair<keyType,Ability*>(T,a));
	a = new Ability("TeleToCaster",250,true,&teleToCaster);
	m_Abilities->insert(std::pair<keyType,Ability*>(U,a));
	a = new Ability("GasAttack",250,false,&gasAttack);
	m_Abilities->insert(std::pair<keyType,Ability*>(I,a));

	SpriteManager::instance()->
		createSprite(this,"PathGuy.png",1);
	m_GruntCount++;
	m_Name = "GruntBlob" + intToString(m_GruntCount);
}
GruntBlob::GruntBlob(Tile* currentTile) :
	Blob(currentTile){
	Ability* a = new Ability("Teleport",200,true,&teleport);
	m_Abilities->insert(std::pair<keyType,Ability*>(Q,a));
	a = new Ability("MorphSpeed",250,false,&morphSpeed);
	m_Abilities->insert(std::pair<keyType,Ability*>(W,a));
	a = new Ability("GroupTeleport",250,true,&groupTeleport);
	m_Abilities->insert(std::pair<keyType,Ability*>(E,a));
	a = new Ability("GooTrap",250,true,&gooTrap);
	m_Abilities->insert(std::pair<keyType,Ability*>(R,a));
	a = new Ability("Split",250,false,&split);
	m_Abilities->insert(std::pair<keyType,Ability*>(T,a));
	a = new Ability("TeleToCaster",250,true,&teleToCaster);
	m_Abilities->insert(std::pair<keyType,Ability*>(U,a));
	a = new Ability("GasAttack",250,false,&gasAttack);
	m_Abilities->insert(std::pair<keyType,Ability*>(I,a));

	SpriteManager::instance()->
		createSprite(this,"PathGuy.png",1);
	m_GruntCount++;
	m_Name = "GruntBlob" + intToString(m_GruntCount);
}
GruntBlob::GruntBlob(int health,int damage,int attackSpeed,
		int moveSpeed,int armour) :
	Blob(health,damage,attackSpeed,moveSpeed,armour){
	Ability* a = new Ability("Teleport",200,true,&teleport);
	m_Abilities->insert(std::pair<keyType,Ability*>(Q,a));
	a = new Ability("MorphSpeed",250,false,&morphSpeed);
	m_Abilities->insert(std::pair<keyType,Ability*>(W,a));
	a = new Ability("GroupTeleport",250,true,&groupTeleport);
	m_Abilities->insert(std::pair<keyType,Ability*>(E,a));
	a = new Ability("GooTrap",250,true,&gooTrap);
	m_Abilities->insert(std::pair<keyType,Ability*>(R,a));
	a = new Ability("Split",250,false,&split);
	m_Abilities->insert(std::pair<keyType,Ability*>(T,a));
	a = new Ability("TeleToCaster",250,true,&teleToCaster);
	m_Abilities->insert(std::pair<keyType,Ability*>(U,a));
	a = new Ability("GasAttack",250,false,&gasAttack);
	m_Abilities->insert(std::pair<keyType,Ability*>(I,a));

	SpriteManager::instance()->
		createSprite(this,"PathGuy.png",1);	
	m_GruntCount++;
	m_Name = "GruntBlob" + intToString(m_GruntCount);
}
GruntBlob::GruntBlob(int health,int damage,int attackSpeed,
		int moveSpeed,int armour,Tile* currentTile) :
	Blob(health,damage,attackSpeed,moveSpeed,armour,currentTile){
	Ability* a = new Ability("Teleport",200,true,&teleport);
	m_Abilities->insert(std::pair<keyType,Ability*>(Q,a));
	a = new Ability("MorphSpeed",250,false,&morphSpeed);
	m_Abilities->insert(std::pair<keyType,Ability*>(W,a));
	a = new Ability("GroupTeleport",250,true,&groupTeleport);
	m_Abilities->insert(std::pair<keyType,Ability*>(E,a));
	a = new Ability("GooTrap",250,true,&gooTrap);
	m_Abilities->insert(std::pair<keyType,Ability*>(R,a));
	a = new Ability("Split",250,false,&split);
	m_Abilities->insert(std::pair<keyType,Ability*>(T,a));
	a = new Ability("TeleToCaster",250,true,&teleToCaster);
	m_Abilities->insert(std::pair<keyType,Ability*>(U,a));
	a = new Ability("GasAttack",250,false,&gasAttack);
	m_Abilities->insert(std::pair<keyType,Ability*>(I,a));

	SpriteManager::instance()->
		createSprite(this,"PathGuy.png",1);
	m_GruntCount++;
	m_Name = "GruntBlob" + intToString(m_GruntCount);
}

GruntBlob::~GruntBlob(){}

void GruntBlob::update(){
	if(m_Health <= 0 && !ContainsFlags(m_UnitStatus,Dead)){
		die();
	}
	else if(!ContainsFlags(m_UnitStatus,Dead)){
		if(getTarget()){
			if(m_Target->getHealth() <= 0){
				m_Target = NULL;
				stop();
			}
			else if(getTarget()->getCurrentTile() != getDestinationTile()){
				setDestinationTile(getTarget()->getCurrentTile());
			}
		}
		moveLerp();
		Enemy* enemy;
		if((enemy = BlobGame::instance()->enemyOnTile(getCurrentTile()))){
			enemy->hit(enemy->getHealth());
		}
		updateAbilities();
	}
}
void GruntBlob::useAbility(keyType key,int cursorWorldX,int cursorWorldY){
	Ability* a;
	if((a = getAbilityFromKey(key))){
		if(a->name == "Teleport"){
			Tile* tile = m_Level->getTileForPosition(cursorWorldX,cursorWorldY);
			doAbility(a,this,tile,NULL);
		}
		else if(a->name == "MorphSpeed"){
			doAbility(a,this,NULL,NULL);
		}
		else if(a->name == "GroupTeleport"){
			Tile* tile = m_Level->getTileForPosition(cursorWorldX,cursorWorldY);
			doAbility(a,this,tile,NULL);
		}
		else if(a->name == "GooTrap"){
			Tile* tile = m_Level->getTileForPosition(cursorWorldX,cursorWorldY);
			doAbility(a,this,tile,NULL);
		}
		else if(a->name == "Split"){
			doAbility(a,this,NULL,NULL);
		}
		else if(a->name == "GasAttack"){
			doAbility(a,this,NULL,NULL);
		}
		else if(a->name == "TeleToCaster"){
			Blob* blob;
			Tile* tile = m_Level->getTileForPosition(cursorWorldX,cursorWorldY);
			if((blob = BlobGame::instance()->blobOnTile(tile))){
				doAbility(a,this,blob,NULL);
			}
		}
	}
}
std::string GruntBlob::getType(){
	return "GruntBlob";
}

void GruntBlob::die(){
	MessageHandler::Instance()->createMessage(2,this,BlobGame::instance(),this,100);
	addStatus(Dead);
	//play death animation
}

void GruntBlob::handleMessage(Message msg){
	switch(msg.type){
	case 0:
		setMoveSpeed(getMoveSpeed()-MORPH_SPEED_INCREASE);
		break;
	default:
		break;
	}
}