#include "Text.h"
#include "SFML\Graphics.hpp"
#include "SFML\Graphics\Font.hpp"
#include "Vector3.h"
#include "common.h"

Text::Text(sf::Font* font,std::string text,int size,Vector3 color,int alpha,float x,float y){
	m_String->setFont(*font);
	m_String->setCharacterSize(size);
	m_String->setString(text);
	m_String->setPosition(x,y);
	sf::Color tColor(color.X,color.Y,color.Z,alpha);
	m_String->setColor(tColor);
}

void Text::update(){
	
}
void Text::setText(std::string s){
	m_String->setString(s);
}
void Text::setPosition(float x,float y){
	m_String->setPosition(x,y);
}
void Text::setSize(int size){
	m_String->setCharacterSize(size);
}
void Text::setColor(Vector3 rgb,int alpha){
	sf::Color tColor(rgb.X,rgb.Y,rgb.Z,alpha);
	m_String->setColor(tColor);
}
void Text::addAttribute(textAttribute att){

}
void Text::setAttributes(textAttribute att){

}