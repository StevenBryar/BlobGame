#ifndef TEXT_H
#define TEXT_H

class sf::Font;
class sf::String;
#include <string>

class Text{
public:
	Text();
	Text(sf::Font font,std::string text);
	Text(sf::Font font,std::string text,int size);
	Text(sf::Font font,std::string text,int size,float x = 0,float y = 0);

	void setAttributes();
	void setFont(sf::Font font);
	void setText(std::string s);
	void setPosX(float x);
	void setPosY(float y);
	void setSize(int size);

	void update();

protected:
	sf::String* m_string;
};
#endif