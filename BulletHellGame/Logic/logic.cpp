#include "logic.hpp"
#include "../Util/math.hpp"
#include "../Util/random.hpp"
#include "../Control/controller.hpp"
#include <cstdlib>
#include <set>

Logic::Logic() {
	tps = 100;
	ms = 1000 / tps;
	lvl = 0, lvls = 100;
	instanceID = 0;
	
	auto* playerModel = vis::AssetsManager::getInstance().getPlayerModel();
	auto* floorModel = vis::AssetsManager::getInstance().getFloorModel();
	auto* bulletModel = vis::AssetsManager::getInstance().getBulletModel(0);

	player = lgc::Ship();
	player
		.setGameObject(&(playerModel->first))
		.setMaterials(&(playerModel->second))
		.setRotation(0, 180, 0);
	player
		.setMaxVel(math::Vector3(0, 3, 0));
	player
		.setHitbox(CollisionSolver.getPlayerHitbox());
	player
		.setBulletGameObject(&(bulletModel->first))
		.setBulletMaterial(&(bulletModel->second[0]))
		.setBulletHitbox(CollisionSolver.getBulletHitbox())
		.setBulletVel(math::Vector3(0, 0, 0.4))
		.setBulletMaxVel(math::Vector3(0, 0, 0.3))
		.setBulletDamage(100)
		.setShootDelay(0.01, tps);
	player
		.setHP(100);

	floor = lgc::Actor(&(floorModel->first));
	floor.setMaterials(&floorModel->second);
	floor.setPosition(0, -4, 0);

	glutTimerFunc(1, updateCB, 0);
}

void Logic::addBullet(lgc::Bullet *bullet, bool isAlly) {
	bullets.insert(std::make_pair(instanceID++, bullet));
	CollisionSolver.insertCollidable(bullets[instanceID - 1], isAlly);
}

void Logic::update(int val) {
	static int enemNum = 0, envNum = 0;
	enemNum++;
	envNum++;

	if (enemNum > 80 && rand() % 25 == 0 && vis::AssetsManager::getInstance().getEnemiesCount() > 0) {
		enemNum = 0;

		int r1 = lgc::RandomUtil::getRandomIndex(lvls, lvl, vis::AssetsManager::getInstance().getEnemiesCount());

		auto* enemyModel = vis::AssetsManager::getInstance().getEnemyModel(r1);
		auto* bulletModel = vis::AssetsManager::getInstance().getBulletModel(0);

		lgc::Ship* enemy = new lgc::Ship();
		int r2 = lgc::RandomUtil::getRandomIndex(lvls, lvl, enemyModel->second.size());

		enemy
			->setGameObject(&(enemyModel->first))
			.setMaterials(&(enemyModel->second[r2]))
			.setRotation(0, 0, 0)
			.setPosition(-10, 0.2, Renderer::getInstance().zmax);
		enemy
			->setMaxVel(math::Vector3(0, 0.2, 0))
			.setAcceleration(math::Vector3(0, 0, -0.2))
			.setAccelerating(true);
		enemy
			->setHitbox(CollisionSolver.getShipHitbox());
		enemy
			->setBulletGameObject(&(bulletModel->first))
			.setBulletMaterial(&bulletModel->second[1])
			.setBulletHitbox(CollisionSolver.getBulletHitbox())
			.setBulletVel(math::Vector3(0, 0, -0.8))
			.setBulletMaxVel(math::Vector3(0, 0, 0.8))
			.setBulletDamage(100)
			.setShootDelay(0.5, tps)
			.setShooting(true)
			.setIsAlly(false);
		enemy
			->setHP(10);
		
		enemies.insert(std::make_pair(instanceID++, enemy));
		CollisionSolver.insertCollidable(enemy);
		lvl++;
	}

	if (envNum > 20 && rand() % 5 == 0 && vis::AssetsManager::getInstance().getEnviromentCount() > 0) {
		envNum = 0;

		int r1 = lgc::RandomUtil::getRandomIndex(10, 1, vis::AssetsManager::getInstance().getEnviromentCount());

		auto* env = vis::AssetsManager::getInstance().getEnviromentModel(r1);

		lgc::Moveable act = lgc::Moveable(&env->first);

		act.setAccelerating(true);
		int r2 = lgc::RandomUtil::getRandomIndex(lvls, lvl, env->second.size());

		float xPos = r1 < vis::AssetsManager::getInstance().getFixedEnviromentXIndex() ?
			lgc::RandomUtil::getRandom(xMin, xMax) :
			0;

		act.setAcceleration(math::Vector3(0, 0, -0.005))
			.setMaxVel(math::Vector3(0, 0, -0.05))
			.setMaterials(&env->second[r2])
			.setPosition(xPos, -4, Renderer::getInstance().zmax + 15);

		enviroment.insert(std::make_pair(instanceID++, act));
	}

	player.onUpdate();

	for (auto& kv : bullets)
		kv.second->onUpdate();

	for (auto& kv : enemies)
		kv.second->onUpdate();

	std::set<int> disableds;

	CollisionSolver.runCollisions();

	for (auto& kv : enemies)
		if (kv.second->isRemoveable()) {
			disableds.insert(kv.first);
			delete kv.second;
		}
	
	for (int i : disableds)
		enemies.erase(i);

	disableds.clear();

	for (auto& kv : bullets)
		if (kv.second->isRemoveable()) {
			disableds.insert(kv.first);
			delete kv.second;
		}

	for (int i : disableds)
		bullets.erase(i);

	disableds.clear();

	//enviroment update
	for (auto& kv : enviroment) {
		kv.second.onUpdate();

		if (kv.second.isRemoveable()) 
			disableds.insert(kv.first);
	}

	for (int i : disableds)
		enemies.erase(i);

	disableds.clear();

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