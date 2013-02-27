#include "LevelEditor.h"
#include "level.h"
#include "BlobGameConstants.h"
#include "constants.h"
#include "common.h"
#include "tile.h"
#include "camera.h"
#include "inputManager.h"
#include "Util.h"
#include <fstream>
#include <iostream>

LevelEditor::LevelEditor(Level* level,Camera* camera) :
m_Level(level),m_Camera(camera){
	m_CurrentTileType = Ground;
	InputManager::instance()->registerMouseInput(this,MOUSE_LB_PRESSED);
	InputManager::instance()->registerKeyinput(this,NUM1,KEY_RELEASED);
	InputManager::instance()->registerKeyinput(this,NUM2,KEY_RELEASED);
	InputManager::instance()->registerKeyinput(this,NUM3,KEY_RELEASED);
}
LevelEditor::~LevelEditor(){
	SafePtrRelease(m_Level);
	InputManager::instance()->removeAllOf(this);
}

bool LevelEditor::loadLevelToEditor(const std::string& path){
	m_Level = loadLevel(path);
	if(m_Level){
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

}

void LevelEditor::setCamera(Camera* camera){m_Camera = camera;}
Camera* LevelEditor::getCamera() const{return m_Camera;}

void LevelEditor::keyInputCallback(const keyType& key,const inputEvent& event){
	switch(event){
	case KEY_RELEASED:
		switch(key){
		case NUM1:
			m_CurrentTileType = Wall;
			break;
		case NUM2:
			m_CurrentTileType = Ground;
			break;
		case NUM3:
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
			Tile* tile;
			Tile* tileToReplace;
			if(ContainsFlags(m_CurrentTileType,(Wall|Ground))){
				tileToReplace = m_Level->getTileForPosition(
				Util::instance()->screenToWorldCoordX(x,m_Camera),
				Util::instance()->screenToWorldCoordY(y,m_Camera));
				if(!ContainsFlags(tileToReplace->getTileTypes(),m_CurrentTileType)){
					tile = new Tile(m_CurrentTileType);
					tile->setPosition(tileToReplace->getPositionX(),
										tileToReplace->getPositionY());
					m_Level->changeTile(tile);
				}
			}
			break;
			}
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
Level* loadLevel(const std::string& path){
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
	for(int i = 0;i < (levelWidth*levelHeight) && 
					  (MAX_HORIZONTAL_TILES*MAX_VERTICAL_TILES);i++){
		file.read((char*)&tileTypes[i],sizeof(unsigned int));
	}
	file.close();
	return new Level(levelWidth,levelHeight,TILE_SIZE,tileTypes,NULL,NULL);
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
	unsigned int width = level->getNumberOfHorizontalTiles();
	unsigned int height = level->getNumberOfVerticalTiles();
	unsigned int tileTypes = 0;

	outputFile.write((char*)&tag,sizeof(unsigned int));
	outputFile.write((char*)&width,sizeof(unsigned int));
	outputFile.write((char*)&height,sizeof(unsigned int));

	unsigned int namelength = level->getName().size();
	outputFile.write((char*)&namelength,sizeof(unsigned int));
	outputFile.write(level->getName().c_str(),namelength);

	for(int i = 0;i < level->getNumberOfHorizontalTiles()*
					  level->getNumberOfVerticalTiles();i++){
		tileTypes = (*level->getTiles())[i]->getTileTypes();
		outputFile.write((char*)&tileTypes,sizeof(unsigned int));
	}
	outputFile.close();
}