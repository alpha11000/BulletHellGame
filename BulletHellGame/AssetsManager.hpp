#pragma once

namespace vis {
	class AssetsManager {
	private:
		std::string resFolder;
		std::vector<std::pair<GameObject, std::vector<MTL>>> enemies;

		AssetsManager() {
			resFolder = "../res/";
			loadModels();
		}

		void loadModels() {
			loadModel("porsche");
			loadModel("av");
		}

		void loadModel(std::string baseName) {
			std::string objName = resFolder + baseName + ".obj";
			
			FILE* objFile = nullptr;
			fopen_s(&objFile, objName.c_str(), "r");

			GameObject obj = ObjectUtil::loadObjModel(objFile);

			std::vector<MTL> mtls = loadMtls(baseName);

			enemies.push_back(std::make_pair(obj, mtls));
		}

		std::vector<vis::MTL> loadMtls(std::string baseName) {
			std::vector<MTL> mtls;
			FILE* mtlFile = nullptr;

			for (int i = 0;; i++) {
				std::string mtlName = resFolder + baseName + ((i == 0) ? "" : std::to_string(i)) + ".mtl";
				fopen_s(&mtlFile, mtlName.c_str(), "r");
				
				if (mtlFile == NULL) break;

				MTL mtl = ObjectUtil::loadMtl(mtlFile);
				mtls.push_back(mtl);
			}

			return mtls;
		}

	public:
		std::vector<std::pair<GameObject, std::vector<MTL>>> getEnemies() {
			return enemies;
		}

		static AssetsManager& getInstance() {
			static AssetsManager instance = AssetsManager();
			return instance;
		}
	};
}