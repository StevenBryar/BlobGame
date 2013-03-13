#include "BlobGame.h"
#include "spriteManager.h"
#include "gameObject.h"
#include "common.h"
#include "constants.h"
#include "camera.h"
#include "inputManager.h"
#include "BlobGameConstants.h"
#include "selectionManager.h"
#include "level.h"
#include "unit.h"
#include "tile.h"
#include "Util.h"
#include "MessageHandler.h"
#include "Enemy.h"
#include "PathFinder.h"
#include "2dSprite.h"
#include "MessageHandler.h"
#include "UnitFactory.h"
#include "Vector3.h"
#include "TileManagement.h"
#include "LevelEditor.h"
#include "uiButton.h"
#include "TextManager.h"
#include "Text.h"
#include "uiListMenu.h"
#include <direct.h>

BlobGame* BlobGame::m_Instance = NULL;
BlobGame::BlobGame() :
	m_CurrentState(None),
	m_TotalPower(0),
	m_TotalEnergy(0){
	loadContent();
	reset();
}
BlobGame::~BlobGame(){
	if(m_GameObjects != NULL){
		SafeVectorDelete((*m_GameObjects));
		delete m_GameObjects;
	}
	SafePtrRelease(m_Effects);
	SafePtrRelease(m_Level);
	SafePtrRelease(m_Camera);
	SafePtrRelease(m_Editor);
	SafePtrRelease(m_GameObjectsToDelete);
	SelectionManager::instance()->cleanupInstance();
	MessageHandler::Instance()->cleanUpInstance();
	TextManager::instance()->cleanupInstance();
}

void BlobGame::cleanupInstance(){
	if(m_Instance != NULL){
		delete m_Instance;
		m_Instance = NULL;
	}
}
BlobGame* BlobGame::instance(){
	if(m_Instance == NULL){
		m_Instance = new BlobGame();
	}
	return m_Instance;
}
void BlobGame::loadContent(){
	SpriteManager::instance()->
		loadTexture("PathGuy.png");
	SpriteManager::instance()->
		loadTexture("TestEnemy.png");
	SpriteManager::instance()->
		loadTexture("GroundTile.png");
	SpriteManager::instance()->
		loadTexture("WallTile.png");
	SpriteManager::instance()->
		loadTexture("EmptyTile.png");
	SpriteManager::instance()->
		loadTexture("ScrollSelect.png");
	SpriteManager::instance()->
		loadTexture("PosionCloud.png");
	SpriteManager::instance()->
		loadTexture("PlayDefault.png");
	SpriteManager::instance()->
		loadTexture("PlayS.png");
	SpriteManager::instance()->
		loadTexture("PlayH.png");
	SpriteManager::instance()->
		loadTexture("EditorD.png");
	SpriteManager::instance()->
		loadTexture("EditorH.png");
	SpriteManager::instance()->
		loadTexture("EditorS.png");
	TextManager::instance()->
		loadFont("tfa_squaresans.ttf");
}
void BlobGame::initialize(){
	m_Camera = new Camera();
	m_Level = NULL;
	m_GameObjects = new std::vector<GameObject*>();
	m_GameObjectsToDelete = new std::vector<GameObject*>();
	m_Effects = new std::vector<Sprite2d*>();
	m_Editor = NULL;
	InputManager::instance()->registerMouseInput(this,MOUSE_MOVED);
}
void BlobGame::update(){
	MessageHandler::Instance()->update();
	(this->*blobUpdate)();
}
Camera* BlobGame::getCamera() const{return m_Camera;}
void BlobGame::setCamera(Camera* camera){m_Camera = camera;}
BlobGameStates BlobGame::getState() const{return m_CurrentState;}
Level* BlobGame::getLevel() const{return m_Level;}
std::vector<GameObject*> BlobGame::getObjects() const{return *m_GameObjects;}

void BlobGame::reset(){
	InputManager::instance()->cleanUpInstance();
	initialize();
	changeState(MainMenu);
}

void BlobGame::setTotalEnergy(const int& e){m_TotalEnergy = e;}
int BlobGame::getTotalEnergy() const{return m_TotalEnergy;}
void BlobGame::setTotalPower(const int& e){m_TotalPower = e;}
int BlobGame::getTotalPower() const{return m_TotalPower;}

GameObject* BlobGame::pointWithinObject(float x,float y){
	x = Util::instance()->screenToWorldCoordX(x,m_Camera);
	y = Util::instance()->screenToWorldCoordY(y,m_Camera);

	for(int i = 0;i < m_GameObjects->size();i++){
		if(x >= (*m_GameObjects)[i]->getPositionX() &&
			x <= (*m_GameObjects)[i]->getPositionX() + 
			(*m_GameObjects)[i]->getWidth() &&
		y >= (*m_GameObjects)[i]->getPositionY() &&
			y <= (*m_GameObjects)[i]->getPositionY() + 
			(*m_GameObjects)[i]->getHeight()){
				return (*m_GameObjects)[i];
		}
	}
	return NULL;
}

void BlobGame::addObject(GameObject* obj){
	m_GameObjects->push_back(obj);
}
void BlobGame::deleteObject(GameObject* obj){
	for(int i = 0;i < m_GameObjects->size();i++){
		if((*m_GameObjects)[i] == obj){
			m_GameObjectsToDelete->push_back(obj);
			return;
		}
	}
}
void BlobGame::addEffect(Sprite2d* effect){
	m_Effects->push_back(effect);
}

Blob* BlobGame::blobOnTile(Tile* tile){
	Unit* unit;
	for(int i = 0;i < m_GameObjects->size();i++){
		Unit* unit = (Unit*)(*m_GameObjects)[i];
		if(unit->getCurrentTile() == tile &&
			unit->getAllegiance() == "Ally"){
			return (Blob*)unit;
		}
	}
	return NULL;
}
bool BlobGame::blobWithinArea(std::vector<Tile*> tiles,Blob* blob){
	for(int i = 0;i < tiles.size();i++){
		if(blobOnTile(tiles[i]) == blob){
			return true;
		}
	}
	return false;
}
std::vector<Blob*> BlobGame::blobsWithinArea(std::vector<Tile*> tiles){
	std::vector<Blob*> blobs;
	Blob* blob;
	for(int i = 0;i < tiles.size();i++){
		if((blob = blobOnTile(tiles[i]))){
			blobs.push_back(blob);
		}
	}
	return blobs;
}
Enemy* BlobGame::enemyOnTile(Tile* tile){
	Unit* unit;
	for(int i = 0;i < m_GameObjects->size();i++){
		Unit* unit = (Unit*)(*m_GameObjects)[i];
		if(unit->getCurrentTile() == tile &&
			unit->getAllegiance() == "Foe"){
			return (Enemy*)unit;
		}
	}
	return NULL;
}
bool BlobGame::enemyWithinArea(std::vector<Tile*> tiles,Enemy* enemy){
	for(int i = 0;i < tiles.size();i++){
		if(enemyOnTile(tiles[i]) == enemy){
			return true;
		}
	}
	return false;
}
std::vector<Enemy*> BlobGame::enemysWithinArea(std::vector<Tile*> tiles){
	Enemy* enemy;
	std::vector<Enemy*> enemies;
	for(int i = 0;i < tiles.size();i++){
		if((enemy = enemyOnTile(tiles[i]))){
			enemies.push_back(enemy);
		}
	}
	return enemies;
}
Unit* BlobGame::unitOnTile(Tile* tile){
	for(int i = 0;i < m_GameObjects->size();i++){
		Unit* unit = (Unit*)(*m_GameObjects)[i];
		if(unit->getCurrentTile() == tile){
			return unit;
		}
	}
	return NULL;
}
bool BlobGame::unitWithinArea(std::vector<Tile*> tiles,Unit* unit){
	for(int i = 0;i < tiles.size();i++){
		if(unitOnTile(tiles[i]) == unit){
			return true;
		}
	}
	return false;
}
std::vector<Unit*> BlobGame::unitsWithinArea(std::vector<Tile*> tiles){
	Unit* unit;
	std::vector<Unit*> units;
	for(int i = 0;i < tiles.size();i++){
		if((unit = unitOnTile(tiles[i]))){
			units.push_back(unit);
		}
	}
	return units;
}

void BlobGame::changeState(const BlobGameStates& state){
	if(state != m_CurrentState){
		switch(m_CurrentState){
		case MainMenu:
			endMainMenu();
			break;
		case OptionsMenu:
			endOptions();
			break;
		case PauseMenu:
			endPause();
			break;
		case Editor:
			endEditor();
			break;
		}
		switch(state){
		case MainMenu:
			beginMainMenu();
			blobUpdate = &BlobGame::mainMenu;
			break;
		case GamePlay:
			beginGame();
			blobUpdate = &BlobGame::gamePlay;
			break;
		case OptionsMenu:
			beginOptions();
			blobUpdate = &BlobGame::optionsMenu;
			break;
		case PauseMenu:
			beginPause();
			blobUpdate = &BlobGame::pauseMenu;
			break;
		case Editor:
			beginEditor();
			blobUpdate = &BlobGame::editor;
			break;
		}
		m_CurrentState = state;
	}
}

void BlobGame::pauseMenu(){

}
void BlobGame::mainMenu(){
	for(int i = 0;i < m_GameObjects->size();i++){
		(*m_GameObjects)[i]->update();
	}
}
void BlobGame::optionsMenu(){

}
void BlobGame::editor(){
	m_Editor->update();
}
void BlobGame::gamePlay(){
	SelectionManager::instance()->update();

	tileUpdate(*m_GameObjects,*m_Level->getTiles());
	for(int i = 0;i < m_GameObjectsToDelete->size();i++){
		for(int j = 0;j < m_GameObjects->size();j++){
			if((*m_GameObjectsToDelete)[i] == (*m_GameObjects)[j]){
				SafePtrRelease((*m_GameObjects)[j]);
				m_GameObjects->erase(m_GameObjects->begin()+j);
				break;
			}
		}
	}
	m_GameObjectsToDelete->clear();
	for(int i = 0;i < m_GameObjects->size();i++){
		(*m_GameObjects)[i]->update();
	}
	updateVision();
	std::cout << "TotalPower:"  << getTotalPower()  << std::endl;
	std::cout << "TotalEnergy:" << getTotalEnergy() << std::endl;
}
void BlobGame::updateVision(){
	std::vector<Tile*> tiles2;
	std::vector<Tile*> tiles3;
	std::vector<Enemy*> enemies;
	Sprite2d* sprite;
	Vector3 colorVec(150,150,150);
	Vector3 colorVec2(70,70,70);
	Unit* unit;
	int i;
	std::vector<Tile*> tiles = *m_Level->getTiles();
	for(i = 0;i < m_Level->getNumberOfHorizontalTiles()*
				  m_Level->getNumberOfVerticalTiles();i++){
		if(!ContainsFlags(tiles[i]->getTileTypes(),Hole) && 
						  tiles[i]->getSprite()->getColor() != colorVec2){
		   tiles[i]->getSprite()->setColor(colorVec2);
		}
	}
	for(i = 0;i < m_GameObjects->size();i++){
		unit = (Unit*)(*m_GameObjects)[i];
		if(unit->getAllegiance() == "Foe"){
			unit->getSprite()->setHiden(true);
			enemies.push_back((Enemy*)unit);
		}
		else if(unit->getType() == "Blob"){
			tiles3 = getSquareVision(m_Level,ALLY_VISION,unit->getCurrentTile());
			for(int j = 0;j < tiles3.size();j++){
				if(tiles2.size() == 0){
					tiles2.push_back(tiles3[j]);
					if(!ContainsFlags(tiles3[j]->getTileTypes(),Hole) &&
						tiles3[j]->getSprite()->getColor() != colorVec){
						tiles3[j]->getSprite()->setColor(150,150,150,255);
					}
					continue;
				}
				for(int k = 0;k < tiles2.size();k++){
					if(tiles2[k] == tiles3[j]){
						break;
					}
					else if(k == tiles2.size()-1){
						tiles2.push_back(tiles3[j]);
						if(!ContainsFlags(tiles3[j]->getTileTypes(),Hole) &&
							tiles3[j]->getSprite()->getColor() != colorVec){
							tiles3[j]->getSprite()->setColor(150,150,150,255);
						}
					}
				}
			}
		}
	}
	for(i= 0;i < tiles2.size();i++){
		for(int k = 0;k < enemies.size();k++){
			if(tiles2[i] == enemies[k]->getCurrentTile()){
				enemies[k]->getSprite()->setHiden(false);
			}
		}
	}
}

void BlobGame::beginPause(){}
void BlobGame::endPause(){}

void BlobGame::beginMainMenu(){
	UiButton* menuButton = new UiButton(300,50,128,128,"PlayDefault.png","PlayS.png","PlayH.png",
					FIRE_ON_RELEASED|HIGHLIGHT_ON_HOVER,m_Camera,&changeGameState,(void*)GamePlay);
	m_GameObjects->push_back(menuButton);
	menuButton = new UiButton(300,178,128,128,"EditorD.png","EditorS.png","EditorH.png",
					FIRE_ON_RELEASED|HIGHLIGHT_ON_HOVER,m_Camera,&changeGameState,(void*)Editor);
	m_GameObjects->push_back(menuButton);
}
void BlobGame::endMainMenu(){
	SafeVectorDelete(*m_GameObjects);
	m_GameObjects->clear();
}

void BlobGame::beginOptions(){}
void BlobGame::endOptions(){}

void BlobGame::beginEditor(){
	m_Camera->moveTo(0,0);
	m_Editor = new LevelEditor(NULL,m_Camera);
	m_Editor->loadLevelToEditor("test.blvl");
}
void BlobGame::endEditor(){
	SafePtrRelease(m_Editor);
}

void BlobGame::beginGame(){
	m_Camera->moveTo(0,0);
	m_Level = loadLevel("test.blvl",false,m_GameObjects,&createUnit);
	//saveLevel("test.blvl",m_Level);
	
	for(int i = 0;i < m_GameObjects->size();i++){
		if((*m_GameObjects)[i]->getType() != "GameObject"){
				Unit* unit = (Unit*)(*m_GameObjects)[i];
				unit->setLevel(m_Level);
		}
		if((*m_GameObjects)[i]->getType() == "Enemy"){
			Enemy* enemy = (Enemy*)(*m_GameObjects)[i];
			enemy->search();
		}
	}
}
void BlobGame::endGame(){
	if(m_GameObjects != NULL){
		SafeVectorDelete((*m_GameObjects));
		delete m_GameObjects;
		m_GameObjects = NULL;
	}
	SafePtrRelease(m_Level);
	SafePtrRelease(m_Camera);
	SafePtrRelease(m_GameObjectsToDelete);
	SelectionManager::instance()->cleanupInstance();
}
void BlobGame::mouseInputCalback(const inputEvent& event,const int& x,const int& y){
	double delta = Util::instance()->getDelta();
	if(getState() == GamePlay || getState() == Editor){
		if(event == MOUSE_MOVED){
			if(x < 2){
				m_Camera->move(-CAMERA_SPEED*delta,0);
			}
			else if(x > m_Camera->getWidth() - 4){
				m_Camera->move(CAMERA_SPEED*delta,0);
			}
			if(y < 2){
				m_Camera->move(0,-CAMERA_SPEED*delta);
			}
			else if(y > m_Camera->getHeight() - 4){
				m_Camera->move(0,CAMERA_SPEED*delta);
			}
		}
	}
}
void BlobGame::handleMessage(const Message& msg){
	switch(msg.type){
	case 2:{
		Unit* unit = (Unit*)msg.extraInfo;
		for(int i = 0;i < m_GameObjects->size();i++){
			if((*m_GameObjects)[i] == unit){
				m_GameObjects->erase(m_GameObjects->begin()+i);
				SafePtrRelease(unit);
				return;
			}
		}
		break;
	}
	case PISTOL_HIT:{
			Unit* unit;
			Tile* tile = (Tile*)msg.extraInfo;
			if((unit = unitOnTile(tile))){
				unit->hit(PISTOL_DAMAGE);
				MessageHandler::Instance()->deleteMessage(msg.name);
			}
		break;
	}
	case REMOVE_TILE_EFFECT:{
		Tile* tile = (Tile*)msg.extraInfo;
		unsigned int tileStatuses = tile->getTileTypes();
		if(msg.name == "Fire"){
			if(ContainsFlags(tile->getTileTypes(),Fire)){
				RemoveFlag(&tileStatuses,Fire);
				tile->setTileTypes(tileStatuses);
			}
		}
		else if(msg.name == "Posion"){
			if(ContainsFlags(tile->getTileTypes(),Posion)){
				RemoveFlag(&tileStatuses,Posion);
				tile->setTileTypes(tileStatuses);
			}
		}
		else if(msg.name == "Slime"){
			if(ContainsFlags(tile->getTileTypes(),Slime)){
				RemoveFlag(&tileStatuses,Slime);
				tile->setTileTypes(tileStatuses);
			}
		}
		for(int i = 0;i < m_Effects->size();i++){
			if((*m_Effects)[i]->getName() == 
				(msg.name + "_" + tile->getName())){
					Sprite2d* sprite = (*m_Effects)[i];
					m_Effects->erase(m_Effects->begin()+i);
					SpriteManager::instance()->deleteSprite(sprite);
			}
		}
	}
	case 5:{
		for(int i = 0;i < m_GameObjects->size();i++){
			if((*m_GameObjects)[i]->getName() == msg.name){
				Enemy* enemy = (Enemy*)(*m_GameObjects)[i];
				enemy->hold();
				return;
			}
		}
		break;
	}
	case 6:{
		for(int i = 0;i < m_GameObjects->size();i++){
			if((*m_GameObjects)[i]->getName() == msg.name){
				Unit* unit = (Unit*)(*m_GameObjects)[i];
				unit->hit(unit->getHealth());
				return;
			}
		}
		break;
	}
	case REMOVE_UNIT_STATUS:{
		Unit* unit = (Unit*)msg.extraInfo;
		unsigned int unitStatuses = unit->getStatus();
		if(msg.name == "Fire"){
			if(ContainsFlags(unit->getStatus(),OnFire)){
				RemoveFlag(&unitStatuses,OnFire);
				unit->setStatus(unitStatuses);
			}
		}
		else if(msg.name == "Posioned"){
			if(ContainsFlags(unit->getStatus(),Posioned)){
				RemoveFlag(&unitStatuses,Posioned);
				unit->setStatus(unitStatuses);
			}
		}
		else if(msg.name == "Slowed"){
			if(ContainsFlags(unit->getStatus(),Slowed)){
				RemoveFlag(&unitStatuses,Slowed);
				unit->setStatus(unitStatuses);
			}
		}
		break;
	}
	case CHANGE_STATE:{
		int s  = (int)msg.extraInfo;
		changeState((BlobGameStates)s);
		break;
	}
	default:
		break;
	}
}

void changeGameState(void* state){
	MessageHandler::Instance()->createMessage(CHANGE_STATE,
		BlobGame::instance(),BlobGame::instance(),state,0);
}
void testCallBack(void* thing){
	UiListMenu* menu = (UiListMenu*)thing;
	menu->scrollDown();
}
void testCallBack2(void* thing){
	UiListMenu* menu = (UiListMenu*)thing;
	menu->scrollUp();
}