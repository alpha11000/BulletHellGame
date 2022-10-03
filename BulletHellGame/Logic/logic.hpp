#pragma once

#include "../Renderer/renderer.hpp"
#include "collisionSolver.hpp"
#include "actor.hpp"
#include <map>

class Logic {
private:
	int tps, ms;
	int lvl, lvls;
	int score;

	lgc::collisionSolver CollisionSolver;

	struct instanceManager {
	public:
		int tps;

		lgc::Ship player;
		lgc::Actor floor;
		std::map<unsigned int, lgc::Moveable> enviroment;
		std::map<unsigned int, lgc::Ship*> enemies;
		std::map<unsigned int, lgc::Bullet*> bullets;

		unsigned int instanceID;

		instanceManager();

		lgc::Ship* createEnemy(int r1, int r2);

		void addInstance(lgc::Ship* s);
		void addInstance(lgc::Bullet* b, bool isAlly = true);

		void updateInstances();
		void clearRemoveables();

		void resetInstances();
	};

	instanceManager InstanceManager;
	int xMin = -50, xMax = 50;

	Logic();
	Logic(const Logic&) = delete;
	Logic& operator=(const Logic&) = delete;
	Logic(Logic&&) = delete;
	Logic& operator=(Logic&&) = delete;
	
public:
	void generateScenario();

	inline auto& getPlayer() { return InstanceManager.player; }
	inline auto& getFloor() { return InstanceManager.floor; }
	inline auto& getEnviroment() { return InstanceManager.enviroment; }
	inline auto& getEnemies() { return InstanceManager.enemies; }
	inline auto& getBullets() { return InstanceManager.bullets; }
	inline auto getScore() { return score; }

	inline void addBullet(lgc::Bullet* b, bool isAlly = true) {
		InstanceManager.addInstance(b, isAlly);
	}

	inline void addScore(int incr) {
		score += incr;
		std::cout << "score: " << score << "\n";
	}

	void update(int val);
	void onKeysUpdate();
	void reset();

	static Logic& getInstance();
};

void updateCB(int val);
void genScenarioCB(int val);