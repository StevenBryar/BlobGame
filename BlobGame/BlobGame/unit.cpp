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
#include "TileManagement.h"

Unit::Unit() :
m_Path(NULL),
m_DestinationTile(NULL),
m_Target(NULL),
calcBaseGScore(NULL),
m_UnitStatus(0),
m_AttackTimer(0),
m_Unwalkables(0){
	if(BlobGame::instance()->getLevel()){
		m_CurrentTile = BlobGame::instance()->getLevel()->
			getTileForPosition(getPositionX(),getPositionY());
	}
	m_PassivesToDelete = new std::vector<void (*)(Unit*)>();
	m_Passives = new std::vector<void (*)(Unit*)>();
}
Unit::Unit(Tile* currentTile):
GameObject(),
m_Path(NULL),
m_DestinationTile(NULL),
m_Target(NULL),
calcBaseGScore(NULL),
m_UnitStatus(0),
m_AttackTimer(0),
m_Unwalkables(0){
	setCurrentTile(currentTile);
	m_PassivesToDelete = new std::vector<void (*)(Unit*)>();
	m_Passives = new std::vector<void (*)(Unit*)>();
}
Unit::Unit(Tile* currentTile,Level* level) :
GameObject(),
m_Path(NULL),
m_DestinationTile(NULL),
m_Target(NULL),
m_Level(level),
calcBaseGScore(NULL),
m_UnitStatus(0),
m_AttackTimer(0),
m_Unwalkables(0){
	setCurrentTile(currentTile);
	m_PassivesToDelete = new std::vector<void (*)(Unit*)>();
	m_Passives = new std::vector<void (*)(Unit*)>();
}
Unit::Unit(int health,int damage,int attackSpeed,
	int moveSpeed,int armour) :
GameObject(),
m_Path(NULL),
m_DestinationTile(NULL),
m_Target(NULL),
m_Health(health),m_Damage(damage),
m_AttackSpeed(attackSpeed),m_MoveSpeed(moveSpeed),
m_Armour(armour),
calcBaseGScore(NULL),
m_UnitStatus(0),
m_AttackTimer(0),
m_Unwalkables(0){
	if(BlobGame::instance()->getLevel()){
		m_CurrentTile = BlobGame::instance()->getLevel()->
			getTileForPosition(getPositionX(),getPositionY());
	}
	m_PassivesToDelete = new std::vector<void (*)(Unit*)>();
	m_Passives = new std::vector<void (*)(Unit*)>();
}
Unit::Unit(int health,int damage,int attackSpeed,
	int moveSpeed,int armour,Tile* currentTile):
GameObject(),
m_Path(NULL),
m_DestinationTile(NULL),
m_Target(NULL),
m_Health(health),m_Damage(damage),
m_AttackSpeed(attackSpeed),m_MoveSpeed(moveSpeed),
m_Armour(armour),
calcBaseGScore(NULL),
m_UnitStatus(0),
m_AttackTimer(0),
m_Unwalkables(0){
	setCurrentTile(currentTile);
	m_PassivesToDelete = new std::vector<void (*)(Unit*)>();
	m_Passives = new std::vector<void (*)(Unit*)>();
}
Unit::Unit(int health,int damage,int attackSpeed,
			int moveSpeed,int armour,Tile* currentTile,Level* level) :
GameObject(),
m_Path(NULL),
m_DestinationTile(NULL),
m_Target(NULL),
m_Health(health),m_Damage(damage),
m_AttackSpeed(attackSpeed),m_MoveSpeed(moveSpeed),
m_Armour(armour),m_Level(level),
calcBaseGScore(NULL),
m_UnitStatus(0),
m_AttackTimer(0),
m_Unwalkables(0){
	setCurrentTile(currentTile);
	m_PassivesToDelete = new std::vector<void (*)(Unit*)>();
	m_Passives = new std::vector<void (*)(Unit*)>();
}
Unit::~Unit(){
	if(m_Path != NULL){
		SafeVectorDelete(*m_Path);
	}
	SafePtrRelease(m_Path);
	SafePtrRelease(m_Passives);
	SafePtrRelease(m_PassivesToDelete);
	m_Level = NULL;
	m_Target = NULL;
	m_DestinationTile = NULL;
	m_CurrentTile = NULL;
	calcBaseGScore = NULL;
}

void Unit::updatePassives(){
	for(int i = 0;i < m_PassivesToDelete->size();i++){
		for(int j = 0;j < m_Passives->size();j++){
			if((*m_Passives)[j] == (*m_PassivesToDelete)[i]){
				m_Passives->erase(m_Passives->begin()+j);
			}
		}
		break;
	}
	m_PassivesToDelete->clear();
	for(int i = 0;i < m_Passives->size();i++){
		(*m_Passives)[i](this);
	}
}
void Unit::addPassive(void (*passive)(Unit*)){
	for(int i = 0;i < m_Passives->size();i++){
		if((*m_Passives)[i] == passive){
			return;
		}
	}
	m_Passives->push_back(passive);
}
void Unit::removePassive(void (*passive)(Unit*)){
	m_PassivesToDelete->push_back(passive);
}
std::vector<void (*)(Unit*)>* Unit::getPassives(){
	return m_Passives;
}

std::string Unit::getType(){return "Unit";}

void Unit::attack(Unit* aUnit){aUnit->hit(getDamage());}
void Unit::hit(int damage){setHealth(getHealth() - damage);}
void Unit::die(){}

void Unit::setCurrentTile(Tile* tile){
	m_CurrentTile = tile;
	setPositionX(tile->getPositionX());
	setPositionY(tile->getPositionY());
}
void Unit::setDestinationTile(Tile* tile){
	if(m_CurrentTile != tile &&
		tile != NULL && !ContainsFlags(tile->getTileTypes(),m_Unwalkables)){
			m_DestinationTile = tile;
			PathNode* startNode = new PathNode(m_CurrentTile,0,
				CalculateScoreH(m_CurrentTile,m_DestinationTile,m_Level));
			if(m_Path != NULL){
				SafeVectorDelete<PathNode>(*m_Path);
				SafePtrRelease(m_Path);
			}
			m_Path = FindPath(startNode,m_DestinationTile,
				m_Level,m_Unwalkables,calcBaseGScore);
	}
}
Tile* Unit::getCurrentTile(){return m_CurrentTile;}
Tile* Unit::getDestinationTile(){return m_DestinationTile;}

void Unit::update(){
	moveLerp();
}

void Unit::moveLerp(){
	if(m_Path != NULL &&
	m_Path->size() > 0){
		if(ContainsFlags(m_Path->back()->getTile()->getTileTypes(),m_Unwalkables)){
			SafeVectorDelete<PathNode>(*m_Path);
			SafePtrRelease(m_Path);
			setDestinationTile(m_DestinationTile);
			if(!m_Path){
				setCurrentTile(m_CurrentTile);
				m_DestinationTile = NULL;
			}
			return;
		}
		if(getPositionX() > m_Path->back()->getTile()->getPositionX()){
			setPositionX(Clamp<float>(m_Path->back()->getTile()->getPositionX(),
			getPositionX(),
			Lerp<float>(getPositionX(),m_Path->back()->getTile()->getPositionX(),m_MoveSpeed*Util::instance()->getDelta())));
		}
		else{
			setPositionX(Clamp<float>(getPositionX(),
			m_Path->back()->getTile()->getPositionX(),
			Lerp<float>(getPositionX(),m_Path->back()->getTile()->getPositionX(),m_MoveSpeed*Util::instance()->getDelta())));
		}
		if(getPositionY() > m_Path->back()->getTile()->getPositionY()){
			setPositionY(Clamp<float>(m_Path->back()->getTile()->getPositionY(),
			getPositionY(),
			Lerp<float>(getPositionY(),m_Path->back()->getTile()->getPositionY(),m_MoveSpeed*Util::instance()->getDelta())));
		}
		else{
			setPositionY(Clamp<float>(getPositionY(),
			m_Path->back()->getTile()->getPositionY(),
			Lerp<float>(getPositionY(),m_Path->back()->getTile()->getPositionY(),m_MoveSpeed*Util::instance()->getDelta())));
		}

	if(   getPositionX() - m_Path->back()->getTile()->getPositionX()  > -0.5 && 
		  getPositionX() - m_Path->back()->getTile()->getPositionX()  <  0.5 && 
		  getPositionY() - m_Path->back()->getTile()->getPositionY()  > -0.5 && 
		  getPositionY() - m_Path->back()->getTile()->getPositionY()  <  0.5){
			setCurrentTile(m_Path->back()->getTile());
			PathNode* oldNode = m_Path->back();
			m_Path->pop_back();
			SafePtrRelease(oldNode);
			tileUpdate(BlobGame::instance()->getObjects(),*m_Level->getTiles());
			if(m_Path->size() > 0){
				if(m_Path->back()->getTile()->getPositionX() > 
					m_CurrentTile->getPositionX()){
					changeDirection(RIGHT);
				}
				else if(m_Path->back()->getTile()->getPositionX() < 
					m_CurrentTile->getPositionX()){
					changeDirection(LEFT);
				}
				else if(m_Path->back()->getTile()->getPositionY() > 
					m_CurrentTile->getPositionY()){
					changeDirection(FRONT);
				}
				else if(m_Path->back()->getTile()->getPositionY() <
					m_CurrentTile->getPositionY()){
					changeDirection(BACK);
				}
			}
		}
	}
}

void Unit::updateStatus(){
	static float fireTimer = 60;
	static float posionTimer = 80;

	if(ContainsFlags(m_UnitStatus,OnFire) &&
		fireTimer <= 0){
		m_Health -= 0;
		fireTimer = 60;
	}
	else if((ContainsFlags(m_UnitStatus,OnFire))){
		fireTimer -= Util::instance()->getDelta();
	}
	if(ContainsFlags(m_UnitStatus,Posioned) &&
		posionTimer <= 0){
		m_Health -= 0;
		posionTimer = 80;
	}
	else if((ContainsFlags(m_UnitStatus,Posioned))){
		posionTimer -= Util::instance()->getDelta();
	}
}

void Unit::stop(){
	if(m_Path != NULL && m_Path->size() > 0){
		Tile* tile = m_Path->back()->getTile();
		SafeVectorDelete<PathNode>(*m_Path);
		SafePtrRelease(m_Path);
		setDestinationTile(tile);
	}
}
void Unit::fullStop(){
	if(m_Path != NULL && m_Path->size() > 0){
		SafeVectorDelete<PathNode>(*m_Path);
		SafePtrRelease(m_Path);
	}
}
void Unit::setHealth(int hp){m_Health = hp;}
void Unit::setMoveSpeed(int moveSpeed){m_MoveSpeed = moveSpeed;}
void Unit::setDamage(int damage){m_Damage = damage;}
void Unit::setAttackSpeed(int attackSpeed){m_AttackSpeed = attackSpeed;}
void Unit::setArmour(int armour){m_Armour = armour;}
void Unit::setTarget(Unit* target){m_Target = target;}
void Unit::setLevel(Level* level){m_Level = level;}
void Unit::setAttackTimer(float attackTimer){m_AttackTimer = attackTimer;}
void Unit::setUnwalkables(unsigned int unWalkables){m_Unwalkables = unWalkables;}

int Unit::getHealth(){return m_Health;}
int Unit::getMoveSpeed(){return m_MoveSpeed;}
int Unit::getDamage(){return m_Damage;}
int Unit::getAttackSpeed(){return m_AttackSpeed;}
int Unit::getArmour(){return m_Armour;}
Unit* Unit::getTarget(){return m_Target;}
float Unit::getAttackTimer(){return m_AttackTimer;}
std::string Unit::getAllegiance(){return "Neutral";}
void Unit::changeDirection(int d){setRotation(90.0f*d);}
int Unit::getDirection(){return getRotation()/90;}
void Unit::addStatus(UnitStuff status){m_UnitStatus |= status;}
void Unit::setStatus(unsigned int status){m_UnitStatus = status;}
unsigned int Unit::getStatus(){return m_UnitStatus;}
unsigned int Unit::getUnwalkables(){return m_Unwalkables;}