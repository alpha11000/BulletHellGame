#include "collisionSolver.hpp"
#include <algorithm>
#include "logic.hpp"

bool lgc::rbAABB::testCollision(Collidable& c1, Collidable& c2) {
 auto c1min = c1.getMin(), c1max = c1.getMax();
 auto c2min = c2.getMin(), c2max = c2.getMax();

 if (c1min[0] <= c2max[0] &&
	 c2min[0] <= c1max[0] &&
	 c1min[1] <= c2max[1] &&
	 c2min[1] <= c1max[1] &&
	 c1min[2] <= c2max[2] &&
	 c2min[2] <= c1max[2]
	 ) return true;
 return false;
}


lgc::collisionSolver::collisionSolver() :
	ship(rbAABB(1.5, 1.5, 1.5)),
	bullet(rbAABB(0.3, 0.3, 0.6)) {}

void lgc::collisionSolver::sortActorLists() {
	std::sort(enemies.begin(), enemies.end(), [&](auto& s1, auto& s2) {
		return s1->getMin()[axis] < s2->getMin()[axis];
	});

	std::sort(allyBullets.begin(), allyBullets.end(), [&](auto& b1, auto& b2) {
		return b1->getMin()[axis] < b2->getMin()[axis];
	});

	std::sort(enemyBullets.begin(), enemyBullets.end(), [&](auto& b1, auto& b2) {
		return b1->getMin()[axis] < b2->getMin()[axis];
	});

}

void lgc::collisionSolver::runCollisions() {
	sortActorLists();

	// enemies x ally bullets
	int nextZMin = 0;
	for (unsigned int i = 0; i < allyBullets.size(); i++) {
		bool gotNextZMin = false;
		for (unsigned int s = i; s < enemies.size(); s++) {
			if (enemies[s]->getMin()[axis] < allyBullets[i]->getMin()[axis]) {
				nextZMin = s - 1;
				gotNextZMin = true;
				break;
			}
		}

		if (!gotNextZMin) break;
		for (unsigned int j = nextZMin; j < enemies.size(); j++) {
			if (enemies[j]->getMin()[axis] > allyBullets[i]->getMax()[axis]) break;
			
			if (rbAABB::testCollision(*allyBullets[i], *enemies[j])) {
				enemies[j]->onCollide(*allyBullets[i]);
				allyBullets[i]->onCollide(*enemies[j]);
			}
		}
	}

	// enemies x enemies
	for (unsigned int i = 0; i < enemies.size(); i++) {
		for (unsigned int j = i + 1; j < enemies.size(); j++) {
			if (enemies[j]->getMin()[axis] > enemies[i]->getMin()[axis]) break;
			
			if (rbAABB::testCollision(*enemies[i], *enemies[j])) {
				enemies[i]->onCollide(*enemies[j]);
				enemies[j]->onCollide(*enemies[i]);
			}
		}
	}

	// player x enemyBullets
	auto& player = Logic::getInstance().getPlayer();

	for (unsigned int i = 0; i < enemyBullets.size(); i++) {
		if (enemyBullets[i]->getMin()[axis] > player.getMin()[axis]) continue;

		if (rbAABB::testCollision(*enemyBullets[i], player)) {
			player.onCollide(*enemyBullets[i]);
			enemyBullets[i]->onCollide(player);
		}
	}

	// player x enemies
	for (unsigned int i = 0; i < enemies.size(); i++) {
		if (enemies[i]->getMin()[axis] > player.getMin()[axis]) continue;

		if (rbAABB::testCollision(*enemies[i], player)) {
			player.onCollide(*enemies[i]);
			enemies[i]->onCollide(player);
		}
	}

	clearRemoveables();
}

void lgc::collisionSolver::insertCollidable(Ship* s) {
	enemies.push_back(s);
}

void lgc::collisionSolver::insertCollidable(Bullet* b, bool isAlly) {
	if (isAlly) allyBullets.push_back(b);
	else	   enemyBullets.push_back(b);
}

void lgc::collisionSolver::clearRemoveables() {
	for (unsigned int i = 0; i < enemies.size(); i++) {
		if (enemies[i]->isRemoveable()) {
			enemies[i] = enemies.back();
			enemies.pop_back();
		}
	}
	
	for (unsigned int i = 0; i < allyBullets.size(); i++) {
		if (allyBullets[i]->isRemoveable()) {
			allyBullets[i] = allyBullets.back();
			allyBullets.pop_back();
		}
	}
	
	for (unsigned int i = 0; i < enemyBullets.size(); i++) {
		if (enemyBullets[i]->isRemoveable()) {
			enemyBullets[i] = enemyBullets.back();
			enemyBullets.pop_back();
		}
	}
}