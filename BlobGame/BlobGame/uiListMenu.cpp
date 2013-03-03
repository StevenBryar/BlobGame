#include "uiListMenu.h"
#include "uiButton.h"
#include "Text.h"
#include "inputManager.h"
#include "constants.h"
#include "common.h"
#include "Vector3.h"
#include "camera.h"
#include "TextManager.h"

UiListMenu::UiListMenu(const int& screenPosX,const int& screenPosY,Camera* camera,
						UiButton* scrollUp,UiButton* scrollDown,const int& textOffsetX,
							const int& textOffsetY,const int& maxVisibleEntries,int entryHeight) :
m_Up(scrollUp),
m_Down(scrollDown),
m_TextOffsetX(textOffsetX),
m_TextOffsetY(textOffsetY),
m_MaxEntriesVisible(maxVisibleEntries),
m_EntryHeight(entryHeight),
UiElement(screenPosX,screenPosY,camera){
	m_Entries = new std::map<int,Text*>();
	InputManager::instance()->registerMouseInput(this,MOUSE_LB_RELEASED);
	if(m_Up){
		m_Up->setCallBackParam(this);
	}
	if(m_Down){
		m_Down->setCallBackParam(this);
	}
	m_StartOfVisibles = 1;
	m_Selected = 1;
}

UiListMenu::~UiListMenu(){
	SafePtrRelease(m_Up);
	SafePtrRelease(m_Down);
	if(m_Entries){
		clearEntries();
	}
	SafePtrRelease(m_Entries);
}
void UiListMenu::addEntry(const int& position,Text* entry){
	if(m_Entries){
		m_Entries->insert(std::pair<int,Text*>(position,entry));
	}
	updateMenu();
}
bool UiListMenu::deleteEntry(Text* entry){
	std::map<int,Text*>::iterator i;
	for(i = m_Entries->begin();i != m_Entries->end();i++){
		if(i->second == entry){
			TextManager::instance()->deleteText(entry);
			m_Entries->erase(i);
			return true;
		}
	}
	return false;
}
void UiListMenu::clearEntries(){
	std::map<int,Text*>::iterator i;
	for(i = m_Entries->begin();i != m_Entries->end();i++){
		TextManager::instance()->deleteText(i->second);
	}
	m_Entries->clear();
}
void UiListMenu::setOffsetX(const int& x){m_TextOffsetX = x; updateMenu();}
void UiListMenu::setOffsetY(const int& y){m_TextOffsetY = y; updateMenu();}
void UiListMenu::setUpButton(UiButton* up){m_Up = up;}
void UiListMenu::setDownButton(UiButton* down){m_Down = down;}
void UiListMenu::setMaxVisibleEntries(const int& max){m_MaxEntriesVisible = max; updateMenu();}
void UiListMenu::setEntries(std::map<int,Text*>* entry){m_Entries = entry; updateMenu();}
void UiListMenu::setSelectedEntry(const int& selected){m_Selected = selected;updateMenu();}

UiButton* UiListMenu::getUpButton() const{return m_Up;}
UiButton* UiListMenu::getDownButton() const{return m_Down;}	
int UiListMenu::getTextOffsetX() const{return m_TextOffsetX;}
int UiListMenu::getTextOffsetY() const{return m_TextOffsetY;}
int UiListMenu::getMaxVisibleEntries() const{return m_MaxEntriesVisible;}
std::map<int,Text*>* UiListMenu::getEntries() const{return m_Entries;}
Text* UiListMenu::getSelectedEntry() const{return m_Entries->at(m_Selected);}
Text* UiListMenu::getEntry(const int& p) const{return m_Entries->at(p);}

void UiListMenu::update(){
	if(m_Camera){
		setPositionX(m_Camera->getWorldPosX()+m_ScreenPosX);
		setPositionY(m_Camera->getWorldPosY()+m_ScreenPosY);
	}
	if(m_Up){
		m_Up->update();
	}
	if(m_Down){
		m_Down->update();
	}
	Text* text = NULL;
	for(int i = m_StartOfVisibles;
		i < (m_StartOfVisibles+m_MaxEntriesVisible) && (i-1) < m_Entries->size();
		i++){
		text = m_Entries->at(i);
		text->setPosition(
			getPositionX()+m_TextOffsetX,
			getPositionY()+(m_EntryHeight*(i-m_StartOfVisibles)+m_TextOffsetY));
	}
}

void UiListMenu::updateMenu(){
	std::map<int,Text*>::iterator iterator;
	for(iterator = m_Entries->begin();iterator != m_Entries->end();iterator++){
		iterator->second->setVisible(false);
		Vector3 c(200,30,40);
		iterator->second->setColor(c,iterator->second->getAlpha());
	}
	Text* text = NULL;
	for(int i = m_StartOfVisibles;
		i < (m_StartOfVisibles+m_MaxEntriesVisible) && (i-1) < m_Entries->size();
		i++){
		text = m_Entries->at(i);
		text->setVisible(true);
		text->setPosition(
			getScreenPosX()+m_TextOffsetX,
			getScreenPosY()+(m_EntryHeight*(i-m_StartOfVisibles)+m_TextOffsetY));
		if(text == getSelectedEntry()){
			Vector3 c(80,200,40);
			text->setColor(c,text->getAlpha());
		}
	}
}

void UiListMenu::mouseInputCalback(const inputEvent& event,
									const int& x,const int& y){
	switch(event){
	case MOUSE_LB_RELEASED:
		if(x > getScreenPosX() && x < (getScreenPosX()+getWidth()) &&
		   y > getScreenPosY() && y < (getScreenPosY()+getHeight())){
			Text* text = NULL;
		   	for(int i = m_StartOfVisibles;
			i < (m_StartOfVisibles+m_MaxEntriesVisible) && (i-1) < m_Entries->size();
			i++){
				text = m_Entries->at(i);
				if(y > getScreenPosY()+(m_EntryHeight*(i-m_StartOfVisibles)) && 
				   y < getScreenPosY()+(m_EntryHeight*((i+1)-m_StartOfVisibles))){
					setSelectedEntry(i);
					return;
				}
			}
		}
	}
}