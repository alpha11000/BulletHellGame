#pragma once

//#include "objectUtil.hpp"

namespace ctrl {
	class Actor {
	private:
		vis::GameObject gameObject;
		vis::Vector3 pos, rot;
	public:
		Actor(vis::GameObject gameObject = vis::GameObject(), vis::Vector3 position = NULL, vis::Vector3 rotation = NULL) :
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

		vis::GameObject getGameObject() {return gameObject;}
		vis::Vector3 getPosition() { return pos; }
		vis::Vector3 getRotation() { return rot; }
	};
}