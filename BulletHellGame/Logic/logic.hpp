#pragma once

#include "../Renderer/renderer.hpp"
#include "actor.hpp"
#include <map>

class Logic {
private:
	int tps, ms;
	int lvl, lvls;
	unsigned long long instanceID;

	int xMin = -50, xMax = 50;

	lgc::Ship player;
	lgc::Actor floor;
	std::map<int, lgc::Moveable> enviroment;
	std::map<int, lgc::Ship> enemies;
	std::map<int, lgc::Bullet> bullets;
	
	Logic();
	Logic(const Logic&) = delete;
	Logic& operator=(const Logic&) = delete;
	Logic(Logic&&) = delete;
	Logic& operator=(Logic&&) = delete;

public:
	inline auto& getPlayer() { return player; }
	inline auto& getFloor() { return floor; }
	inline auto& getEnviroment() { return enviroment; }
	inline auto& getEnemies() { return enemies; }
	inline auto& getBullets() { return bullets; }

	void addBullet(lgc::Bullet bullet);

	void update(int val);
	void onKeysUpdate();

	static Logic& getInstance();
};

void updateCB(int val);