#pragma once

namespace ctrl {
	class Actor {
	private:
		bool actived = true;

		vis::GameObject gameObject;
		std::map<std::string, vis::Material> materials;
		vis::Vector3 pos, rot;
	public:
		Actor(vis::GameObject gameObject = vis::GameObject(), vis::Vector3 position = vis::Vector3(0, 0, 0), vis::Vector3 rotation = vis::Vector3(0, 0, 0)) :
			gameObject(gameObject),
			pos(position),
			rot(rotation) {}

		void setMatrials(vis::MTL materials) {
			this->materials = materials.mtlMaterials;
		}

		void onUpdate() {
			translate(0, 0, -5);
			if (pos.v[2] < -100) {
				actived = false;
			}
		}

		void draw() {
			//render.drawObject(this);
		}

		bool isActived() {
			return actived;
		}

		void translate(float tX, float tY, float tZ) {
			pos.v[0] += tX;
			pos.v[1] += tY;
			pos.v[2] += tZ;
		}

		void rotate(float rX, float rY, float rZ) {
			rot.v[0] += rX;
			rot.v[1] += rY;
			rot.v[2] += rZ;
		}

		vis::GameObject getGameObject() {return gameObject;}
		std::map<std::string, vis::Material> getMaterials() { return materials; }
		vis::Vector3 getPosition() { return pos; }
		vis::Vector3 getRotation() { return rot; }
	};
}