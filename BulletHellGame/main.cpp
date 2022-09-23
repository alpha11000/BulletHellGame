#include <set>

#include "GL/freeglut.h"
#include "objectUtil.hpp"
#include "actor.hpp"
#include "AssetsManager.hpp"
#include "randomUtil.hpp"

int levels = 100;
int level = 0;
int enemies = 5;

int num = 0;/////

int ms = 1000 / 30;
ctrl::Actor actor1;

std::map<int, ctrl::Actor> actors;

std::vector<vis::MTL> mtlMaterials;
int index = 0;

void setColor(vis::Material material) {
	glColor4fv(material.difuse);

	// Define a refletância do material
	glMaterialfv(GL_FRONT, GL_SPECULAR, material.specular);
	// Define a concentração do brilho
	glMateriali(GL_FRONT, GL_SHININESS, material.brightness);
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

	auto materials = actor.getMaterials();
	vis::Material errorMaterial = vis::Material();

	std::vector<vis::Polygon> polygons = gameObject.getPolygons();
	auto materialIndexes = gameObject.getMaterialsIndexes();

	int polIndex = 0;
	vis::Material mat = vis::Material();

	int mi = 0;
	int nextMatIndex = 0;

	std::string matName;

	if (materialIndexes.size() > 0) {
		matName = materialIndexes[0].second;

		mat = ((materials.count(matName))) ? materials[matName] : errorMaterial;

		nextMatIndex = materialIndexes[1].first;
	}

	glPushMatrix();
	glRotatef(rot.v[1], 0, 1, 0);
	glTranslatef(pos.v[0], pos.v[1], pos.v[2]);

	setColor(mat);

	for (vis::Polygon p : polygons) {
		glBegin(GL_POLYGON);

		if (polIndex >= nextMatIndex) {
			if (mi < materialIndexes.size()) {
				std::string matName = materialIndexes[mi++].second;

				mat = ((materials.count(matName))) ? materials[matName] : errorMaterial;

				setColor(mat);
			}

			if (mi < materialIndexes.size()) {
				nextMatIndex = materialIndexes[mi].first;
			}
		}

		polIndex++;

		if (normals.size() > p.normalIndex - 1) {
			vis::Vector3 normal = normals[p.normalIndex - 1];
			glNormal3f(normal.v[0], normal.v[1], normal.v[2]);
		}

		for (int i : p.verticesIndexes) {
			glVertex3fv(vertices[i - 1].v);
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
	
	for (auto a : actors) {
		if (!a.second.isActived()) continue;
		drawActor(a.second);
	}

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
	num++;

	if (num % 15 == 0) {
		printf("entrering\n");
		int r1 = lgc::RandomUtil::getRandomIndex(levels, level, vis::AssetsManager::getInstance().getEnemiessCount());
		
		auto enemie = vis::AssetsManager::getInstance().getEnemie(r1);

		ctrl::Actor act = ctrl::Actor(enemie.first);

		int r2 = lgc::RandomUtil::getRandomIndex(levels, level, enemie.second.size());

		act.setMatrials(enemie.second[r2]);

		actors.insert(std::make_pair(actors.size(), act));
		level++;

		printf("ok\n");
	}

	std::set<int> disableds;

	for (std::pair<int, ctrl::Actor> kv : actors) {
		int k = kv.first;

		actors[k].onUpdate();

		if (!actors[k].isActived()) disableds.insert(k);
	}

	for (int i : disableds) {
		actors.erase(i);
	}

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
	//glShadeModel(GL_FLAT);

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

	num = 90;
	auto enemies = vis::AssetsManager::getInstance().getEnemies();

	actor1 = ctrl::Actor(enemies[1].first);
	mtlMaterials = enemies[1].second;
	actor1.setMatrials(enemies[1].second[0]);
	actor1.rotate(0, 0, 0);
	
	initGLUT("BulletHell S/N", argc, argv);
	glutMainLoop();

	return 0;
}
