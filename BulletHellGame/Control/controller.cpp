#include "controller.hpp"
#include "../Logic/logic.hpp"

Controller::Controller() {
	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(keyboardDownCB);
	glutKeyboardUpFunc(keyboardUpCB);

	glutSpecialFunc(specialKeyboardDownCB);
	glutSpecialUpFunc(specialKeyboardUpCB);
}

Controller& Controller::getInstance() {
	static Controller instance;
	return instance;
}

void Controller::updateKeys() {
	Logic::getInstance().onKeysUpdate();
}

void keyboardDownCB(unsigned char key, int x, int y) {
	Controller::getInstance()
		.keyboardDown(key, x, y)
		.updateKeys();
}

void keyboardUpCB(unsigned char key, int x, int y) {
	Controller::getInstance()
		.keyboardUp(key, x, y)
		.updateKeys();
}

void specialKeyboardDownCB(int key, int x, int y) {
	Controller::getInstance()
		.specialKeyboardDown(key, x, y)
		.updateKeys();
}

void specialKeyboardUpCB(int key, int x, int y) {
	Controller::getInstance()
		.specialKeyboardUp(key, x, y)
		.updateKeys();
}