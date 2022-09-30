#include <set>

#include "GL/freeglut.h"
#include "Renderer/renderer.hpp"
#include "Util/math.hpp"
#include "Logic/logic.hpp"
#include "Control/controller.hpp"
#include "Util/random.hpp"

void initGLUT(const char* nome, int argc, char** argv) {
	//glutIgnoreKeyRepeat(1);
	//glutKeyboardFunc(normalKeysDown);
	//glutKeyboardUpFunc(normalKeysUp);

	//glutSpecialFunc(specialKeysDown);
	//glutSpecialUpFunc(specialKeysUp);

	//glutMouseFunc(mouseButton);
	//glutPassiveMotionFunc(mouseMove);
	//glutMotionFunc(pressedMouseMove);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);

	Renderer::getInstance();
	Controller::getInstance(); 
	Logic::getInstance();
	
	glutMainLoop();

	return 0;
}
