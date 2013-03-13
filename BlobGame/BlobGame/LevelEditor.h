#ifndef LEVEL_EDITOR_H
#define LEVEL_EDITOR_H

#include "inputListener.h"
#include <string>
#include <vector>

class Level;
class Tile;
class Camera;
class UiListMenu;
class UiTextBox;
class Sprite2d;
class GameObject;

class LevelEditor : public InputListener {
public:
	LevelEditor(Level* level,Camera* camera);
	virtual ~LevelEditor();

	bool loadLevelToEditor(const std::string& path);
	bool saveLevelFromEditor(const std::string& path);
	void setCamera(Camera* camera);
	Camera* getCamera() const;
	void update();
	void keyInputCallback(const keyType& key,const inputEvent& event);
	void mouseInputCalback(const inputEvent& event,const int& x,const int& y);
protected:
	void changeTile(const unsigned int& tileType,const int& posX,const int& posY);

	Level* m_Level;
	Camera* m_Camera;
	UiListMenu* m_BaseTiles;
	UiListMenu* m_Objects;
	UiTextBox* m_NameBox;
	std::vector<Sprite2d*>* m_ObjectSprites;
};

bool   loadPreview(std::string* name,unsigned int* width,unsigned int* height);
Level* loadLevel(const std::string& path,bool editMode,
				std::vector<GameObject*>* objects,
				GameObject*(factory)(const unsigned int& flags,Tile* tile));
void   saveLevel(const std::string& path,Level* level);
#endif