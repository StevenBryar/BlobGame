#ifndef UI_TEXT_BOX
#define UI_TEXT_BOX
#include "uiElement.h"
#include "MessageListener.h"
#include <string>
#include <vector>

class Text;
class Vector3;

class UiTextBox : public UiElement , public MessageListner{
public:
	UiTextBox(Text* text,Camera* camera,int x = 0,int y = 0,int width = 128,int height = 30);
	UiTextBox(std::string font,int fontSize,std::string text,Vector3 color,Camera* camera,
				int x = 0,int y = 0,int width = 128,int height = 30);
	~UiTextBox();
	bool getFocus();
	void setFocux(bool focus);
	std::string getString();
	void setString(std::string aString);
	Text* getText();
	void setText(Text* text);
	void appendString(std::string aString);

	static std::vector<UiTextBox*>* getTextBoxes();
protected:
	Text* m_Text;
	bool m_HasFocus;
	void handleMessage(const Message& msg);
	static std::vector<UiTextBox*>* m_TextBoxes;
};
#endif