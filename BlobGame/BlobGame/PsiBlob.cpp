#include "PsiBlob.h"
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

int PsiBlob::m_PsiCount = 0;
PsiBlob::PsiBlob(){
	Ability* a = new Ability("Teleport",200,true,&teleport);
	m_Abilities->insert(std::pair<keyType,Ability*>(Q,a));
	a = new Ability("GroupTeleport",250,true,&groupTeleport);
	m_Abilities->insert(std::pair<keyType,Ability*>(E,a));
	a = new Ability("TeleToCaster",250,true,&teleToCaster);
	m_Abilities->insert(std::pair<keyType,Ability*>(U,a));

	SpriteManager::instance()->
		createSprite(this,"PathGuy.png",1);
	m_PsiCount++;
	m_Name = "GruntBlob" + intToString(m_PsiCount);
}
PsiBlob::PsiBlob(Tile* currentTile) :
	Blob(currentTile){
	Ability* a = new Ability("Teleport",200,true,&teleport);
	m_Abilities->insert(std::pair<keyType,Ability*>(Q,a));
	a = new Ability("GroupTeleport",250,true,&groupTeleport);
	m_Abilities->insert(std::pair<keyType,Ability*>(E,a));
	a = new Ability("TeleToCaster",250,true,&teleToCaster);
	m_Abilities->insert(std::pair<keyType,Ability*>(U,a));

	SpriteManager::instance()->
		createSprite(this,"PathGuy.png",1);
	m_PsiCount++;
	m_Name = "GruntBlob" + intToString(m_PsiCount);
}
PsiBlob::PsiBlob(int health,int damage,int attackSpeed,
		int moveSpeed,int armour) :
	Blob(health,damage,attackSpeed,moveSpeed,armour){
	Ability* a = new Ability("Teleport",200,true,&teleport);
	m_Abilities->insert(std::pair<keyType,Ability*>(Q,a));
	a = new Ability("GroupTeleport",250,true,&groupTeleport);
	m_Abilities->insert(std::pair<keyType,Ability*>(E,a));
	a = new Ability("TeleToCaster",250,true,&teleToCaster);
	m_Abilities->insert(std::pair<keyType,Ability*>(U,a));

	SpriteManager::instance()->
		createSprite(this,"PathGuy.png",1);	
	m_PsiCount++;
	m_Name = "GruntBlob" + intToString(m_PsiCount);
}
PsiBlob::PsiBlob(int health,int damage,int attackSpeed,
		int moveSpeed,int armour,Tile* currentTile) :
	Blob(health,damage,attackSpeed,moveSpeed,armour,currentTile){
	Ability* a = new Ability("Teleport",200,true,&teleport);
	m_Abilities->insert(std::pair<keyType,Ability*>(Q,a));
	a = new Ability("GroupTeleport",250,true,&groupTeleport);
	m_Abilities->insert(std::pair<keyType,Ability*>(E,a));
	a = new Ability("TeleToCaster",250,true,&teleToCaster);
	m_Abilities->insert(std::pair<keyType,Ability*>(U,a));

	SpriteManager::instance()->
		createSprite(this,"PathGuy.png",1);
	m_PsiCount++;
	m_Name = "GruntBlob" + intToString(m_PsiCount);
}

PsiBlob::~PsiBlob(){}

void PsiBlob::update(){
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
void PsiBlob::useAbility(keyType key,int cursorWorldX,int cursorWorldY){
	Ability* a;
	if((a = getAbilityFromKey(key))){
		if(a->name == "Teleport"){
			Tile* tile = m_Level->getTileForPosition(cursorWorldX,cursorWorldY);
			doAbility(a,this,tile,NULL);
		}
		else if(a->name == "GroupTeleport"){
			Tile* tile = m_Level->getTileForPosition(cursorWorldX,cursorWorldY);
			doAbility(a,this,tile,NULL);
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
std::string PsiBlob::getType(){
	return "PsiBlob";
}

void PsiBlob::die(){
	MessageHandler::Instance()->createMessage(2,this,BlobGame::instance(),this,100);
	addStatus(Dead);
	//play death animation
}

void PsiBlob::handleMessage(Message msg){
	switch(msg.type){
	case 0:
		setMoveSpeed(getMoveSpeed()-MORPH_SPEED_INCREASE);
		break;
	default:
		break;
	}
}