#include "logic.hpp"
#include "../Util/math.hpp"
#include "../Util/random.hpp"
#include "../Control/controller.hpp"
#include <set>

Logic::Logic() {
	fps = 30;
	ms = 1000 / fps;
	lvl = 0; lvls = 100;
	enemies = 5;
	num = 90;

	auto playerModel = vis::AssetsManager::getInstance().getPlayerModel();
	player = lgc::Actor(playerModel.first);
	player.setMatrials(playerModel.second);
	player.rotate(0, 180, 0);

	glutTimerFunc(1, updateCB, 0);
}

void Logic::update(int val) {
	num++;

	if (num % 15 == 0 && vis::AssetsManager::getInstance().getEnemiesCount() > 0) {
		int r1 = lgc::RandomUtil::getRandomIndex(lvls, lvl, vis::AssetsManager::getInstance().getEnemiesCount());

		auto enemie = vis::AssetsManager::getInstance().getEnemyModel(r1);

		lgc::Actor act = lgc::Actor(enemie.first);

		int r2 = lgc::RandomUtil::getRandomIndex(lvls, lvl, enemie.second.size());

		act.setMatrials(enemie.second[r2]);
		act.translate(0, 0.2, 75);

		actors.insert(std::make_pair(actors.size(), act));
		lvl++;
	}

	std::set<int> disableds;

	for (std::pair<int, lgc::Actor> kv : actors) {
		int k = kv.first;

		actors[k].onUpdate();

		if (actors[k].isRemoveable()) disableds.insert(k);
	}

	for (int i : disableds) {
		actors.erase(i);
	}

	glutPostRedisplay();
	glutTimerFunc(ms, updateCB, 0);
}

void Logic::onKeysUpdate() {
	if (Controller::getInstance().specialIsPressed(102)) {
		player.translate(5, 0, 0);
	}

	if (Controller::getInstance().specialIsPressed(100)) {
		player.translate(-5, 0, 0);
	}
}

Logic& Logic::getInstance() {
	static Logic instance;
	return instance;
}

lgc::Actor& Logic::getPlayer() {
	return player;
}

void updateCB(int val) {
	Logic::getInstance().update(val);
}