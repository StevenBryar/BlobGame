#include "uiButton.h"
#include "2dSprite.h"
#include "inputManager.h"
#include "camera.h"
#include "common.h"
#include "constants.h"
#include "spriteManager.h"

UiButton::UiButton(const int& offsetX,const int& offsetY,const int& width,const int& height,const std::string& defaultSprite,
					const unsigned int& buttonAtt,Camera* camera,void(*call)()) :
m_DefaultSprite(defaultSprite),
m_ButtonAtt(buttonAtt),
m_Camera(camera),
callBack(call),
m_OffsetX(offsetX),
m_OffsetY(offsetY){
	setWidth(width);
	setHeight(height);
	m_Hover = false;
	InputManager::instance()->registerMouseInput(this,MOUSE_MOVED);
	InputManager::instance()->registerMouseInput(this,MOUSE_LB_PRESSED);
	InputManager::instance()->registerMouseInput(this,MOUSE_LB_RELEASED);
	m_Sprite = SpriteManager::instance()->createSprite(this,m_DefaultSprite,0);
}
UiButton::UiButton(const int& offsetX,const int& offsetY,const int& width,const int& height,const std::string& defaultSprite,
					const std::string& downSprite,const unsigned int& buttonAtt,Camera* camera,void(*call)()) :
m_DefaultSprite(defaultSprite),
m_DownSprite(downSprite),
m_ButtonAtt(buttonAtt),
m_Camera(camera),
callBack(call),
m_OffsetX(offsetX),
m_OffsetY(offsetY){
	setWidth(width);
	setHeight(height);
	m_Hover = false;
	InputManager::instance()->registerMouseInput(this,MOUSE_MOVED);
	InputManager::instance()->registerMouseInput(this,MOUSE_LB_PRESSED);
	InputManager::instance()->registerMouseInput(this,MOUSE_LB_RELEASED);
	m_Sprite = SpriteManager::instance()->createSprite(this,m_DefaultSprite,0);
}
UiButton::UiButton(const int& offsetX,const int& offsetY,const int& width,const int& height,const std::string& defaultSprite,const std::string& downSprite,
					const std::string& highlightSprite,const unsigned int& buttonAtt,Camera* camera,void(*call)()) :
m_DefaultSprite(defaultSprite),
m_DownSprite(downSprite),
m_HighlightSprite(highlightSprite),
m_ButtonAtt(buttonAtt),
m_Camera(camera),
callBack(call),
m_OffsetX(offsetX),
m_OffsetY(offsetY){
	setWidth(width);
	setHeight(height);
	m_Hover = false;
	InputManager::instance()->registerMouseInput(this,MOUSE_MOVED);
	InputManager::instance()->registerMouseInput(this,MOUSE_LB_PRESSED);
	InputManager::instance()->registerMouseInput(this,MOUSE_LB_RELEASED);
	m_Sprite = SpriteManager::instance()->createSprite(this,m_DefaultSprite,0);
}

UiButton::~UiButton(){
	InputManager::instance()->removeAllOf(this);
}

void UiButton::update(){
	if(m_Camera){
		setPositionX(m_Camera->getWorldPosX()+m_OffsetX);
		setPositionY(m_Camera->getWorldPosY()+m_OffsetY);
	}
}
void UiButton::addAttribute(const ButtonAtt& att){
	if(!ContainsFlags(m_ButtonAtt,att)){
		m_ButtonAtt |= att;
	}
}
void UiButton::setAttributes(const unsigned int& atts){m_ButtonAtt = atts;}
void UiButton::setCamera(Camera* camera){m_Camera  = camera;}
Camera* UiButton::getCamera() const{return m_Camera;}

void UiButton::mouseInputCalback(const inputEvent& event,const int& x,const int& y){
	switch(event){
	case MOUSE_MOVED:
		if(!m_Hover && ContainsFlags(m_ButtonAtt,HIGHLIGHT_ON_HOVER)  &&
						!m_HighlightSprite.empty() &&
						(x > m_OffsetX && x < (m_OffsetX+m_Width) &&
						y > m_OffsetY && y < (m_OffsetY+m_Height))){
			m_Hover = true;
			SpriteManager::instance()->deleteSprite(m_Sprite);
			m_Sprite = SpriteManager::instance()->createSprite(this,m_HighlightSprite,0);
		}
		else if(m_Hover && 
			x < m_OffsetX || x > (m_OffsetX+m_Width) ||
			y < m_OffsetY || y > (m_OffsetY+m_Height)){
			m_Hover = false;
			SpriteManager::instance()->deleteSprite(m_Sprite);
			m_Sprite = SpriteManager::instance()->createSprite(this,m_DefaultSprite,0);
		}
		break;
	case MOUSE_LB_PRESSED:
		if(ContainsFlags(m_ButtonAtt,FIRE_ON_PRESSED) &&
		  (x > m_OffsetX && x < (m_OffsetX+m_Width) &&
		   y > m_OffsetY && y < (m_OffsetY+m_Height)) &&
		   callBack){
		  	callBack();
		}
		if(!m_DownSprite.empty() &&
		  (x > m_OffsetX && x < (m_OffsetX+m_Width) &&
		   y > m_OffsetY && y < (m_OffsetY+m_Height))){
		   SpriteManager::instance()->deleteSprite(m_Sprite);
		   m_Sprite = SpriteManager::instance()->createSprite(this,m_DownSprite,0);
		}
		break;
	case MOUSE_LB_RELEASED:
		if(ContainsFlags(m_ButtonAtt,FIRE_ON_RELEASED) &&
		  (x > m_OffsetX && x < (m_OffsetX+m_Width) &&
		   y > m_OffsetY && y < (m_OffsetY+m_Height)) &&
		   callBack){
		  	callBack();
		}
		if(!m_DefaultSprite.empty()){
		   SpriteManager::instance()->deleteSprite(m_Sprite);
		   m_Sprite = SpriteManager::instance()->createSprite(this,m_DefaultSprite,0);
		}
		break;
	}
}