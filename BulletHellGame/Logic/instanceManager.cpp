#include "logic.hpp"
#include "../Util/random.hpp"
#include <list>

Logic::instanceManager::instanceManager() {
	auto* playerModel = vis::AssetsManager::getInstance().getPlayerModel();
	auto* floorModel = vis::AssetsManager::getInstance().getFloorModel();
	auto* bulletModel = vis::AssetsManager::getInstance().getBulletModel(0);

	tps = 100;

	player = lgc::Ship();
	player
		.setGameObject(&(playerModel->first))
		.setMaterials(&(playerModel->second))
		.setRotation(0, 180, 0);
	player
		.setMaxVel(math::Vector3(0, 2.5, 0));
	player
		.setHitbox(lgc::collisionSolver::getPlayerHitbox());
	player
		.setBulletGameObject(&(bulletModel->first))
		.setBulletMaterial(&(bulletModel->second[0]))
		.setBulletHitbox(lgc::collisionSolver::getBulletHitbox())
		.setBulletVel(math::Vector3(0, 0, 0.4))
		.setBulletMaxVel(math::Vector3(0, 0, 0.3))
		.setBulletDamage(1)
		.setShootDelay(0.2, tps);
	player
		.setHP(10)
		.setScore(0);

	floor = lgc::Actor(&(floorModel->first));
	floor.setMaterials(&floorModel->second);
	floor.setPosition(0, -4, 0);
}

lgc::Ship* Logic::instanceManager::createEnemy(int r1, int r2) {
	auto* enemyModel = vis::AssetsManager::getInstance().getEnemyModel(r1);
	auto* bulletModel = vis::AssetsManager::getInstance().getBulletModel(0);

	lgc::Ship* enemy = new lgc::Ship();

	enemy
		->setGameObject(&(enemyModel->first))
		.setMaterials(&(enemyModel->second[r2]))
		.setRotation(0, 0, 0)
		.setPosition(lgc::RandomUtil::getRandom(-25, 25), 0.2, Renderer::getInstance().zmax);
	enemy
		->setMaxVel(math::Vector3(0, 0.2, 0))
		.setAcceleration(math::Vector3(0, 0, -0.2))
		.setAccelerating(true);
	enemy
		->setHitbox(lgc::collisionSolver::getShipHitbox(r1));
	enemy
		->setBulletGameObject(&(bulletModel->first))
		.setBulletMaterial(&bulletModel->second[1])
		.setBulletHitbox(lgc::collisionSolver::getBulletHitbox())
		.setBulletVel(math::Vector3(0, 0, -0.8))
		.setBulletMaxVel(math::Vector3(0, 0, 0.8))
		.setBulletDamage(1)
		.setShootDelay(1 - (r1 + r2) / 10, tps)
		.setShooting(true)
		.setIsAlly(false);
	enemy
		->setHP(1 + (r1 + r2))
		.setScore(100);

	return enemy;
}

void Logic::instanceManager::addInstance(lgc::Ship* s) {
	enemies.insert(std::make_pair(instanceID++, s));
	Logic::getInstance().CollisionSolver.insertCollidable(s);
}

void Logic::instanceManager::addInstance(lgc::Bullet* b, bool isAlly) {
	bullets.insert(std::make_pair(instanceID++, b));
	Logic::getInstance().CollisionSolver.insertCollidable(b, isAlly);
}

void Logic::instanceManager::updateInstances() {
	player.onUpdate();

	for (auto& kv : bullets)
		kv.second->onUpdate();

	for (auto& kv : enemies)
		kv.second->onUpdate();

	for (auto& kv : enviroment)
		kv.second.onUpdate();
}

void Logic::instanceManager::clearRemoveables() {
	std::list<int> removeables;

	for (auto& kv : enemies)
		if (kv.second->isRemoveable()) {
			removeables.push_back(kv.first);
			delete kv.second;
		}

	for (int i : removeables)
		enemies.erase(i);

	removeables.clear();

	for (auto& kv : bullets)
		if (kv.second->isRemoveable()) {
			removeables.push_back(kv.first);
			delete kv.second;
		}

	for (int i : removeables)
		bullets.erase(i);

	removeables.clear();

	for (auto& kv : enviroment) {
		if (kv.second.isRemoveable())
			removeables.push_back(kv.first);
	}

	for (int i : removeables)
		enemies.erase(i);

	removeables.clear();
}

void Logic::instanceManager::resetInstances() {
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
		.setHitbox(lgc::collisionSolver::getPlayerHitbox());
	player
		.setBulletGameObject(&(bulletModel->first))
		.setBulletMaterial(&(bulletModel->second[0]))
		.setBulletHitbox(lgc::collisionSolver::getBulletHitbox())
		.setBulletVel(math::Vector3(0, 0, 0.4))
		.setBulletMaxVel(math::Vector3(0, 0, 0.3))
		.setBulletDamage(1)
		.setShootDelay(0.2, tps);
	player
		.setHP(10)
		.setScore(0);

	enemies.clear();
	enviroment.clear();
	bullets.clear();
}