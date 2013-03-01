#ifndef UI_LIST_MENU_H
#define UI_LIST_MENU_H
#include "inputListener.h"
#include "gameObject.h"
#include <map>

class Text;
class UiButton;

class UiListMenu : public GameObject , public InputListener{
public:


protected:
	int m_TextOffsetX;
	int m_TextOffsetY;
	int m_MaxEntriesVisible;
	std::map<int,Text*>* m_Entries;
	UiButton* m_Up;
	UiButton* m_Down;
};

#endif