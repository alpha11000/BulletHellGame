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
	score = 0;

	glutTimerFunc(1, genScenarioCB, 0);
	glutTimerFunc(1, updateCB, 0);
}

void Logic::update(int val) {
	static int enemNum = 0;
	enemNum++;

	if (enemNum > 80 && rand() % 25 == 0 && vis::AssetsManager::getInstance().getEnemiesCount() > 0) {
		enemNum = 0;

		int r1 = lgc::RandomUtil::getRandomIndex(lvls, lvl, 3);
		int r2 = lgc::RandomUtil::getRandomIndex(lvls, lvl, 3);

		InstanceManager.addInstance(InstanceManager.createEnemy(r1, r2));

		lvl++;
		lvl = lvl % 100;
	}

	InstanceManager.updateInstances();
	CollisionSolver.runCollisions();
	InstanceManager.clearRemoveables();

	glutTimerFunc(ms, updateCB, 0);
}

void Logic::onKeysUpdate() {
	InstanceManager.player.setAccelerating(false);
	InstanceManager.player.setAcceleration(math::Vector3());
	// Adicionar tecla pra ver as caixas de colisão
	if (Controller::getInstance().specialIsPressed(GLUT_KEY_LEFT)) {
		InstanceManager.player.changeAcceleration(math::Vector3(0.1, 0, 0));
		InstanceManager.player.setAccelerating(true);
	}

	if (Controller::getInstance().specialIsPressed(GLUT_KEY_RIGHT)) {
		InstanceManager.player.changeAcceleration(math::Vector3(-0.1, 0, 0));
		InstanceManager.player.setAccelerating(true);
	}

	if (Controller::getInstance().isPressed(' '))
		InstanceManager.player.setShooting(true);
	else
		InstanceManager.player.setShooting(false);
}

Logic& Logic::getInstance() {
	static Logic instance;
	return instance;
}

void Logic::generateScenario() {
	if (rand() % 5) {
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

		InstanceManager.enviroment.insert(std::make_pair(InstanceManager.instanceID++, act));

		glutTimerFunc(1000 / 1.5, genScenarioCB, 0);
		return;
	}
	glutTimerFunc(1000 / (tps * 2), genScenarioCB, 0);
}

void updateCB(int val) {
	Logic::getInstance().update(val);
}

void genScenarioCB(int val) {
	Logic::getInstance().generateScenario();
}