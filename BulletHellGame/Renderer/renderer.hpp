#pragma once
#include <GL/freeglut.h>

#include "model.hpp"
#include "AssetsManager.hpp"
#include "../Logic/actor.hpp"
#include "../Util/math.hpp"

class Renderer {
public:
	int W, H, zmax, fps, ms;
private:
	Renderer();
	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;
	Renderer(Renderer&&) = delete;
	Renderer& operator=(Renderer&&) = delete;

public:
	void render();
	void reshape(int w, int h);

	void drawActor(lgc::Actor actor);
	static Renderer& getInstance();
	void requestRender();
};

void renderCB();
void reshapeCB(int w, int h);
void requestRenderCB(int val);