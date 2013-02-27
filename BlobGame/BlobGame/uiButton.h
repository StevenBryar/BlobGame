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
	UiButton(const int& offsetX,const int& offsetY,const int& width,const int& height,const std::string& defaultSprite,const unsigned int& buttonAtt,Camera* camera,void(*func)());
	UiButton(const int& offsetX,const int& offsetY,const int& width,const int& height,const std::string& defaultSprite,const std::string& downSprite,const unsigned int& buttonAtt,Camera* camera,void(*func)());
	UiButton(const int& offsetX,const int& offsetY,const int& width,const int& height,const std::string& defaultSprite,const std::string& downSprite,
		    const std::string& highlightSprite,const unsigned int& buttonAtt,Camera* camera,void(*func)());
	virtual ~UiButton();

	void(*callBack)();

	void update();

	void addAttribute(const ButtonAtt& att);
	void setAttributes(const unsigned int& atts);
	void setCamera(Camera* camera);
	Camera* getCamera() const;

	void mouseInputCalback(const inputEvent& event,const int& x,const int& y);
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