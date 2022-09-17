#pragma once

#include "objectUtil.hpp"

namespace vis {
	class Actor {
	private:
		GameObject gameObject;
		float tX, tY, tZ,
			rX, rY, rZ;
	public:
		Actor(GameObject gameObject = NULL, float tX = 0, float tY = 0, float tZ = 0, float rX = 0, float rY = 0, float rZ = 0):
			gameObject(gameObject),
			tX(tX),
			tY(tY),
			tZ(tZ),
			rX(rX),
			rY(rY),
			rZ(rZ) {}

		void draw() {
			//render.draw(this);
		}

		void translate(float tX, float tY, float tZ) {
			this->tX += tX;
			this->tY += tY;
			this->tZ += tZ;
		}
	};
}