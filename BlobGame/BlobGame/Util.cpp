#include "Util.h"
#include "common.h"
#include "camera.h"

Util* Util::m_Instance = NULL;
Util* Util::instance(){
	if(m_Instance == NULL){
		m_Instance = new Util();
	}
	return m_Instance;
}
void Util::cleanUpInstance(){
	SafePtrRelease(m_Instance);
}

Util::Util() :
m_Delta(0.0f){
	sf::Clock clock;
	m_TimeSinceUpdate = clock.getElapsedTime().asSeconds();
}
Util::~Util(){}

void Util::update(){
	m_Delta = m_Clock.getElapsedTime().asSeconds() - m_TimeSinceUpdate;
	m_TimeSinceUpdate = m_Clock.getElapsedTime().asSeconds();
}
double Util::getDelta(){
	return m_Delta;
}
int Util::getElapsedTime(){
	return m_Clock.getElapsedTime().asMilliseconds();
}

bool Util::isKeyDown(keyType key){
	if(sf::Keyboard::isKeyPressed((sf::Keyboard::Key)key)){
		return true;
	}
	return false;
}
bool Util::isKeyUp(keyType key){
	return !isKeyDown(key);
}

float Util::screenToWorldCoordX(int pointScreenPosX,Camera* camera){
	return camera->getWorldPosX() + pointScreenPosX;
}
float Util::screenToWorldCoordY(int pointScreenPosY,Camera* camera){
	return camera->getWorldPosY() + pointScreenPosY;
}
float Util::worldToScreenCoordX(int pointWorldPosX,Camera* camera){
	return pointWorldPosX - camera->getWorldPosX();
}
float Util::worldToScreenCoordY(int pointWorldPosY,Camera* camera){
	return pointWorldPosY - camera->getWorldPosY();
}

bool Util::rectIntersection(float x1,float y1,float w1,float h1,
							float x2,float y2,float w2,float h2){
	return   (x1 < (x2+w2) &&
			 (x1+w1) > x2  &&
			  y1 < (y2+h2) &&
			 (y1+h1) > y2);
}

int Util::getCursorX(){
	return sf::Mouse::getPosition().x;
}
int Util::getCursorY(){
	return sf::Mouse::getPosition().y;
}