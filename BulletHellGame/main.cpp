#include "GL/freeglut.h"
#include "objectUtil.hpp"
#include "actor.hpp"

int ms = 1000 / 30;
vis::Actor actor;

void drawActor(vis::Actor actor) {

	vis::GameObject gameObject = actor.getGameObject();
	
	vis::Vector3
		pos = actor.getPosition(),
		rot = actor.getRotation(),
		* vertices = gameObject.getVertices(),
		* normals = gameObject.getNormals(),
		* uvCordinates = gameObject.getUvCordinates();

	vis::Polygon* polygons = gameObject.getPolygons();
	auto = gameObject.getMaterialsIndexes();

		



	int polygonIndex = 0;

	int matIndex = 0;
	int faceIndex = 1;
	vis::Material mat;


}

void render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.2f, 0.4f, 0.6f, 1.0f);
	glLoadIdentity();

	drawActor(actor); 

	glutSwapBuffers();
}

void reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0, w / (double)h, 1.0, 200.0);
	
}

void update(int val) {
	glutPostRedisplay();
	glutTimerFunc(ms, update, 0);
}



void initGLUT(const char* nome, int argc, char** argv) {
	// Init GLUT
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow(nome);

	glutDisplayFunc(render);
	glutReshapeFunc(reshape);
	glutTimerFunc(ms, update, 0);

	//glutIgnoreKeyRepeat(1);
	//glutKeyboardFunc(normalKeysDown);
	//glutKeyboardUpFunc(normalKeysUp);

	//glutSpecialFunc(specialKeysDown);
	//glutSpecialUpFunc(specialKeysUp);

	//glutMouseFunc(mouseButton);
	//glutPassiveMotionFunc(mouseMove);
	//glutMotionFunc(pressedMouseMove);
}

//void drawObject()

int main(int argc, char** argv) {
	FILE* obj = nullptr;
	FILE* mtl = nullptr;

	fopen_s(&obj, "../res/porsche.obj", "r");
	fopen_s(&mtl, "../res/porsche.mtl", "r");

	vis::GameObject gameObject = vis::ObjectUtil::loadObjModel(obj, mtl);
	actor = vis::Actor(gameObject);

	initGLUT("BulletHell S/N", argc, argv);
	glutMainLoop();

	return 0;
}
