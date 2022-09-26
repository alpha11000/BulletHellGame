#include "logic.hpp"
#include "../Util/math.hpp"
#include "../Util/random.hpp"
#include "../Control/controller.hpp"
#include <set>

Logic::Logic() {
	fps = 30;
	ms = 1000 / fps;
	lvl = 0; lvls = 100;
	instanceID = 0;
	num = 0;
	
	auto playerModel = vis::AssetsManager::getInstance().getPlayerModel();
	player = lgc::Actor(playerModel.first, math::Vector3(), math::Vector3(0, 180, 0));
	player.setMaterials(playerModel.second);

	glutTimerFunc(1, updateCB, 0);
}

void Logic::addBullet(lgc::Bullet bullet) {
	bullets.insert(std::make_pair(bullets.size(), bullet));
}

void Logic::update(int val) {
	num++;

	if (num % 15 == 0 && vis::AssetsManager::getInstance().getEnemiesCount() > 0) {
		int r1 = lgc::RandomUtil::getRandomIndex(lvls, lvl, vis::AssetsManager::getInstance().getEnemiesCount());

		auto enemy = vis::AssetsManager::getInstance().getEnemyModel(r1);

		lgc::Ship act = lgc::Ship(enemy.first, math::Vector3(0, 0.2, 30), math::Vector3(), math::Vector3(), math::Vector3(1, 0, 0), math::Vector3(0, 0, -0.5));
		act.setAccelerating(true);
		int r2 = lgc::RandomUtil::getRandomIndex(lvls, lvl, enemy.second.size());

		act.setMaterials(enemy.second[r2]);
		
		enemies.insert(std::make_pair(instanceID, act));
		instanceID++;
		lvl++;
	}

	player.onUpdate();

	std::set<int> disableds;

	for (auto& kv : enemies) {
		kv.second.onUpdate();

		if (kv.second.isRemoveable()) disableds.insert(kv.first);
	}

	for (int i : disableds) {
		enemies.erase(i);
	}

	glutPostRedisplay();
	glutTimerFunc(ms, updateCB, 0);
}

void Logic::onKeysUpdate() {
	if (Controller::getInstance().specialIsPressed(GLUT_KEY_LEFT)) {
		//player.setAccelerating(true);
	}
	
	if (Controller::getInstance().specialIsPressed(GLUT_KEY_RIGHT)) {
		//player.setAccelerating(true);
	}
}

Logic& Logic::getInstance() {
	static Logic instance;
	return instance;
}

void updateCB(int val) {
	Logic::getInstance().update(val);
}