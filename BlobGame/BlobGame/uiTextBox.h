#ifndef UI_TEXT_BOX
#define UI_TEXT_BOX
#include "uiElement.h"
#include <string>

class Font;
class Text;

class UiTextBox : public UiElement , public MessageListner{

protected:
	Text* m_Text;
	bool m_HasFocus;
};
#endif