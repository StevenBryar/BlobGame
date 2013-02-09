#ifndef UTIL_H
#define UTIL_H

//class will contain:keyboard,mouse,deltaTime,totalTime,
//maybe some other stuff i don't know right now.
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include "constants.h"
#include <string>

class Camera;

class Util{
public:
	static Util* instance();
	static void cleanUpInstance();
	void update();
	double getDelta();
	int getElapsedTime();
	bool isKeyDown(keyType key);
	bool isKeyUp(keyType key);
	float screenToWorldCoordX(int pointScreenPosX,Camera* camera);
	float screenToWorldCoordY(int pointScreenPosY,Camera* camera);
	float worldToScreenCoordX(int pointWorldPosX,Camera* camera);
	float worldToScreenCoordY(int pointWorldPosY,Camera* camera);
	bool rectIntersection(float x1,float y1,float w1,float h1,
							float x2,float y2,float w2,float h2);
	int getCursorX();
	int getCursorY();
private:
	Util();
	~Util();
	static Util* m_Instance;
protected:
	double m_Delta;
	double m_TimeSinceUpdate;
	sf::Clock m_Clock;
};
#endif