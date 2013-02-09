#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "inputListener.h"
#include <string>

class Sprite2d;

class GameObject{
public:
float getRotation();
float getPositionX();
float getOriginX();
float getPositionY();
float getOriginY();
float getHeight();
float getWidth();
virtual std::string getType();
std::string getName();
Sprite2d* getSprite();

void setRotation(float r);
void setPositionX(float x);
void setOriginX(float x);
void setPositionY(float y);
void setOriginY(float y);
void setHeight(float h);
void setWidth(float w);
void setSprite(Sprite2d* sprite);

void setPosition(float x,float y);
void setOrigin(float x,float y);
void setSize(float height,float width);

	GameObject();
	virtual ~GameObject();

	virtual void update();

protected:
	float m_Rotation;
	float m_PositionX;
	float m_PositionY;
	float m_OriginX;
	float m_OriginY;
	float m_Height;
	float m_Width;
	std::string m_Name;
	Sprite2d* m_Sprite;
};

#endif