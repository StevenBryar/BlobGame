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
	Text(sf::Font* font,const std::string& text,const int& size,const Vector3& color,const textAttribute& atts,const int& alpha = 255,const float& x = 0,const float& y = 0);

	void setText(const std::string s);
	void setPosition(const float x,const float y);
	void setSize(const int size);
	void setColor(const Vector3 rgb,const int alpha);
	void addAttribute(const textAttribute att);
	void setAttributes(const textAttribute att);

	std::string getText() const;
	float getPosX() const;
	float getPosY() const;
	int getSize() const;
	Vector3 getColor() const;
	int getAlpha() const;
	unsigned int getAttributes() const;

	void update();

protected:
	unsigned int m_TextAtt;
	sf::Text* m_String;
};
#endif