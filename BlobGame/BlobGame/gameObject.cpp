#include "GameObject.h"
#include "spriteManager.h"
#include "2dSprite.h"

GameObject::GameObject() :
m_PositionX(0),m_PositionY(0),
m_Rotation(0),
m_OriginX(0),m_OriginY(0),
m_Height(64),m_Width(64),m_Name(""),
m_Sprite(NULL){}

GameObject::~GameObject(){
	SpriteManager::instance()->deleteSprite(m_Sprite);
}
void GameObject::update(){}

std::string GameObject::getType(){
	return "GameObject";
}
std::string GameObject::getName(){return m_Name;}
float GameObject::getRotation(){return m_Rotation;}
float GameObject::getPositionX(){return m_PositionX;}
float GameObject::getOriginX(){return m_OriginX;}
float GameObject::getPositionY(){return m_PositionY;}
float GameObject::getOriginY(){return m_OriginY;}
float GameObject::getHeight(){return m_Height;}
float GameObject::getWidth(){return m_Width;}
Sprite2d* GameObject::getSprite(){return m_Sprite;}

void GameObject::setRotation(float r){m_Rotation = r;}
void GameObject::setPositionX(float x){m_PositionX = x;}
void GameObject::setOriginX(float x){m_OriginX = x;}
void GameObject::setPositionY(float y){m_PositionY = y;}
void GameObject::setOriginY(float y){m_OriginY = y;}
void GameObject::setHeight(float h){m_Height = h;}
void GameObject::setWidth(float w){m_Width = w;}
void GameObject::setSprite(Sprite2d* sprite){
	if(m_Sprite){
	SpriteManager::instance()->deleteSprite(m_Sprite);
	}
	m_Sprite = sprite;
	
}

void GameObject::setPosition(float x,float y){
	setPositionX(x);
	setPositionY(y);
}
void GameObject::setOrigin(float x,float y){
	setOriginX(x);
	setOriginY(y);
}
void GameObject::setSize(float height,float width){
	setHeight(height);
	setWidth(width);
}