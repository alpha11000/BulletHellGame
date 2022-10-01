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
		rbAABB player;
		rbAABB ship;
		rbAABB bullet;

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

		inline rbAABB* getPlayerHitbox() { return &player; }
		inline rbAABB* getShipHitbox() { return &ship; }
		inline rbAABB* getBulletHitbox() { return &bullet; }
		void clearRemoveables();

	};
}

