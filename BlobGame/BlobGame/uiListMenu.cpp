#include "uiListMenu.h"
#include "uiButton.h"
#include "Text.h"
#include "inputManager.h"
#include "constants.h"
#include "common.h"


UiListMenu::UiListMenu(UiButton* scrollUp,UiButton* scrollDown,std::map<int,Text*>* entries,
					const int& textOffsetX,const int& textOffsetY,const int& maxVisibleEntries,int entryWidth) :
m_Up(scrollUp),
m_Down(scrollDown),
m_Entries(entries),
m_TextOffsetX(textOffsetX),
m_TextOffsetY(textOffsetY),
m_MaxEntriesVisible(maxVisibleEntries),
m_EntryWidth(entryWidth){
	InputManager::instance()->registerMouseInput(this,MOUSE_LB_RELEASED);
}
UiListMenu::UiListMenu(UiButton* scrollUp,UiButton* scrollDown,const int& textOffsetX,
					const int& textOffsetY,const int& maxVisibleEntries,int entryWidth) :
m_Up(scrollUp),
m_Down(scrollDown),
m_TextOffsetX(textOffsetX),
m_TextOffsetY(textOffsetY),
m_MaxEntriesVisible(maxVisibleEntries),
m_EntryWidth(entryWidth){
	m_Entries = new std::map<int,Text*>();
	InputManager::instance()->registerMouseInput(this,MOUSE_LB_RELEASED);
}

UiListMenu::~UiListMenu(){
	SafePtrRelease(m_Up);
	SafePtrRelease(m_Down);
	if(m_Entries){
		SafeMapDelete<int,Text>(*m_Entries);
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
			SafePtrRelease(entry);
			m_Entries->erase(i);
			return true;
		}
	}
	return false;
}
void UiListMenu::clearEntries(){
	std::map<int,Text*>::iterator i;
	for(i = m_Entries->begin();i != m_Entries->end();i++){
		SafePtrRelease(i->second);
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
int UiListMenu::getOffsetX() const{return m_TextOffsetX;}
int UiListMenu::getOffsetY() const{return m_TextOffsetY;}
int UiListMenu::getMaxVisibleEntries() const{return m_MaxEntriesVisible;}
std::map<int,Text*>* UiListMenu::getEntries() const{return m_Entries;}
Text* UiListMenu::getSelectedEntry() const{return m_Entries->at(m_Selected);}
Text* UiListMenu::getEntry(const int& p) const{return m_Entries->at(p);}

void UiListMenu::updateMenu(){
	std::map<int,Text*>::iterator i;
	for(i = m_Entries->begin();i != m_Entries->end();i++){
		i->second->setVisible(false);
	}
	Text* text = NULL;
	for(int i = m_StartOfVisibles;
		i < (m_StartOfVisibles+m_MaxEntriesVisible) || i < m_Entries->size();
		i++){
			text = m_Entries->at(i);
			text->setVisible(true);
			text->setPosition(getPositionX()+m_TextOffsetX,
				getPositionY()*(i-m_StartOfVisibles)+m_TextOffsetY);

	}
}