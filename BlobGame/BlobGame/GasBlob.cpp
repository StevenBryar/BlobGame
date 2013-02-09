#include "GasBlob.h"
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

int GasBlob::m_GasBlobCount = 0;
GasBlob::GasBlob(){
	Ability* a = new Ability("GasAttack",250,false,&gasAttack);
	m_Abilities->insert(std::pair<keyType,Ability*>(W,a));
	a = new Ability("GooTrap",250,true,&gooTrap);
	m_Abilities->insert(std::pair<keyType,Ability*>(Q,a));

	SpriteManager::instance()->
		createSprite(this,"PathGuy.png",1);
	m_GasBlobCount++;
	m_Name = "GasBlob" + intToString(m_GasBlobCount);
}
GasBlob::GasBlob(Tile* currentTile) :
	Blob(currentTile){
	Ability* a = new Ability("GasAttack",250,false,&gasAttack);
	m_Abilities->insert(std::pair<keyType,Ability*>(W,a));
	a = new Ability("GooTrap",250,true,&gooTrap);
	m_Abilities->insert(std::pair<keyType,Ability*>(Q,a));

	SpriteManager::instance()->
		createSprite(this,"PathGuy.png",1);
	m_GasBlobCount++;
	m_Name = "GasBlob" + intToString(m_GasBlobCount);
}
GasBlob::GasBlob(int health,int damage,int attackSpeed,
		int moveSpeed,int armour) :
	Blob(health,damage,attackSpeed,moveSpeed,armour){
	Ability* a = new Ability("GasAttack",250,false,&gasAttack);
	m_Abilities->insert(std::pair<keyType,Ability*>(W,a));
	a = new Ability("GooTrap",250,true,&gooTrap);
	m_Abilities->insert(std::pair<keyType,Ability*>(Q,a));

	SpriteManager::instance()->
		createSprite(this,"PathGuy.png",1);
	m_GasBlobCount++;
	m_Name = "GasBlob" + intToString(m_GasBlobCount);
}
GasBlob::GasBlob(int health,int damage,int attackSpeed,
		int moveSpeed,int armour,Tile* currentTile) :
	Blob(health,damage,attackSpeed,moveSpeed,armour,currentTile){
	Ability* a = new Ability("GasAttack",250,false,&gasAttack);
	m_Abilities->insert(std::pair<keyType,Ability*>(W,a));
	a = new Ability("GooTrap",250,true,&gooTrap);
	m_Abilities->insert(std::pair<keyType,Ability*>(Q,a));

	SpriteManager::instance()->
		createSprite(this,"PathGuy.png",1);
	m_GasBlobCount++;
	m_Name = "GasBlob" + intToString(m_GasBlobCount);
}

GasBlob::~GasBlob(){}

void GasBlob::update(){
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
void GasBlob::useAbility(keyType key,int cursorWorldX,int cursorWorldY){
	Ability* a;
	if((a = getAbilityFromKey(key))){
		if(a->name == "GasAttack"){
			doAbility(a,this,NULL,NULL);
		}
	}
}
std::string GasBlob::getType(){
	return "GasBlob";
}

void GasBlob::die(){
	MessageHandler::Instance()->createMessage(2,this,BlobGame::instance(),this,100);
	addStatus(Dead);
	//play death animation
}

void GasBlob::handleMessage(Message msg){
	switch(msg.type){
	default:
		break;
	}
}