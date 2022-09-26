#pragma once

#include <string>
#include <vector>

#include "model.hpp"

namespace vis {
	class AssetsManager {
	private:
		enum ModelType { PLAYER, ENEMY, BOOST, ENVIROMENT, BULLET};

		std::string resFolder;
		std::pair<GameObject, MTL> player;
		std::vector<std::pair<GameObject, std::vector<MTL>>> enemies;
		std::vector<std::pair<GameObject, std::vector<MTL>>> boosts;
		std::vector<std::pair<GameObject, std::vector<MTL>>> enviroment;
		std::vector<std::pair<GameObject, std::vector<MTL>>> bullets;

		static math::Vector3 readVector3FromFile(FILE* file);
		static MTL loadMtl(FILE* mtl);
		static GameObject loadObjModel(FILE* obj);

		void loadModels();
		void loadModel(std::string baseName, ModelType modelType);
		std::vector<vis::MTL> loadMtls(std::string baseName);

		AssetsManager();
		AssetsManager(const AssetsManager&) = delete;
		AssetsManager& operator=(const AssetsManager&) = delete;
		AssetsManager(AssetsManager&&) = delete;
		AssetsManager& operator=(AssetsManager&&) = delete;


	public:
		inline std::vector<std::pair<GameObject, std::vector<MTL>>> getEnemies() { return enemies; }

		inline auto getPlayerModel() { return player; }

		inline auto getEnemyModel(int index) { return enemies[index]; }
		inline int getEnemiesCount() { return enemies.size(); }
		inline int getEnemyMaterialCount(int index) { return enemies[index].second.size(); }

		static AssetsManager& getInstance();
	};
}