#include "InputManager.h"
namespace Bengine {


	InputManager::InputManager() : _mouseCoords(0.0f)
	{
	}


	InputManager::~InputManager()
	{
	}

	void InputManager::update() {
		/*
		
		for (auto it = _keyMap.begin(); it != _keyMap.end(); it++) {
			_previouskeyMap[it->first] = it->second;
		}
		*/

		for (auto& it : _keyMap) {
			_previouskeyMap[it.first] = it.second;
		}
	}

	void InputManager::pressKey(unsigned int keyID) {
		_keyMap[keyID] = true; 
	}

	void InputManager::releaseKey(unsigned int keyID) {
		_keyMap[keyID] = false;
	}
	void InputManager::setMouseCoords(float x, float y) {
		_mouseCoords.x = x;
		_mouseCoords.y = y;

	}


	bool InputManager::isKeyDown(unsigned int keyID) {
		auto it = _keyMap.find(keyID);
		if (it != _keyMap.end()) {
			return it->second;
		}
		else {
			return false;
		}
	}

	bool InputManager::isKeyPressed(unsigned int keyID) {

		//check if it is pressed this frame, and wasn't pressed last frame
		if (isKeyDown(keyID) == true && wasKeyDown(keyID) == false) {
			return true;
		}
		return false;
	}

	bool InputManager::wasKeyDown(unsigned int keyID) {
		auto it = _previouskeyMap.find(keyID);
		if (it != _previouskeyMap.end()) {
			return it->second;
		}
		else {
			return false;
		}
	}



}