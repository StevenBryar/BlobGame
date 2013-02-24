#ifndef BLOBGAME_H	
#define BLOBGAME_H
#include "game.h"
#include "inputListener.h"
#include "MessageListener.h"
#include <vector>

enum BlobGameStates{
	None,
	MainMenu,
	PauseMenu,
	OptionsMenu,
	GamePlay,
	Editor
};

class GameObject;
class Level;
class Camera;
class Blob;
class Tile;
class Enemy;
class Unit;
class Sprite2d;
class LevelEditor;

class BlobGame : public Game , public InputListener , public MessageListener{
public:
	void update();
	void reset();
	void cleanupInstance();
	static BlobGame* instance();
	void mouseInputCalback(inputEvent event,int x,int y);
	Camera* getCamera();
	void setCamera(Camera* camera);
	BlobGameStates getState();
	
	Level* getLevel();
	std::vector<GameObject*> getObjects();

	void setTotalEnergy(int e);
	int getTotalEnergy();
	void setTotalPower(int e);
	int getTotalPower();

	GameObject* pointWithinObject(float x,float y);

	Blob* blobOnTile(Tile* tile);
	std::vector<Blob*> blobsWithinArea(std::vector<Tile*> tiles);
	bool blobWithinArea(std::vector<Tile*> tiles,Blob* blob);
	Enemy* enemyOnTile(Tile* tile);
	std::vector<Enemy*> enemysWithinArea(std::vector<Tile*> tiles);
	bool enemyWithinArea(std::vector<Tile*> tiles,Enemy* enemy);
	Unit* unitOnTile(Tile* tile);
	std::vector<Unit*> unitsWithinArea(std::vector<Tile*> tiles);
	bool unitWithinArea(std::vector<Tile*> tiles,Unit* unit);

	void addObject(GameObject* obj);
	void deleteObject(GameObject* obj);
	void addEffect(Sprite2d* effect);

	void handleMessage(Message msg);
	void changeState(BlobGameStates state);
protected:
	BlobGame();
	virtual ~BlobGame();
	static BlobGame* m_Instance;
	void updateVision();

	void loadContent();
	void initialize();
	void (BlobGame::*blobUpdate)();

	void pauseMenu();
	void mainMenu();
	void optionsMenu();
	void gamePlay();
	void editor();

	void beginPause();
	void endPause();

	void beginMainMenu();
	void endMainMenu();

	void beginOptions();
	void endOptions();

	void beginEditor();
	void endEditor();

	void beginGame();
	void endGame();

	std::vector<GameObject*>* m_GameObjects;
	std::vector<GameObject*>* m_GameObjectsToDelete;
	std::vector<Sprite2d*>* m_Effects;
	BlobGameStates m_CurrentState;
	Level* m_Level;
	Camera* m_Camera;
	int m_TotalEnergy;
	int m_TotalPower;
	LevelEditor* m_Editor;
};

void testCallBack();
#endif