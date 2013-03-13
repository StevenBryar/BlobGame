#include "LevelEditor.h"
#include "level.h"
#include "BlobGameConstants.h"
#include "constants.h"
#include "common.h"
#include "tile.h"
#include "camera.h"
#include "inputManager.h"
#include "uiButton.h"
#include "uiListMenu.h"
#include "uiTextBox.h"
#include "TextManager.h"
#include "Vector3.h"
#include "BlobGame.h"
#include "Util.h"
#include "2dSprite.h"
#include "spriteManager.h"
#include "gameObject.h"
#include "Util.h"
#include <fstream>
#include <iostream>

LevelEditor::LevelEditor(Level* level,Camera* camera) :
m_Level(level),m_Camera(camera){
	InputManager::instance()->registerMouseInput(this,MOUSE_LB_PRESSED);
	InputManager::instance()->registerMouseInput(this,MOUSE_RB_PRESSED);
	InputManager::instance()->registerKeyinput(this,Q,KEY_RELEASED);
	Vector3 c(100,250,100);
	m_ObjectSprites = new std::vector<Sprite2d*>();
	m_BaseTiles = new UiListMenu(200,300,camera,NULL,NULL,"tfa_squaresans.ttf",20,3,30);
	m_BaseTiles->setWidth(250);
	m_BaseTiles->setHeight(200);
	m_BaseTiles->addEntry("Empty");
	m_BaseTiles->addEntry("Wall");
	m_BaseTiles->addEntry("Ground");
	m_Objects = new UiListMenu(400,300,camera,NULL,NULL,"tfa_squaresans.ttf",20,3,30);
	m_Objects->setWidth(270);
	m_Objects->setHeight(200);
	m_Objects->addEntry("None");
	m_Objects->addEntry("GruntSpawn");
	m_Objects->addEntry("GlobSpawn");
	m_Objects->addEntry("TeleBlobSpawn");
	m_Objects->addEntry("MotherBlobSpawn");
	m_Objects->addEntry("PistolEnemySpawn");
	m_NameBox = new UiTextBox("tfa_squaresans.ttf",20,"Two",c,m_Camera,100,100,128,20);
	m_NameBox->setFocus(true);
}
LevelEditor::~LevelEditor(){
	SafePtrRelease(m_Level);
	SafePtrRelease(m_BaseTiles);
	SafePtrRelease(m_Objects);
	SafePtrRelease(m_NameBox);
	if(m_ObjectSprites){
		for(int i = 0;i < m_ObjectSprites->size();i++){
			SpriteManager::instance()->deleteSprite((*m_ObjectSprites)[i]);
		}
		SafePtrRelease(m_ObjectSprites);
	}
}

bool LevelEditor::loadLevelToEditor(const std::string& path){
	m_Level = loadLevel(path,true,NULL,NULL);
	if(m_Level){
		std::vector<Tile*>* tiles = m_Level->getTiles();
		for(int i = 0;i < tiles->size();i++){
			if(ContainsFlags((*tiles)[i]->getTileTypes(),GruntStart)){
				m_ObjectSprites->push_back(
				SpriteManager::instance()->createSprite((*tiles)[i],"PathGuy.png",1));
			}
			else if(ContainsFlags((*tiles)[i]->getTileTypes(),TeleStart)){
				m_ObjectSprites->push_back(
				SpriteManager::instance()->createSprite((*tiles)[i],"PathGuy.png",1));
			}
			else if(ContainsFlags((*tiles)[i]->getTileTypes(),GlobStart)){
				m_ObjectSprites->push_back(
				SpriteManager::instance()->createSprite((*tiles)[i],"PathGuy.png",1));
			}
			else if(ContainsFlags((*tiles)[i]->getTileTypes(),MotherStart)){
				m_ObjectSprites->push_back(
				SpriteManager::instance()->createSprite((*tiles)[i],"PathGuy.png",1));
			}
			else if(ContainsFlags((*tiles)[i]->getTileTypes(),PistolEnemy)){
				m_ObjectSprites->push_back(
				SpriteManager::instance()->createSprite((*tiles)[i],"TestEnemy.png",1));
			}
		}
		return true;
	}
	return false;
}
bool LevelEditor::saveLevelFromEditor(const std::string& path){
	if(m_Level){
		saveLevel(path,m_Level);
		return true;
	}
	return false;
}
void LevelEditor::update(){
	m_BaseTiles->update();
	m_Objects->update();
	m_NameBox->update();
}

void LevelEditor::setCamera(Camera* camera){m_Camera = camera;}
Camera* LevelEditor::getCamera() const{return m_Camera;}

void LevelEditor::keyInputCallback(const keyType& key,const inputEvent& event){
	switch(event){
	case KEY_RELEASED:
		switch(key){
		case Q:
			saveLevelFromEditor("test.blvl");
			break;
		}
		break;
	}
}

void LevelEditor::mouseInputCalback(const inputEvent& event,const int& x,const int& y){
	if(m_Level && m_Camera){
		switch(event){
			case MOUSE_LB_PRESSED:{
				if(!Util::instance()->rectIntersection(x,y,1,1,m_BaseTiles->getScreenPosX(),
					m_BaseTiles->getScreenPosY(),m_BaseTiles->getWidth(),m_BaseTiles->getHeight()) &&
					!Util::instance()->rectIntersection(x,y,1,1,m_Objects->getScreenPosX(),
					m_Objects->getScreenPosY(),m_Objects->getWidth(),m_Objects->getHeight())){
						Tile* tile;
						Tile* tileToReplace;
						unsigned int currentTileType = 0;
						if(m_BaseTiles->getSelectedEntry() == "Wall"){
							currentTileType = Wall;
						}
						else if(m_BaseTiles->getSelectedEntry() == "Empty"){
							currentTileType = Empty;
						}
						else if(m_BaseTiles->getSelectedEntry() == "Ground"){
							currentTileType = Ground;
						}
						if(ContainsFlags(currentTileType,(Wall|Ground|Empty))){
							tileToReplace = m_Level->getTileForPosition(
							Util::instance()->screenToWorldCoordX(x,m_Camera),
							Util::instance()->screenToWorldCoordY(y,m_Camera));
							if(tileToReplace &&
							  !ContainsFlags(tileToReplace->getTileTypes(),currentTileType)){
								tile = new Tile(currentTileType);
								tile->setPosition(tileToReplace->getPositionX(),
													tileToReplace->getPositionY());
								m_Level->changeTile(tile);
							}
						}
					}
				break;
				}
			case MOUSE_RB_PRESSED:{
				if(!Util::instance()->rectIntersection(x,y,1,1,m_BaseTiles->getPositionX(),
					m_BaseTiles->getPositionY(),m_BaseTiles->getWidth(),m_BaseTiles->getHeight()) &&
					!Util::instance()->rectIntersection(x,y,1,1,m_Objects->getPositionX(),
					m_Objects->getPositionY(),m_Objects->getWidth(),m_Objects->getHeight())){
						unsigned int tileObject = 0;
						unsigned int unwalkables = 0;
						if(m_Objects->getSelectedEntry() == "None"){
							tileObject = 0;
							unwalkables = 0;
						}
						if(m_Objects->getSelectedEntry() == "GruntSpawn"){
							tileObject = GruntStart;
							unwalkables = BLOB_UNWALKABLES;
						}
						else if(m_Objects->getSelectedEntry() == "GlobSpawn"){
							tileObject = GlobStart;
							unwalkables = BLOB_UNWALKABLES;
						}
						else if(m_Objects->getSelectedEntry() == "TeleSpawn"){
							tileObject = TeleStart;
							unwalkables = BLOB_UNWALKABLES;
						}
						else if(m_Objects->getSelectedEntry() == "MotherSpawn"){
							tileObject = MotherStart;
							unwalkables = BLOB_UNWALKABLES;
						}
						else if(m_Objects->getSelectedEntry() == "PistolEnemySpawn"){
							tileObject = PistolEnemy;
							unwalkables = ENEMY_UNWALKABLES;
						}
						Tile* tile = m_Level->getTileForPosition(
						Util::instance()->screenToWorldCoordX(x,m_Camera),
						Util::instance()->screenToWorldCoordY(y,m_Camera));
						if(tile && !ContainsFlags(tile->getTileTypes(),tileObject) &&
							!ContainsFlags(tile->getTileTypes(),unwalkables)){
							unsigned int tileTypes = tile->getTileTypes();
							RemoveFlag(&tileTypes,(GruntStart|GlobStart|TeleStart
													|MotherStart|PistolEnemy));
							for(int i = 0;i < m_ObjectSprites->size();i++){
								if((*m_ObjectSprites)[i]->getOwner() == tile){
									SpriteManager::instance()->deleteSprite((*m_ObjectSprites)[i]);
									break;
								}
							}
							tile->setTileTypes(tileTypes|tileObject);
							switch(tileObject){
							case GruntStart:
								m_ObjectSprites->push_back(
									SpriteManager::instance()->createSprite(tile,"PathGuy.png",1));
								break;
							case GlobStart:
								m_ObjectSprites->push_back(
									SpriteManager::instance()->createSprite(tile,"PathGuy.png",1));
								break;
							case TeleStart:
								m_ObjectSprites->push_back(
									SpriteManager::instance()->createSprite(tile,"PathGuy.png",1));
								break;
							case MotherStart:
								m_ObjectSprites->push_back(
									SpriteManager::instance()->createSprite(tile,"PathGuy.png",1));
								break;
							case PistolEnemy:
								m_ObjectSprites->push_back(
									SpriteManager::instance()->createSprite(tile,"TestEnemy.png",1));
								break;
							default:
								break;
						}
					}
				}
				break;
			}//case MOUSE_RB_PRESSED
		}
	}
}

bool loadPreview(std::string* name,unsigned int* width,unsigned int* height){
	std::fstream file;
	unsigned int tag = 0;
	unsigned int nameLength = 0;
	file.read((char*)&tag,sizeof(unsigned int));
	if(BLOB_LEVEL_TAG != tag){
		return false;
	}
	file.read((char*)width,sizeof(unsigned int));

	file.read((char*)height,sizeof(unsigned int));

	file.read((char*)&nameLength,sizeof(unsigned int));
	char* buffer = new char[nameLength];

	file.read(buffer,nameLength);
	name = new std::string(buffer,nameLength);
	delete[] buffer;
	file.close();
	return true;
}
Level* loadLevel(const std::string& path,bool editMode,std::vector<GameObject*>* objects,
				GameObject*(factory)(const unsigned int& flags,Tile* tile)){

	int tileTypes[MAX_HORIZONTAL_TILES*MAX_VERTICAL_TILES];
	std::fstream file(path,std::ios::in | std::ios::binary);
	if(!file){
		return false;
	}
	unsigned int tag = 0;
	unsigned int levelWidth = 0;//in tiles
	unsigned int levelHeight = 0;//in tiles
	unsigned int nameLength = 0;
	file.read((char*)&tag,sizeof(unsigned int));
	if(BLOB_LEVEL_TAG != tag){
		return false;
	}
	file.read((char*)&levelWidth,sizeof(unsigned int));

	file.read((char*)&levelHeight,sizeof(unsigned int));

	file.read((char*)&nameLength,sizeof(unsigned int));
	char* buffer = new char[nameLength];

	file.read(buffer,nameLength);
	std::string name(buffer,nameLength);
	delete[] buffer;
	Level* level = NULL;
	if(editMode){
		Tile* newTile = NULL;
		Tile* oldTile = NULL;
		for(int i = 0;(i < levelWidth*levelHeight || editMode) && 
			           i < (MAX_HORIZONTAL_TILES*MAX_VERTICAL_TILES);i++){
			tileTypes[i] = Empty;
		}
		level = new Level(MAX_HORIZONTAL_TILES,MAX_VERTICAL_TILES,TILE_SIZE,tileTypes,objects,factory);
		unsigned int tileType = 0;
		for(int i = 0;i < levelHeight;i++){
			for(int j = 0;j < levelWidth;j++){
				file.read((char*)&tileType,sizeof(unsigned int));
				if(!ContainsFlags(tileType,Hole)){
					oldTile = level->getTileForCoordinates(j,i);
					newTile = new Tile(tileType);
					newTile->setPosition(oldTile->getPositionX(),oldTile->getPositionY());
					level->changeTile(newTile);
					oldTile = NULL;
				}
			}
		}
	}
	else{
		for(int i = 0;(i < levelWidth*levelHeight) && 
					 i < (MAX_HORIZONTAL_TILES*MAX_VERTICAL_TILES);i++){
			file.read((char*)&tileTypes[i],sizeof(unsigned int));
		}
		level = new Level(levelWidth,levelHeight,TILE_SIZE,tileTypes,objects,factory);
	}
	file.close();
	return level;
}
void saveLevel(const std::string& path,Level* level){
	std::ifstream file(path);
	if(file){
		file.close();
		if(remove(path.c_str()) != 0){
			std::cout << "Can't save level";
			return;
		}
	}
	std::ofstream outputFile(path,std::ios::out|std::ios::binary);
	unsigned int tag = BLOB_LEVEL_TAG;
	unsigned int width = 0;
	unsigned int height = 0;
	Tile* tile = NULL;
	for(int i = 0;i < level->getNumberOfVerticalTiles();i++){
		for(int j = 0;j < level->getNumberOfHorizontalTiles();j++){
			tile = level->getTileForCoordinates(j,i);
			if(!ContainsFlags(tile->getTileTypes(),Empty)){
				if(i >= height){height = i+1;}
				if(j >= width){width = j+1;}
			}
		}
	}
	unsigned int tileTypes = 0;

	outputFile.write((char*)&tag,sizeof(unsigned int));
	outputFile.write((char*)&width,sizeof(unsigned int));
	outputFile.write((char*)&height,sizeof(unsigned int));

	unsigned int namelength = level->getName().size();
	outputFile.write((char*)&namelength,sizeof(unsigned int));
	outputFile.write(level->getName().c_str(),namelength);

	for(int i = 0;i < height;i++){
		for(int j = 0;j < width;j++){
			tileTypes = level->getTileForCoordinates(j,i)->getTileTypes();
			if(ContainsFlags(tileTypes,Empty)){tileTypes = Hole;}
			outputFile.write((char*)&tileTypes,sizeof(unsigned int));
		}
	}
	outputFile.close();
}