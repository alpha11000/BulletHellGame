#pragma once

#include "../Renderer/renderer.hpp"
#include "actor.hpp"
#include <map>

class Logic {
private:
	int fps, ms;
	int lvl, lvls, enemies;
	int num;

	std::map<int, lgc::Actor> actors;
	
	Logic();
	Logic(const Logic&) = delete;
	Logic& operator=(const Logic&) = delete;
	Logic(Logic&&) = delete;
	Logic& operator=(Logic&&) = delete;

public:
	
	inline auto getActors() {
		return actors;
	}

	void update(int val);

	static Logic& getInstance();
};

void updateCB(int val);