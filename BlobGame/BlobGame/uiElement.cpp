#include "uiElement.h"
#include "camera.h"

UiElement::UiElement(const int& posX,const int& posY,Camera* camera) :
m_ScreenPosX(posX),
m_ScreenPosY(posY),
m_Camera(camera),
GameObject(){}
UiElement::~UiElement(){

}

void UiElement::update(){
	if(m_Camera){
		setPositionX(m_Camera->getWorldPosX()+m_ScreenPosX);
		setPositionY(m_Camera->getWorldPosY()+m_ScreenPosY);
	}
}

void UiElement::setCamera(Camera* camera){m_Camera = camera;}
void UiElement::setScreenPos(const int& screenPosX,const int& screenPosY){
	m_ScreenPosX = screenPosX;
	m_ScreenPosY = screenPosY;
}

Camera* UiElement::getCamera() const{return m_Camera;}
int UiElement::getScreenPosX() const{return m_ScreenPosX;}
int UiElement::getScreenPosY() const{return m_ScreenPosY;}