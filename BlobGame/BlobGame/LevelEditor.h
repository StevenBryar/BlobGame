#ifndef LEVEL_EDITOR_H
#define LEVEL_EDITOR_H

#include "inputListener.h"
#include <string>

class Level;
class Tile;
class Camera;

class LevelEditor : public InputListener {
public:
	LevelEditor(Level* level,Camera* camera);
	virtual ~LevelEditor();

	bool loadLevelToEditor(std::string path);
	bool saveLevelFromEditor(std::string path);
	void setCamera(Camera* camera);
	Camera* getCamera();
	void update();

	void keyInputCallback(keyType key,inputEvent event);
	void mouseInputCalback(inputEvent event,int x,int y);
protected:
	void changeTile(unsigned int tileType,int posX,int posY);


	Level* m_Level;
	Camera* m_Camera;
	unsigned int m_CurrentTileType;
};

bool   loadPreview(std::string* name,unsigned int* width,unsigned int* height);
Level* loadLevel(std::string path);
void   saveLevel(std::string path,Level* level);
#endif