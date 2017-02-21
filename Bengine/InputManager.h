#pragma once
#include <unordered_map>
#include <glm/glm.hpp>
namespace Bengine {

class InputManager
{
public:
	InputManager();
	~InputManager();

	void update();

	void pressKey(unsigned int keyID);
	void releaseKey(unsigned int keyID);

	void setMouseCoords(float x, float y);

	//return true if the key is held down
	bool isKeyDown(unsigned int keyID);
	

	//return true if the key just pressed
	bool isKeyPressed(unsigned int keyID);

	glm::vec2 getMouseCoords() const {
		return _mouseCoords;
	}
private:
	//return true if the key is held down
	bool wasKeyDown(unsigned int keyID);

	std::unordered_map<unsigned int, bool> _keyMap;
	std::unordered_map<unsigned int, bool> _previouskeyMap;
	glm::vec2 _mouseCoords;
};


}