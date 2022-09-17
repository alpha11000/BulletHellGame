#pragma once

#include "objectUtil.hpp"

namespace vis {
	class Actor {
	private:
		GameObject gameObject;
		Vector3 pos, rot;
	public:
		Actor(GameObject gameObject = NULL, Vector3 position = NULL, Vector3 rotation = NULL):
			gameObject(gameObject),
			pos(position),
			rot(rotation) {}

		void draw() {
			//render.drawObject(this);
		}

		void translate(float tX, float tY, float tZ) {
			pos.x += tX;
			pos.y += tY;
			pos.z += tZ;
		}

		GameObject getGameObject() {return gameObject;}
		Vector3 getPosition() { return pos; }
		Vector3 getRotation() { return rot; }
	};
}