#include "uiTextBox.h"
#include "Vector3.h"
#include "Text.h"
#include "common.h"
#include "TextManager.h"
#include "MessageHandler.h"
#include "constants.h"
#include "camera.h"

std::vector<UiTextBox*>* UiTextBox::m_TextBoxes = new std::vector<UiTextBox*>();

UiTextBox::UiTextBox(Text* text,Camera* camera,int x,int y,int width,int height) :
UiElement(x,y,camera){
	m_Width = width;
	m_Height = height;
	m_Text = text;
	if(!m_TextBoxes){
		m_TextBoxes = new std::vector<UiTextBox*>();
	}
	m_TextBoxes->push_back(this);
}
UiTextBox::UiTextBox(std::string font,int fontSize,std::string text,Vector3 color,Camera* camera,
		int x,int y,int width,int height) : 
UiElement(x,y,camera){
	m_Width = width;
	m_Height = height;
	m_Text = TextManager::instance()->createText
		(text,font,fontSize,color,255,x,y,0,false,0);
	if(!m_TextBoxes){
		m_TextBoxes = new std::vector<UiTextBox*>();
	}
	m_TextBoxes->push_back(this);
}

UiTextBox::~UiTextBox(){
	for(int i = 0;i < m_TextBoxes->size();i++){
		if((*m_TextBoxes)[i] == this){
			m_TextBoxes->erase(m_TextBoxes->begin()+i);
			break;
		}
	}
	if(m_TextBoxes->size() == 0){
		SafePtrRelease(m_TextBoxes);
	}
	TextManager::instance()->deleteText(m_Text);
}

void UiTextBox::update(){
	if(m_Camera){
		setPositionX(m_Camera->getWorldPosX()+m_ScreenPosX);
		setPositionY(m_Camera->getWorldPosY()+m_ScreenPosY);
		m_Text->setPosition(getPositionX(),getPositionY());
	}
}

bool UiTextBox::getFocus(){return m_HasFocus;}
void UiTextBox::setFocus(bool focus){m_HasFocus = focus;}
std::string UiTextBox::getString(){return m_Text->getText();}
void UiTextBox::setString(std::string aString){m_Text->setText(aString);}
Text* UiTextBox::getText(){return m_Text;}
void UiTextBox::setText(Text* text){m_Text = text;}
void UiTextBox::appendString(std::string aString){
	m_Text->setText(m_Text->getText()+aString);
}

std::vector<UiTextBox*>* UiTextBox::getTextBoxes(){
	if(!m_TextBoxes){
		m_TextBoxes = new std::vector<UiTextBox*>();
	}
	return m_TextBoxes;
}

void UiTextBox::handleMessage(const Message& msg){
	switch(msg.type){
	case TEXT_ENTERED:{
		unsigned int chr = (unsigned int)msg.extraInfo;
		if(chr == 8){
			std::string stng = m_Text->getText();
			
		}
		else if(chr != 8){
			appendString(std::string((char*)&chr));
		}
	 }
	 break;
	default:
		break;
	}
}