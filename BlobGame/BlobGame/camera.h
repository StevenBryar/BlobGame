#ifndef Camera_H
#define Camera_H
#include <SFML/Graphics.hpp>

class GameObject;

//wrap around SFML view class.
class Camera{
public:
	Camera();
	Camera(sf::View* view);
	~Camera();
	void rotate(float rotation);
	void moveTo(float x,float y);
	void centre(float x,float y);
	void zoom(float z);
	void move(float x,float y);
	void setSize(float x,float y);
	void setView(sf::View* view);
	void setFollow(GameObject* thing);

	float getRotation();
	GameObject* getFollow();
	sf::View* getView();
	float getWidth();
	float getHeight();
	float getCentreX();
	float getCentreY();
	float getWorldPosX();
	float getWorldPosY();

private:
	sf::View* m_View;
	GameObject* m_Follow;
};
#endif