#ifndef TEXT_MANAGER_H
#define TEXT_MANAGER_H
#include "MessageListener.h"
#include <vector>
#include "SFML\Graphics\Text.hpp"


class Text;
class Vector3;
enum textAttribute;
class TextManager : public MessageListener{
public:
	static TextManager* instance();
	void cleanupInstance();
	Text* createText(std::string text,std::string fontName,int fontSize,
						Vector3 color,int alpha,int x,int y,textAttribute atts,
							bool deleteAfterTime,float timeTillDelete);
	void deleteText(Text* text);
	void loadFont(const std::string file);
	std::vector<Text*>* getTexts();
	void update();

protected:
	TextManager();
	~TextManager();
	static TextManager* m_Instance;

	std::vector<Text*>* m_Texts;
	std::map<std::string,sf::Font*>* m_Fonts;
	void handleMessage(Message msg);
};
#endif