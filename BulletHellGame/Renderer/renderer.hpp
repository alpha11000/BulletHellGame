#pragma once
#include <GL/freeglut.h>

#include "model.hpp"
#include "AssetsManager.hpp"
#include "../Logic/actor.hpp"
#include "../Util/math.hpp"

class Renderer {
public:
	int W, H, zmax, fps, ms;
	float fov, camy;
	vis::GameObject error_GO = vis::GameObject();
	std::map<std::string, vis::Material> error_mats = std::map<std::string, vis::Material>();

private:
	bool _debugFlag;

	Renderer();
	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;
	Renderer(Renderer&&) = delete;
	Renderer& operator=(Renderer&&) = delete;

public:
	void render();
	void reshape(int w, int h);

	void drawActor(lgc::Actor* actor);
	static Renderer& getInstance();
	void requestRender();
	void switchDebug();

	//void renderHP();
	void writeToScreen(math::Vector3 position, const unsigned char* string, void* font = GLUT_BITMAP_TIMES_ROMAN_24, math::Vector3 rgb = math::Vector3());

};

void renderCB();
void reshapeCB(int w, int h);
void requestRenderCB(int val);