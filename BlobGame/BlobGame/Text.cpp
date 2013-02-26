#include "Text.h"
#include "SFML\Graphics.hpp"
#include "SFML\Graphics\Font.hpp"
#include "Vector3.h"
#include "common.h"

Text::Text(sf::Font* font,std::string text,int size,Vector3 color,textAttribute atts,int alpha,float x,float y){
	m_String->setFont(*font);
	m_String->setCharacterSize(size);
	m_String->setString(text);
	m_String->setPosition(x,y);
	sf::Color tColor(color.X,color.Y,color.Z,alpha);
	m_String->setColor(tColor);
	m_TextAtt = atts;
}

void Text::update(){
	if(ContainsFlags(m_TextAtt,FADING) &&
	   getAlpha() != 0){
		setColor(getColor(),getAlpha()-2);
	}
	if(ContainsFlags(m_TextAtt,RISING)){
		setPosition(getPosX(),getPosY()-0.5);
	}
	if(ContainsFlags(m_TextAtt,FALLING)){
		setPosition(getPosX(),getPosY()+0.5);
	}
	if(ContainsFlags(m_TextAtt,SHRINKING) &&
		getSize() > 0){
		setSize(getSize()-1);
	}
	if(ContainsFlags(m_TextAtt,GROWING)){
		setSize(getSize()+1);
	}
	if(ContainsFlags(m_TextAtt,TO_THE_LEFT)){
		setPosition(getPosX()-0.5,getPosY());
	}
	if(ContainsFlags(m_TextAtt,TO_THE_RIGHT)){
		setPosition(getPosX()+0.5,getPosY());
	}
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
	if(!ContainsFlags(m_TextAtt,att)){
		m_TextAtt |= att;
	}
}
void Text::setAttributes(textAttribute att){
	m_TextAtt = att;
}
std::string Text::getText(){return m_String->getString();}
float Text::getPosX(){return m_String->getPosition().x;}
float Text::getPosY(){return m_String->getPosition().y;}
int Text::getSize(){return m_String->getCharacterSize();}
Vector3 Text::getColor(){
	Vector3 color(m_String->getColor().r,
				  m_String->getColor().g,
				  m_String->getColor().b);
	return color;
}	
int Text::getAlpha(){return m_String->getColor().a;}
unsigned int Text::getAttributes(){return m_TextAtt;}