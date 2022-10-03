#pragma once
#include <vector>
#include <memory>

#include "actor.hpp"
#include "../Util/math.hpp"

namespace lgc {
	class rbAABB {
		math::Vector3 nBounds;
		math::Vector3 bounds;

	public:
		static bool testCollision(Collidable& c1, Collidable& c2);

		inline rbAABB(float dx, float dy, float dz) :
			bounds(math::Vector3(std::abs(dx), std::abs(dy), std::abs(dz))) {
			nBounds = math::Vector3(-bounds[0], -bounds[1], -bounds[2]);
		}

		inline math::Vector3 getMinDisp() {
			return nBounds;
		}

		inline math::Vector3 getMaxDisp() {
			return bounds;
		}
	};

	class collisionSolver {
	private:
		static rbAABB ship[3];
		static rbAABB player;
		static rbAABB bullet;

		std::vector< Ship* > enemies;
		std::vector< Bullet* > allyBullets;
		std::vector< Bullet* > enemyBullets;
		
		int axis = 2;

		void sortActorLists();
	public:
		collisionSolver();

		void runCollisions();
		void insertCollidable(Ship* s);
		void insertCollidable(Bullet* b, bool isAlly);

		inline static rbAABB* getPlayerHitbox() { return &player; }
		inline static rbAABB* getShipHitbox(int lvl) { return &ship[lvl]; }
		inline static rbAABB* getBulletHitbox() { return &bullet; }
		void clearRemoveables();

		void resetInstances();
	};
}

