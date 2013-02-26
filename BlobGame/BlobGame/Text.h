#ifndef TEXT_H
#define TEXT_H

#include "SFML\Graphics\Text.hpp"
#include <string>

enum textAttribute{
	FADING = (1 << 0),
	RISING = (1 << 1),
	FALLING = (1 << 2),
	SHRINKING = (1 << 3),
	GROWING = (1 << 4),
	TO_THE_LEFT = (1 << 5),
	TO_THE_RIGHT = (1 << 6),
};

class Vector3;

class Text{
public:
	Text(sf::Font* font,std::string text,int size,Vector3 color,textAttribute atts,int alpha = 255,float x = 0,float y = 0);

	void setText(std::string s);
	void setPosition(float x,float y);
	void setSize(int size);
	void setColor(Vector3 rgb,int alpha);
	void addAttribute(textAttribute att);
	void setAttributes(textAttribute att);

	std::string getText();
	float getPosX();
	float getPosY();
	int getSize();
	Vector3 getColor();
	int getAlpha();
	unsigned int getAttributes();

	void update();

protected:
	unsigned int m_TextAtt;
	sf::Text* m_String;
};
#endif