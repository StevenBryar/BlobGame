#ifndef UIBUTTON_H
#define UIBUTTON_H
#include "inputListener.h"
#include "gameObject.h"
#include <string>

enum ButtonAtt{
	FIRE_ON_PRESSED = (1 << 0),
	FIRE_ON_RELEASED = (1 << 1),
	HIGHLIGHT_ON_HOVER = (1 << 2),
};

class Sprite2d;
class Camera;
class UiButton : public GameObject,public InputListener{
public:
	UiButton(int offsetX,int offsetY,int width,int height,std::string defaultSprite,unsigned int buttonAtt,Camera* camera,void(*func)());
	UiButton(int offsetX,int offsetY,int width,int height,std::string defaultSprite,std::string downSprite,unsigned int buttonAtt,Camera* camera,void(*func)());
	UiButton(int offsetX,int offsetY,int width,int height,std::string defaultSprite,std::string downSprite,std::string highlightSprite,unsigned int buttonAtt,Camera* camera,void(*func)());
	virtual ~UiButton();

	void(*callBack)();

	void update();

	void addAttribute(ButtonAtt att);
	void setAttributes(unsigned int atts);
	void setCamera(Camera* camera);
	Camera* getCamera();

	void mouseInputCalback(inputEvent event,int x,int y);
protected:
	std::string m_DefaultSprite;
	std::string m_DownSprite;
	std::string m_HighlightSprite;
	unsigned int m_ButtonAtt;
	Camera* m_Camera;
	bool m_Hover;
	int m_OffsetX;
	int m_OffsetY;
};
#endif