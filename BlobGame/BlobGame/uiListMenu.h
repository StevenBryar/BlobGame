#ifndef UI_LIST_MENU_H
#define UI_LIST_MENU_H
#include "inputListener.h"
#include "gameObject.h"
#include <map>

class Text;
class UiButton;

class UiListMenu : public GameObject , public InputListener{
public:
	UiListMenu(UiButton* scrollUp,UiButton* scrollDown,std::map<int,Text*>* entries,
					const int& textOffsetX = 0,const int& textOffsetY = 0,const int& maxVisibleEntries = 3,int entryWidth = 30);
	UiListMenu(UiButton* scrollUp,UiButton* scrollDown,const int& textOffsetX = 0,
					const int& textOffsetY = 0,const int& maxVisibleEntries = 3,int entryWidth = 30);
	virtual ~UiListMenu();

	void addEntry(const int& position,Text* entry);
	bool deleteEntry(Text* entry);
	void clearEntries();

	void setOffsetX(const int& x);
	void setOffsetY(const int& y);
	void setUpButton(UiButton* up);
	void setDownButton(UiButton* down);
	void setMaxVisibleEntries(const int& max);
	void setEntries(std::map<int,Text*>* entry);
	void setSelectedEntry(const int& selected);

	UiButton* getUpButton() const;
	UiButton* getDownButton() const;
	int getOffsetX() const;
	int getOffsetY() const;
	int getMaxVisibleEntries() const;
	std::map<int,Text*>* getEntries() const;
	Text* getSelectedEntry() const;
	Text* getEntry(const int& p) const;

	void updateMenu();
protected:
	int m_TextOffsetX;
	int m_TextOffsetY;
	int m_MaxEntriesVisible;
	int m_Selected;
	int m_StartOfVisibles;
	int m_EntryWidth;
	std::map<int,Text*>* m_Entries;
	UiButton* m_Up;
	UiButton* m_Down;
};

#endif