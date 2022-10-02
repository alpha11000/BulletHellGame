#include "collisionSolver.hpp"
#include <algorithm>
#include "logic.hpp"

bool lgc::rbAABB::testCollision(Collidable& c1, Collidable& c2) {
 auto c1min = c1.getMin(), c1max = c1.getMax();
 auto c2min = c2.getMin(), c2max = c2.getMax();

 return(
	 c1min[0] <= c2max[0] &&
	 c2min[0] <= c1max[0] &&
	 c1min[1] <= c2max[1] &&
	 c2min[1] <= c1max[1] &&
	 c1min[2] <= c2max[2] &&
	 c2min[2] <= c1max[2]
	 );
}


lgc::rbAABB lgc::collisionSolver::ship = lgc::rbAABB(1.0, 1.0, 1.0);
lgc::rbAABB lgc::collisionSolver::bullet = lgc::rbAABB(0.3, 0.3, 0.6);
lgc::rbAABB lgc::collisionSolver::player = lgc::rbAABB(2.f, 1.3f, 2.f);

lgc::collisionSolver::collisionSolver() {}

void lgc::collisionSolver::sortActorLists() {
	std::sort(enemies.begin(), enemies.end(), [&](auto& s1, auto& s2) {
		return s1->getMin()[axis] < s2->getMin()[axis];
	});

	std::sort(allyBullets.begin(), allyBullets.end(), [&](auto& b1, auto& b2) {
		return b1->getMin()[axis] > b2->getMin()[axis];
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
		while (nextZMin < enemies.size() && enemies[nextZMin]->getMin()[axis] > allyBullets[i]->getMax()[axis]) {
			nextZMin++;
		}

		for (unsigned int j = nextZMin; j < enemies.size(); j++) {
			if (enemies[j]->getMax()[axis] < allyBullets[i]->getMin()[axis] ||
				enemies[j]->getMin()[axis] > allyBullets[i]->getMax()[axis]) break;
			
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
		if (enemyBullets[i]->getMin()[axis] > player.getMax()[axis] ||
			enemyBullets[i]->getMax()[axis] < player.getMin()[axis]) continue;

		if (rbAABB::testCollision(*enemyBullets[i], player)) {
			player.onCollide(*enemyBullets[i]);
			enemyBullets[i]->onCollide(player);
		}
	}

	// player x enemies
	for (unsigned int i = 0; i < enemies.size(); i++) {
		if (enemies[i]->getMin()[axis] > player.getMax()[axis] ) continue;

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
			i--;
		}
	}
	
	for (unsigned int i = 0; i < allyBullets.size(); i++) {
		if (allyBullets[i]->isRemoveable()) {
			allyBullets[i] = allyBullets.back();
			allyBullets.pop_back();
			i--;
		}
	}
	
	for (unsigned int i = 0; i < enemyBullets.size(); i++) {
		if (enemyBullets[i]->isRemoveable()) {
			enemyBullets[i] = enemyBullets.back();
			enemyBullets.pop_back();
			i--;
		}
	}
}