#ifndef InputManager_H
#define InputManager_H

#include <map>
#include <vector>

class InputListener;
enum keyType;
enum inputEvent;

struct InputType{
public:
	inputEvent event;
	keyType key;

	InputType(inputEvent aEvent,keyType aKey)
	{event = aEvent;key = aKey;}

	bool operator==(const InputType& type) const{
		if(this->event == type.event &&
			this->key == type.key)
				return true;

		return false;
	}
	bool operator!=(const InputType& type) const{
		return !(*this == type);
	}
};

class InputManager{
public:
	static InputManager* instance();
	static void cleanUpInstance();

	void registerKeyinput(InputListener* listener,
							keyType key,inputEvent event);
	void registerMouseInput(InputListener* listener,
								inputEvent event);

	void handleKeyInput(inputEvent event,keyType key);
	void handleMouseInput(inputEvent event,int x,int y);

	void pauseInput(InputListener* listener,InputType type);
	void pauseAllOf(InputListener* listener);
	void pauseAll();

	void resumeInput(InputListener* listener,InputType type);
	void resumeAllOf(InputListener* listener);
	void resumeAll();

	void removeInput(InputListener* listener,InputType type);
	void removeAllOf(InputListener* listener);

	void update();

private:
	InputManager();
	~InputManager();

	static InputManager* m_Instance;

	std::multimap<InputListener*, InputType>* m_InputRegistry;
	std::multimap<InputListener*, InputType>* m_InputsPaused;
	std::multimap<InputListener*, InputType>* m_InputsToAdd;
	std::multimap<InputListener*, InputType>* m_InputsToRemove;
};
#endif