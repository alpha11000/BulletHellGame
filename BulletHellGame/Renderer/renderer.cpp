#include "renderer.hpp"
#include "../Logic/logic.hpp"

Renderer::Renderer() {
	W = 800, H = 600, zmax = 80;
	fov = 90, camy = 15;
	fps = 60, ms = 1000 / fps;

	_debugFlag = false;
	
	vis::AssetsManager::getInstance();

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(W, H);
	glutCreateWindow("Bulletto");

	glutDisplayFunc(renderCB);
	glutReshapeFunc(reshapeCB);

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

	glutTimerFunc(1, requestRenderCB, 0);
}

static void setColor(vis::Material material) {
	glColor4fv(material.difuse);

	// Define a refletância do material
	glMaterialfv(GL_FRONT, GL_SPECULAR, material.specular);
	// Define a concentração do brilho
	glMateriali(GL_FRONT, GL_SHININESS, material.brightness);
}

void Renderer::drawActor(lgc::Actor* actor) {
	vis::GameObject& gameObject = actor->getGameObject();

	math::Vector3
		pos = actor->getPosition(),
		rot = actor->getRotation();

	std::vector<math::Vector3>
		&vertices = gameObject.getVertices(),
		&normals = gameObject.getNormals(),
		&uvCordinates = gameObject.getUvCordinates();

	auto& materials = actor->getMaterials();
	vis::Material errorMaterial = vis::Material();

	std::vector<vis::Polygon>& polygons = gameObject.getPolygons();
	auto& materialIndexes = gameObject.getMaterialsIndexes();

	int polIndex = 0;
	vis::Material mat = vis::Material();

	int mi = 0;
	int nextMatIndex = 0;

	std::string matName;

	if (materialIndexes.size() > 0) {
		matName = materialIndexes[0].second;

		mat = materials.count(matName) ? 
			materials[matName] : 
			errorMaterial;

		if (materialIndexes.size() > 1)
			nextMatIndex = materialIndexes[1].first;
		else
			nextMatIndex = polygons.size();
	}

	glPushMatrix();
	glTranslatef(pos.v[0], pos.v[1], pos.v[2]);
	glRotatef(rot.v[0], 1, 0, 0);
	glRotatef(rot.v[1], 0, 1, 0);
	glRotatef(rot.v[2], 0, 0, 1);

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
			math::Vector3 normal = normals[p.normalIndex - 1];
			glNormal3f(normal.v[0], normal.v[1], normal.v[2]);
		}

		for (int i : p.verticesIndexes) {
			glVertex3fv(vertices[i - 1].v);
		}
		glEnd();
	}
	glPopMatrix();
}

void Renderer::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.2f, 0.4f, 0.6f, 1.0f);
	glLoadIdentity();

	glColor3f(1, 1, 1);
	
	drawActor(&Logic::getInstance().getPlayer());
	if (_debugFlag)
		Logic::getInstance().getPlayer()._renderHitbox();

	drawActor(&Logic::getInstance().getFloor());

	for (auto& a : Logic::getInstance().getEnemies()) {
		if (a.second->isRemoveable()) continue;
		drawActor(a.second);
		if (_debugFlag)
			a.second->_renderHitbox();
	}

	for (auto& b : Logic::getInstance().getBullets()) {
		if (b.second->isRemoveable()) continue;
		drawActor(b.second);
		if (_debugFlag)
			b.second->_renderHitbox();
	}

	for (auto& e : Logic::getInstance().getEnviroment()) {
		if (e.second.isRemoveable()) continue;
		drawActor(&e.second);
	}

	//auto scoretxt = Logic::getInstance().getScore();

	//unsigned char _scoretxt[50];
	//std::copy(scoretxt.begin(), scoretxt.end(), _scoretxt);
	//_scoretxt[scoretxt.length()] = 0;

	//Renderer::getInstance().writeToScreen(math::Vector3(20, 20, 0), _scoretxt);

	glutSwapBuffers();
}

void Renderer::reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float raio = std::sqrt((zmax * zmax) + (camy * camy));
	float a = (std::acos(zmax / raio) * 180 / math::pi);
	float total_angle = camy < 0 ? a + fov / 2 : -(a + fov / 2);

	gluPerspective(fov, w / (double)h, 1.0, 1000);

	glRotatef(0, 0.0, 0.0, 1.0);			// x
	glRotatef(180, 0.0, 1.0, 0.0);			// y
	glRotatef(total_angle, 1.0, 0.0, 0.0);	// z
	glTranslated(0, -camy, 0);

	W = w; H = h;

	glMatrixMode(GL_MODELVIEW);
}

void Renderer::requestRender() {
	glutPostRedisplay();
	glutTimerFunc(ms, requestRenderCB, 0);
}

Renderer& Renderer::getInstance() {
	static Renderer instance;
	return instance;
}

void Renderer::switchDebug() {
	_debugFlag = !_debugFlag;
}

void Renderer::writeToScreen(math::Vector3 position, const unsigned char* string, void* font, math::Vector3 rgb) {
	char* c;

	glColor3f(rgb[0], rgb[1], rgb[2]);
	glRasterPos2f(position[0], position[1]);

	const unsigned char test[] = "texto";
	glutBitmapString(font, test);
}

void renderCB() { Renderer::getInstance().render(); }
void reshapeCB(int w, int h) { Renderer::getInstance().reshape(w, h); }
void requestRenderCB(int val) { Renderer::getInstance().requestRender(); }