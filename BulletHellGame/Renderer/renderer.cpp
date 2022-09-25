#include "renderer.hpp"
#include "../Logic/logic.hpp"

Renderer::Renderer() {
	W = 800; H = 600;
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
}

static void setColor(vis::Material material) {
	glColor4fv(material.difuse);

	// Define a refletância do material
	glMaterialfv(GL_FRONT, GL_SPECULAR, material.specular);
	// Define a concentração do brilho
	glMateriali(GL_FRONT, GL_SHININESS, material.brightness);
}

void Renderer::drawActor(lgc::Actor actor) {
	vis::GameObject gameObject = actor.getGameObject();

	math::Vector3
		pos = actor.getPosition(),
		rot = actor.getRotation();

	std::vector<math::Vector3>
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

	glPushMatrix();
	glColor3f(1, 1, 1);

	drawActor(Logic::getInstance().getPlayer());

	for (auto& a : Logic::getInstance().getActors()) {
		if (a.second.isRemoveable()) continue;
		drawActor(a.second);
	}
	glPushMatrix();

	glutSwapBuffers();
}

void Renderer::reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float fov = 90.f;
	float zs = 50.f;
	float camy = 20.f;

	float raio = std::sqrt((zs * zs) + (camy * camy));
	float a = (std::acos(zs / raio) * 180 / math::pi);

	float total_angle = camy < 0 ? a + fov / 2 : -(a + fov / 2);

	gluPerspective(fov, w / (double)h, 1.0, 1000);

	glRotatef(0, 0.0, 0.0, 1.0);			// x
	glRotatef(180, 0.0, 1.0, 0.0);			// y
	glRotatef(total_angle, 1.0, 0.0, 0.0);	// z
	glTranslated(0, -camy, 0);

	W = w; H = h;

	glMatrixMode(GL_MODELVIEW);
}

Renderer& Renderer::getInstance() {
	static Renderer instance;
	return instance;
}

void renderCB() { Renderer::getInstance().render(); }
void reshapeCB(int w, int h) { Renderer::getInstance().reshape(w, h); }