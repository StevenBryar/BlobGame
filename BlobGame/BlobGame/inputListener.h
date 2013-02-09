#ifndef INPUT_LISTENER_H
#define INPUT_LISTENER_H	

enum keyType;
enum inputEvent;

class InputListener{
public:
	virtual void keyInputCallback(keyType key,inputEvent event);
	virtual void mouseInputCalback(inputEvent event,int x,int y);
};

#endif