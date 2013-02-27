#include "Enemy.h"
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
#include <time.h>
#include <stdlib.h>
#include "MessageHandler.h"
#include "Vector2.h"
#include "blob.h"

int Enemy::m_EnemyCount = 0;

Enemy::Enemy(std::vector<Enemy*>* mates,const std::string& spriteFileName,
	std::vector<Tile*> (*getAttackRange)(Tile* currentTile,const int& dirFacing,Level* level),
	void (*fire)(Tile* currentTile,const int& dirFacing,Level* level,Enemy* attacker),const int& aRange) :
Unit(),m_SquadMates(mates),
getAttackRange(getAttackRange),
fire(fire),
m_AttackRange(aRange){
	changeState(Hold);
	if(mates == NULL){
		setIsSquadLeader(true);
		setSquadMates(new std::vector<Enemy*>());
	}
	setSprite(SpriteManager::instance()->
		createSprite(this,spriteFileName,2));
	m_EnemyCount++;
	m_Name = "Enemy" + intToString(m_EnemyCount);
}

Enemy::Enemy(std::vector<Enemy*>* mates,Tile* currentTile,const std::string& spriteFileName,
	std::vector<Tile*> (*getAttackRange)(Tile* currentTile,const int& dirFacing,Level* level),
	void (*fire)(Tile* currentTile,const int& dirFacing,Level* level,Enemy* attacker),const int& aRange) :
Unit(currentTile),m_SquadMates(mates),
getAttackRange(getAttackRange),
fire(fire),
m_AttackRange(aRange){
	changeState(Hold);
	if(mates == NULL){
		setIsSquadLeader(true);
		setSquadMates(new std::vector<Enemy*>());
	}
	setSprite(SpriteManager::instance()->
		createSprite(this,spriteFileName,2));
	m_EnemyCount++;
	m_Name = "Enemy" + intToString(m_EnemyCount);
}

Enemy::Enemy(std::vector<Enemy*>* mates,const int& health,const int& damage,
	const int& attackSpeed,const int& moveSpeed,const int& armour,const std::string& spriteFileName,
	std::vector<Tile*> (*getAttackRange)(Tile* currentTile,const int& dirFacing,Level* level),
	void (*fire)(Tile* currentTile,const int& dirFacing,Level* level,Enemy* attacker),const int& aRange) :
Unit(health,damage,attackSpeed,moveSpeed,armour),
m_SquadMates(mates),
getAttackRange(getAttackRange),
fire(fire),
m_AttackRange(aRange){
	changeState(Hold);
	if(mates == NULL){
		setIsSquadLeader(true);
		setSquadMates(new std::vector<Enemy*>());
	}
	setSprite(SpriteManager::instance()->
		createSprite(this,spriteFileName,2));
	m_EnemyCount++;
	m_Name = "Enemy" + intToString(m_EnemyCount);
}

Enemy::Enemy(std::vector<Enemy*>* mates,const int& health,const int& damage,
	const int& attackSpeed,const int& moveSpeed,const int& armour,Tile* currentTile,const std::string& spriteFileName,
	std::vector<Tile*> (*getAttackRange)(Tile* currentTile,const int& dirFacing,Level* level),
	void (*fire)(Tile* currentTile,const int& dirFacing,Level* level,Enemy* attacker),const int& aRange) :
Unit(health,damage,attackSpeed,moveSpeed,armour,currentTile),
m_SquadMates(mates),
getAttackRange(getAttackRange),
fire(fire),
m_AttackRange(aRange){
	changeState(Hold);
	if(mates == NULL){
		setIsSquadLeader(true);
		setSquadMates(new std::vector<Enemy*>());
	}
	setSprite(SpriteManager::instance()->
		createSprite(this,spriteFileName,2));
	m_EnemyCount++;
	m_Name = "Enemy" + intToString(m_EnemyCount);
}

Enemy::~Enemy(){
	if(isSquadLeader() &&
		m_SquadMates != NULL &&
		m_SquadMates->size() > 0){
		m_SquadMates->front()->setIsSquadLeader(true);
	}
	SafePtrRelease(m_SquadMates);
}

void Enemy::search(){
	//if leader 
	//find a tile that is a certain distance away from the current tile.
	if(isSquadLeader()){
		int maxX = Clamp<int>(0,
							  m_Level->getTileCoordinateForPosition(this->getPositionX()) + Ceil((m_Level->getNumberOfHorizontalTiles()-1)*0.25),
							  m_Level->getNumberOfHorizontalTiles()-1);
		int maxY = Clamp<int>(0,
							  m_Level->getTileCoordinateForPosition(this->getPositionY()) + Ceil((m_Level->getNumberOfVerticalTiles()-1)*0.25),
							  m_Level->getNumberOfVerticalTiles()-1);
		int minX = Clamp<int>(0,
							  m_Level->getTileCoordinateForPosition(this->getPositionX()) - (maxX - m_Level->getTileCoordinateForPosition(this->getPositionX())),
							  m_Level->getNumberOfHorizontalTiles()-1);
		int minY = Clamp<int>(0,
							  m_Level->getTileCoordinateForPosition(this->getPositionY()) - (maxY - m_Level->getTileCoordinateForPosition(this->getPositionY())),
							  m_Level->getNumberOfVerticalTiles()-1);
		srand(time(0));
		for(int i = 0;i < 10;i++){
			int posX = (rand() %(maxX-minX))+minX;
			int posY = (rand() %(maxY-minY))+minY;
			if(m_Level->getTileForCoordinates(posX,posY) == m_CurrentTile ||
				ContainsFlags(m_Level->getTileForCoordinates(posX,posY)->getTileTypes(),m_Unwalkables)){
					continue;
			}
				setDestinationTile(m_Level->getTileForCoordinates(posX,posY));
				changeState(Search);
				break;
		}
		for(int i = 0;i < m_SquadMates->size();i++){
			(*m_SquadMates)[i]->search();
		}
	}
	else{
		if(getSquadLeader()->getDestinationTile() != NULL){
			setDestinationTile(getSquadLeader()->getDestinationTile());
		}
	}
}
void Enemy::hold(){
	stop();
	setTarget(NULL);
	changeState(Hold);
	if(isSquadLeader()){
		for(int i = 0;i < m_SquadMates->size();i++){
			(*m_SquadMates)[i]->hold();
		}
	}
}
void Enemy::idle(){
	stop();
	EnemyUpdate = NULL;
	m_Target = NULL;
}
std::vector<Enemy*>* Enemy::getSquadMates() const{
	return m_SquadMates;
}
void Enemy::setSquadMates(std::vector<Enemy*>* mates){
	SafePtrRelease(m_SquadMates);
	m_SquadMates = mates;
}
Enemy* Enemy::getSquadLeader(){
	if(isSquadLeader()){
		return this;
	}
	for(int i = 0;i < m_SquadMates->size();i++){
		if((*m_SquadMates)[i]->isSquadLeader()){
			return (*m_SquadMates)[i];
		}
	}
}
EnemyStates Enemy::getState() const{return m_State;}
bool Enemy::isSquadLeader()const {return m_SquadLeader;}
void Enemy::setIsSquadLeader(const bool& isLeader){m_SquadLeader = isLeader;}
std::string Enemy::getAllegiance(){return "Foe";}
std::string Enemy::getType(){return "Enemy";}

void Enemy::die(){
	MessageHandler::Instance()->createMessage(2,this,BlobGame::instance(),this,100);
	//play death animation
	EnemyUpdate = NULL;
}
void Enemy::update(){
	if(m_Health <= 0){
		die();
	}
	if(m_Target &&
		m_Target->getHealth() <= 0){
		m_Target == NULL;
		changeState(Search);
	}
	if(m_AttackTimer != 0){
		m_AttackTimer -= (m_AttackSpeed*Util::instance()->getDelta());
		if(m_AttackTimer <= 0){
			m_AttackTimer = 0;
		}
	}
	if(EnemyUpdate){
		(this->*EnemyUpdate)();
	}
}
void Enemy::changeState(EnemyStates state){
	switch(state){
	case Hold:
		EnemyUpdate = &Enemy::holdUpdate;
		m_State = state;
		return;
	case Attack:
		EnemyUpdate = &Enemy::attackUpdate;
		m_State = state;
		return;
	case Search:
		EnemyUpdate = &Enemy::searchUpdate;
		m_State = state;
		return;
	case rePosition:
		EnemyUpdate = &Enemy::rePosUpdate;
		m_State = state;
		return;
	default:
		return;
	}
}
void Enemy::searchUpdate(){
	//check if blob in vision.
	//if yes switch to attack.and set blob as target.
	//if there is currently a path set,follow it.
	std::vector<Blob*> blobs = BlobGame::instance()->blobsWithinArea(
		getSquareVision(m_Level,ENEMY_VISION,m_CurrentTile));
	for(int i = 0;i < blobs.size();i++){
		if(blobs[i]->getHealth() > 0){
			setTarget(blobs[i]);
			changeState(Attack);
			return;
		}
	}
	if(m_Path != NULL && m_Path->size() > 0){
		moveLerp();
	}
	else{//if no path set, use search.
		search();
	}
}
void Enemy::holdUpdate(){
	//do nothing unless enemy in sight.
	//if enemy in sight attack.
	std::vector<Blob*> blobs = BlobGame::instance()->blobsWithinArea(
		getSquareVision(m_Level,ENEMY_VISION,m_CurrentTile));
	for(int i = 0;i < blobs.size();i++){
		if(blobs[i]->getHealth() > 0){
			setTarget(blobs[i]);
			changeState(Attack);
		}
	}
}
void Enemy::attackUpdate(){
	//check if target within attack range.
	if(BlobGame::instance()->blobWithinArea(
		getAttackRange(m_CurrentTile,getDirection(),m_Level),(Blob*)m_Target) &&
		m_AttackTimer == 0){
		fire(m_CurrentTile,getDirection(),m_Level,this);
		m_AttackTimer = ENEMY_ATTACK_TIME;
		return;
	}
	else if(!BlobGame::instance()->blobWithinArea(
		getAttackRange(m_CurrentTile,getDirection(),m_Level),(Blob*)m_Target)){
		changeState(rePosition);
	}
}
void Enemy::rePosUpdate(){
	if((m_Path && m_Path->size() == 1) ||
	   (m_Path && m_Path->size() > 0 &&
	   !BlobGame::instance()->blobWithinArea(
	   getSquareVision(m_Level,ENEMY_VISION,m_CurrentTile),(Blob*)m_Target))){
		moveLerp();
		return;
	}
	if(((m_Path && m_Path->size() == 0) || !m_Path) && 
		!BlobGame::instance()->blobWithinArea(getSquareVision
		(m_Level,ENEMY_VISION,m_CurrentTile),(Blob*)m_Target)){
		m_Target = NULL;
		changeState(Search);
		return;
	}
	for(int i = 0;i < 4;i++){
		if(BlobGame::instance()->blobWithinArea(
			getAttackRange(m_CurrentTile,i,m_Level),(Blob*)m_Target)){
			stop();
			changeState(Attack);
			changeDirection(i);
			return;
		}
	}
	if(m_Path && m_Path->size() > 0){
		for(int i = 0;i < 4;i++){
			if(BlobGame::instance()->blobWithinArea(
				getAttackRange(m_Path->front()->getTile(),i,m_Level),(Blob*)m_Target)){
				moveLerp();
				return;
			}
		}
	}
	std::vector<Tile*> tiles = getSquareVision(m_Level,m_AttackRange,m_Target->getCurrentTile());
	std::map<int,Tile*> mapTiles;
	for(int i = 0;i < tiles.size();i++){
		if(ContainsFlags(tiles[i]->getTileTypes(),m_Unwalkables)){
			continue;
		}
		for(int j = 0;j < 4;j++){
			if(BlobGame::instance()->blobWithinArea(
				getAttackRange(tiles[i],j,m_Level),(Blob*)m_Target)){
					setDestinationTile(tiles[i]);
					if(m_Path){
						mapTiles.insert(std::pair<int,Tile*>(m_Path->size(),tiles[i]));
					}
					stop();
			}
		}
	}
	if(mapTiles.size() > 0){
		setDestinationTile(mapTiles.begin()->second);
	}
	else{
		changeState(Search);
	}
}

void Enemy::handleMessage(const Message& msg){
	switch(msg.type){
	default:
		break;
	}
}