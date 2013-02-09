#include "2dSprite.h"
#include "GameObject.h"
#include "MyMath.h"
#include <stdlib.h>
#include "Vector3.h"

int Sprite2d::m_SpriteCount = 0;

Sprite2d::Sprite2d(GameObject* entity,
					sf::Texture* texture) :
	m_Owner(entity){
		m_Sprite = new sf::Sprite(*texture);
		m_SpriteNumber = ++m_SpriteCount;
		m_Hidden = false;
		update();
}
Sprite2d::~Sprite2d(){
	m_Owner = NULL;
	if(m_Sprite != NULL){
		delete m_Sprite;
		m_Sprite = NULL;
	}
}
void Sprite2d::update(){
	if(m_Owner != NULL){
		setOrigin(m_Owner->getOriginX(),m_Owner->getOriginY());
		setPosition(m_Owner->getPositionX() + m_Owner->getOriginX(),
					m_Owner->getPositionY() + m_Owner->getOriginY());
		setRotation(m_Owner->getRotation());
		setScale(m_Owner->getWidth()/m_Sprite->getGlobalBounds().width,
			m_Owner->getHeight()/m_Sprite->getGlobalBounds().height);
	}
}
void Sprite2d::move(float x,float y){
	setPosition(m_Sprite->getPosition().x + x,
				m_Sprite->getPosition().y + y);
}

void Sprite2d::scale(float x,float y){m_Sprite->scale(x,y);}
void Sprite2d::setRotation(float r){m_Sprite->setRotation(r);}
void Sprite2d::setOrigin(float x,float y){m_Sprite->setOrigin(x,y);}
void Sprite2d::setScale(float x,float y){m_Sprite->setScale(x,y);}
void Sprite2d::setPosition(float x,float y){m_Sprite->setPosition(x,y);}
void Sprite2d::setColor(Vector3 rgb,int a){m_Sprite->setColor(sf::Color(rgb.X,rgb.Y,rgb.Z,a));}
void Sprite2d::setColor(int r,int g,int b,int a){m_Sprite->setColor(sf::Color(r,g,b,a));}
void Sprite2d::setHiden(bool hidden){m_Hidden = hidden;}
void Sprite2d::setName(std::string name){m_Name = name;}
void Sprite2d::setWidth(float w){
	setScale(w/m_Sprite->getLocalBounds().width , getScaleY());
}
void Sprite2d::setHeight(float h){
	setScale(getScaleX() , h/m_Sprite->getLocalBounds().height);
}
bool Sprite2d::getHidden(){return m_Hidden;}
float Sprite2d::getRotation(){return m_Sprite->getRotation();}
float Sprite2d::getOriginX(){return m_Sprite->getOrigin().x;}
float Sprite2d::getScaleX(){return m_Sprite->getScale().x;}
float Sprite2d::getPositionX(){return m_Sprite->getPosition().x;}
float Sprite2d::getOriginY(){return m_Sprite->getOrigin().y;}
float Sprite2d::getScaleY(){return m_Sprite->getScale().y;}
float Sprite2d::getPositionY(){return m_Sprite->getPosition().y;}
float Sprite2d::getWidth(){return (m_Sprite->getLocalBounds().width*getScaleX());}
float Sprite2d::getHeight(){return (m_Sprite->getLocalBounds().height*getScaleY());}
Vector3 Sprite2d::getColor(){return Vector3(m_Sprite->getColor().r,m_Sprite->getColor().g,m_Sprite->getColor().b);}
int Sprite2d::getColorApha(){return m_Sprite->getColor().a;}
std::string Sprite2d::getName(){return m_Name;}