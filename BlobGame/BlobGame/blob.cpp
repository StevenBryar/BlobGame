#include "blob.h"
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
#include "spriteManager.h"
#include "2dSprite.h"
#include "Vector3.h"
#include "Passives.h"

int Blob::m_BlobCount = 0;

Blob::Blob() :
m_EnergyResource(0){
	m_BlobCount++;
	m_Abilities = new std::map<keyType,Ability*>();
	m_Name = "Blob" + intToString(m_BlobCount);
}

Blob::Blob(Tile* currentTile) :
m_EnergyResource(0),
Unit(currentTile){
	m_BlobCount++;
	m_Abilities = new std::map<keyType,Ability*>();
	m_Name = "Blob" + intToString(m_BlobCount);
}

Blob::Blob(int health,int damage,int attackSpeed,
	int moveSpeed,int armour) :
m_EnergyResource(0),
Unit(health,damage,attackSpeed,moveSpeed,armour){
	m_BlobCount++;
	m_Abilities = new std::map<keyType,Ability*>();
	m_Name = "Blob" + intToString(m_BlobCount);
}

Blob::Blob(int health,int damage,int attackSpeed,
	int moveSpeed,int armour,Tile* currentTile) :
m_EnergyResource(0),
Unit(health,damage,attackSpeed,moveSpeed,armour,currentTile){
	m_BlobCount++;
	m_Abilities = new std::map<keyType,Ability*>();
	m_Name = "Blob" + intToString(m_BlobCount);
}

Blob::~Blob(){
	 SafeMapDelete<keyType,Ability>(*m_Abilities);
	 SafePtrRelease(m_Abilities);
}

Ability* Blob::getAbilityFromKey(keyType key){
	std::map<keyType,Ability*>::iterator iterator;
	for(iterator = m_Abilities->begin();
		iterator != m_Abilities->end();iterator++){
		if(iterator->first == key){
			return iterator->second;
		}
	}
	return NULL;
}
Ability* Blob::getAbilityFromName(std::string name){
	std::map<keyType,Ability*>::iterator iterator;
	for(iterator = m_Abilities->begin();
		iterator != m_Abilities->end();iterator++){
			if(iterator->second->name == name){
			return iterator->second;
		}
	}
	return NULL;
}

void Blob::useAbility(keyType key,int cursorWorldX,int cursorWorldY){
	Ability* a;
	if((a = getAbilityFromKey(key))){
		if(!a->onCoolDown &&
			a->useAbility(this,cursorWorldX,cursorWorldY)){
			a->onCoolDown = true;
			a->coolDown = 0;
		}
		else{
			//play error sound.
		}
	}
}

void Blob::addAbilty(Ability* a,keyType key){
	if(!getAbilityFromKey(key)){
		std::map<keyType,Ability*>::iterator i;
		for(i = m_Abilities->begin();i != m_Abilities->end();i++){
			if(i->second == a){
				return;
			}
		}
	}
	m_Abilities->insert(std::pair<keyType,Ability*>(key,a));
}

void Blob::removeAbility(Ability* a){
	std::map<keyType,Ability*>::iterator i;
	for(i = m_Abilities->begin();i != m_Abilities->end();i++){
		if(i->second == a){
			SafePtrRelease(a);
			m_Abilities->erase(i);
		}
	}
}
void Blob::removeAbility(keyType key){
	Ability* a = getAbilityFromKey(key);
	if(a){
		removeAbility(a);
	}
}

std::map<keyType,Ability*>* Blob::getAbilities(){
	return m_Abilities;
}
void Blob::updateAbilities(){
	std::map<keyType,Ability*>::iterator i;
	for(i = m_Abilities->begin();i != m_Abilities->end();i++){
		if(i->second->onCoolDown){
			i->second->coolDown += (Util::instance()->getDelta()*100);
			if(i->second->coolDown >= i->second->coolDownDuration){
				i->second->onCoolDown = false;
				i->second->coolDown = 0.0f;
			}
		}
	}
}

void Blob::update(){
	updatePassives();
	if(m_Health <= 0 && !ContainsFlags(m_UnitStatus,Dead)){
		die();
	}
	else if(!ContainsFlags(m_UnitStatus,Dead)){
		if(getTarget() && !ContainsFlags(m_UnitStatus,Consume)){
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
		if((enemy = BlobGame::instance()->enemyOnTile(getCurrentTile())) &&
			!ContainsFlags(m_UnitStatus,Consume) && 
			!ContainsFlags(enemy->getStatus(),Consumed)){
			m_Target = enemy;
			enemy->idle();
			enemy->fullStop();
			enemy->addStatus(Consumed);
			addPassive(&consume);
			addStatus(Consume);
			MessageHandler::Instance()->createMessage
				(7,this,BlobGame::instance(),enemy,0);
		}
		updateAbilities();
	}
}
void Blob::die(){
	MessageHandler::Instance()->createMessage(2,this,BlobGame::instance(),this,100);
	addStatus(Dead);
	fullStop();
	if(ContainsFlags(m_UnitStatus,Consume) && m_Target){
		unsigned int state = m_Target->getStatus();
		RemoveFlag(&state,Consumed);
		m_Target->setStatus(state);
		Enemy* enemy = (Enemy*)m_Target;
		enemy->search();
	}
	//play death animation
}
int Blob::getEnergy(){
	return m_EnergyResource;
}
void Blob::setEnergy(int e){
	m_EnergyResource = e;
}
void Blob::handleMessage(Message msg){
	switch(msg.type){
	case 0:
		setMoveSpeed(getMoveSpeed()-MORPH_SPEED_INCREASE);
		break;
	default:
		break;
	}
}

std::string Blob::getAllegiance(){return "Ally";}
std::string Blob::getType(){return "Blob";}