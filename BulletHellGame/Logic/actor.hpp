#pragma once

#include <map>
#include "../Util/math.hpp"
#include "../Renderer/model.hpp"

namespace lgc {
	class Actor {
	private:
		bool removeable;

		vis::GameObject gameObject;
		std::map<std::string, vis::Material> materials;
		math::Vector3 pos, rot;
	public:
		Actor(
			vis::GameObject gameObject = vis::GameObject(), math::Vector3 position = math::Vector3(0, 0, 0),
			math::Vector3 rotation = math::Vector3(0, 0, 0));

		void setMatrials(vis::MTL materials);
		void onUpdate();
		
		void draw();

		inline bool isRemoveable() {
			return removeable;
		}

		inline void translate(float tX, float tY, float tZ) {
			pos.v[0] += tX;
			pos.v[1] += tY;
			pos.v[2] += tZ;
		}

		inline void rotate(float rX, float rY, float rZ) {
			rot.v[0] += rX;
			rot.v[1] += rY;
			rot.v[2] += rZ;
		}

		inline vis::GameObject getGameObject() {return gameObject;}
		
		inline std::map<std::string, vis::Material> getMaterials() { return materials; }
		
		inline math::Vector3 getPosition() { return pos; }
		
		inline math::Vector3 getRotation() { return rot; }
	};

	class Moveable : Actor {

	};

}