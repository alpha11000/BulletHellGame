#include "logic.hpp"
#include "../Util/math.hpp"
#include "../Util/random.hpp"
#include "../Control/controller.hpp"
#include <set>

Logic::Logic() {
	tps = 100;
	ms = 1000 / tps;
	lvl = 0, lvls = 100;
	instanceID = 0;
	num = 0;
	
	auto* playerModel = vis::AssetsManager::getInstance().getPlayerModel();
	auto* bulletModel = vis::AssetsManager::getInstance().getBulletModel(0);

	player = lgc::Ship();
	player
		.setGameObject(&(playerModel->first))
		.setMaterials(&(playerModel->second))
		.setRotation(0, 180, 0);
	player
		.setMaxVel(math::Vector3(0, 3, 0));
	player
		.setBulletGameObject(&(bulletModel->first))
		.setBulletVel(math::Vector3(0, 0, 0.4))
		.setBulletMaxVel(math::Vector3(0, 0, 0.3))
		.setBulletDamage(100)
		.setShootDelay(0.01, tps);
	player
		.setHP(100);

	glutTimerFunc(1, updateCB, 0);
}

void Logic::addBullet(lgc::Bullet bullet) {
	bullets.insert(std::make_pair(instanceID++, bullet));
}

void Logic::update(int val) {
	num++;

	if (num % 50 == 0 && vis::AssetsManager::getInstance().getEnemiesCount() > 0) {
		int r1 = lgc::RandomUtil::getRandomIndex(lvls, lvl, vis::AssetsManager::getInstance().getEnemiesCount());

		auto* enemy = vis::AssetsManager::getInstance().getEnemyModel(r1);

		lgc::Ship act = lgc::Ship(&enemy->first, math::Vector3(-10, 0.2, Renderer::getInstance().zmax), math::Vector3(), math::Vector3(), math::Vector3(1, 0, 0), math::Vector3(0, 0, -0.5));
		act.setAccelerating(true);
		int r2 = lgc::RandomUtil::getRandomIndex(lvls, lvl, enemy->second.size());

		act.setMaterials(&enemy->second[r2]);
		
		enemies.insert(std::make_pair(instanceID++, act));
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

	disableds.clear();

	for (auto& kv : bullets) {
		kv.second.onUpdate();

		if (kv.second.isRemoveable()) disableds.insert(kv.first);
	}

	for (int i : disableds) {
		bullets.erase(i);
	}

	glutTimerFunc(ms, updateCB, 0);
}

void Logic::onKeysUpdate() {
	player.setAccelerating(false);
	player.setAcceleration(math::Vector3());

	if (Controller::getInstance().specialIsPressed(GLUT_KEY_LEFT)) {
		player.changeAcceleration(math::Vector3(0.1, 0, 0));
		player.setAccelerating(true);
	}

	if (Controller::getInstance().specialIsPressed(GLUT_KEY_RIGHT)) {
		player.changeAcceleration(math::Vector3(-0.1, 0, 0));
		player.setAccelerating(true);
	}

	if (Controller::getInstance().isPressed(' '))
		player.setShooting(true);
	else
		player.setShooting(false);
}

Logic& Logic::getInstance() {
	static Logic instance;
	return instance;
}

void updateCB(int val) {
	Logic::getInstance().update(val);
}