#pragma once

#include "../Renderer/renderer.hpp"
#include <set>

class Controller {
private:
	std::set<int> pressedKeys;
	std::set<int> pressedSpecialKeys;

	Controller();
public:
	static Controller &getInstance();
	void updateKeys();

	inline Controller& keyboardDown(unsigned char key, int x, int y) {
		pressedKeys.insert(key);
		return *this;
	}

	inline Controller& keyboardUp(unsigned char key, int x, int y) {
		pressedKeys.erase(key);
		return *this;
	}

	inline Controller& specialKeyboardDown(int key, int x, int y) {
		pressedSpecialKeys.insert(key);
		return *this;
	}

	inline Controller& specialKeyboardUp(int key, int x, int y) {
		pressedSpecialKeys.erase(key);
		return *this;
	}

	bool inline isPressed(int key) {
		return pressedKeys.count(key);
	}

	bool inline specialIsPressed(int key) {
		return pressedSpecialKeys.count(key);
	}
};

void keyboardDownCB(unsigned char key, int x, int y);
void keyboardUpCB(unsigned char key, int x, int y);

void specialKeyboardDownCB(int key, int x, int y);
void specialKeyboardUpCB(int key, int x, int y);