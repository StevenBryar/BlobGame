#ifndef Sprite2d_H
#define Sprite2d_H

#include "SFML/Graphics.hpp"

class GameObject;
class Vector3;
#include <string>

class Sprite2d{
	friend class SpriteManager;
	friend class Renderer;
public:
	Sprite2d(GameObject* entity,sf::Texture* texture);
	~Sprite2d();
	void update();
	void scale(float x,float y);
	void setScale(float x,float y);
	void setRotation(float r);
	void setOrigin(float x,float y);
	void setPosition(float x,float y);
	void setWidth(float w);
	void setHeight(float h);
	void setColor(Vector3 rgb,int a = 255);
	void setColor(int r,int g,int b,int a = 255);
	void setHiden(bool hidden);
	void setName(std::string name);
	Vector3 getColor();
	int getColorApha();
	float getRotation();
	float getOriginX();
	float getScaleX();
	float getPositionX();
	float getOriginY();
	float getScaleY();
	float getPositionY();
	float getWidth();
	float getHeight();
	std::string getName();
	bool getHidden();

	void move(float x,float y);
protected:
	sf::Sprite* m_Sprite;
	GameObject* m_Owner;
	int m_SpriteNumber;
	static int m_SpriteCount;
	bool m_Hidden;
	std::string m_Name;
};

#endif



