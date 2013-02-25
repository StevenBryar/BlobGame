#ifndef TEXT_H
#define TEXT_H

#include "SFML\Graphics\Text.hpp"
#include <string>

enum textAttribute{
	FADING,
	RISING,
	FALLING,
	SHRINKING,
	GROWING,
	TO_THE_LEFT,
	TO_THE_RIGHT,
};

class Vector3;

class Text{
public:
	Text(sf::Font* font,std::string text,int size,Vector3 color,int alpha = 255,float x = 0,float y = 0);

	void setText(std::string s);
	void setPosition(float x,float y);
	void setSize(int size);
	void setColor(Vector3 rgb,int alpha);
	void addAttribute(textAttribute att);
	void setAttributes(textAttribute att);

	void update();

protected:
	unsigned int m_TextAtt;
	sf::Text* m_String;
};
#endif