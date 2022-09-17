#include "GL/freeglut.h"
#include "objectUtil.hpp"
#include "actor.hpp"

int ms = 1000 / 30;
ctrl::Actor actor;

void setColor(vis::Material material) {
	glColor4f(material.r, material.g, material.b, material.a);
}

void drawActor(ctrl::Actor actor) {

	vis::GameObject gameObject = actor.getGameObject();
	
	vis::Vector3
		pos = actor.getPosition(),
		rot = actor.getRotation();

	std::vector<vis::Vector3>
		vertices = gameObject.getVertices(),
		normals = gameObject.getNormals(),
		uvCordinates = gameObject.getUvCordinates();

	std::vector<vis::Polygon> polygons = gameObject.getPolygons();
	auto materialIndexes = gameObject.getMaterialsIndexes();

	int polIndex = 0;
	vis::Material mat = vis::Material();

	int mi = 0;
	int nextMatIndex = 0;

	if (materialIndexes.size() > 0) {
		mat = materialIndexes[0].second;
	}

	if (materialIndexes.size() >= 1) {
		nextMatIndex = materialIndexes[1].first;
	}

	glPushMatrix();
	glTranslatef(0, 0, -50);
	glRotatef(actor.getPosition().z, 0, 1, 0);

	setColor(mat);

	for (vis::Polygon p : polygons) {
		glBegin(GL_POLYGON);

		if (polIndex >= nextMatIndex) {
			if (mi < materialIndexes.size()) {
				mat = materialIndexes[mi++].second;
				setColor(mat);
			}

			if (mi < materialIndexes.size()) {
				nextMatIndex = materialIndexes[mi].first;
			}
		}

		polIndex++;

		if (normals.size() > p.normalIndex - 1) {
			vis::Vector3 normal = normals[p.normalIndex - 1];
			glNormal3f(normal.x, normal.y, normal.z);
		}

		for (int i : p.verticesIndexes) {
			glVertex3f(vertices[i - 1].x, vertices[i - 1].y, vertices[i - 1].z);
		}

		glEnd();
	}
	glPopMatrix();
}

void render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.2f, 0.4f, 0.6f, 1.0f);
	glLoadIdentity();

	glColor3f(1, 1, 1);
	drawActor(actor); 

	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslated(0, 0, 20);
	glutSolidCube(10);
	glPopMatrix();

	glutSwapBuffers();
}

void reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0, w / (double)h, 1.0, 200);
	gluLookAt(
		45, 45, 100,
		0, 0, 0,
		0, 1, 0
	);

	glMatrixMode(GL_MODELVIEW);
}

void update(int val) {
	actor.translate(0, 0, 4);
	glutPostRedisplay();
	glutTimerFunc(ms, update, 0);
}

void initLights() {
	GLfloat luzAmbiente[4] = { 0.2,0.2,0.2,1.0 };
	GLfloat luzDifusa[4] = { 0.7,0.7,0.7,0.7 };
	GLfloat luzEspecular[4] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat posicaoLuz[4] = { 20, 50, 80, 1.0 };

	// Capacidade de brilho do material
	GLfloat especularidade[4] = { 0.4,0.4,0.4,0.4 };
	GLint especMaterial = 5;

	// Habilita o modelo de tonalização de Gouraud
	glShadeModel(GL_SMOOTH);
	// glShadeModel(GL_FLAT);

	// Define a refletância do material
	glMaterialfv(GL_FRONT, GL_SPECULAR, especularidade);
	// Define a concentração do brilho
	glMateriali(GL_FRONT, GL_SHININESS, especMaterial);

	// Ativa o uso da luz ambiente
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);

	// Define os parâmetros da luz de número 0
	glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);
	glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular);
	glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz);

	// Habilita a definição da cor do material
	// a partir da cor corrente
	glEnable(GL_COLOR_MATERIAL);
	//Habilita o uso de iluminação
	glEnable(GL_LIGHTING);
	// Habilita a luz de número 0
	glEnable(GL_LIGHT0);
	// Habilita o depth-buffering
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
	initLights();
}

int main(int argc, char** argv) {
	FILE* obj = nullptr;
	FILE* mtl = nullptr;

	fopen_s(&obj, "../res/porsche.obj", "r");
	fopen_s(&mtl, "../res/porsche.mtl", "r");

	vis::GameObject gameObject = vis::ObjectUtil::loadObjModel(obj, mtl);
	actor = ctrl::Actor(gameObject);

	initGLUT("BulletHell S/N", argc, argv);
	glutMainLoop();

	return 0;
}
