#include "AssetsManager.hpp"

vis::AssetsManager::AssetsManager() {
	resFolder = "../res/";
	loadModels();
}

void vis::AssetsManager::loadModels() {
	loadModel("player", PLAYER);
	loadModel("props/floor", FLOOR);
	
	loadModel("props/tree1", ENVIRONMENT);
	loadModel("props/pin", ENVIRONMENT);

	loadModel("props/house1", ENVIRONMENT);
	loadModel("props/house2", ENVIRONMENT);
	loadModel("props/house3", ENVIRONMENT);
	loadModel("props/house4", ENVIRONMENT);

	//fixing x, and importing fixed x objects
	enviromentFixedXIndex = enviroment.size();
	loadModel("props/street", ENVIRONMENT);

	loadModel("thunder", ENEMY);
	loadModel("xWings", ENEMY);
	loadModel("faxx", ENEMY);

	loadModel("bullet1", BULLET);
}

void vis::AssetsManager::loadModel(std::string baseName, ModelType modelType) {
	std::string objName = resFolder + baseName + ".obj";

	FILE* objFile = nullptr;
	fopen_s(&objFile, objName.c_str(), "r");

	GameObject obj = loadObjModel(objFile);

	std::vector<MTL> mtls = loadMtls(baseName);

	if (mtls.empty()) {
		MTL mtl = MTL();
		mtls.push_back(mtl);
	}

	switch (modelType)
	{
	case PLAYER:
		player = std::make_pair(obj, mtls[0]);
		break;
	case FLOOR:
		floor = std::make_pair(obj, mtls[0]);
		break;
	case ENEMY:
		enemies.push_back(std::make_pair(obj, mtls));
		break;
	case BOOST:
		boosts.push_back(std::make_pair(obj, mtls));
		break;
	case ENVIRONMENT:
		enviroment.push_back(std::make_pair(obj, mtls));
		break;
	case BULLET:
		bullets.push_back(std::make_pair(obj, mtls));
		break;
	default:
		break;
	}
}

std::vector<vis::MTL> vis::AssetsManager::loadMtls(std::string baseName) {
	std::vector<MTL> mtls;
	FILE* mtlFile = nullptr;

	for (int i = 0;; i++) {
		std::string mtlName = resFolder + baseName + ((i == 0) ? "" : std::to_string(i)) + ".mtl";
		fopen_s(&mtlFile, mtlName.c_str(), "r");

		if (mtlFile == NULL) break;

		MTL mtl = loadMtl(mtlFile);
		mtls.push_back(mtl);
	}

	return mtls;
}

vis::AssetsManager& vis::AssetsManager::getInstance() {
	static AssetsManager instance;
	return instance;
}

math::Vector3 vis::AssetsManager::readVector3FromFile(FILE* file) {
	math::Vector3 v3;
	fscanf_s(file, " %f %f %f", &v3.v[0], &v3.v[1], &v3.v[2]);

	return v3;
}

vis::MTL vis::AssetsManager::loadMtl(FILE* mtl) {
	std::map<std::string, Material> materials;

	if (mtl != NULL) {
		char materialName[32];

		while (true) {
			char lec[64];
			int s = fscanf_s(mtl, "%s", lec, 64);

			if (s == EOF) break;

			if (strcmp(lec, "newmtl") == 0) {
				fscanf_s(mtl, "%s", &materialName, 32);
			}

			if (strcmp(lec, "Kd") == 0) {
				float r, g, b;

				int k = fscanf_s(mtl, "%f %f %f", &r, &g, &b);
				Material material = Material(materialName, r, g, b, 1);
				materials[materialName] = material;
			}

			if (strcmp(lec, "Ks") == 0) {
				float r, g, b;

				int k = fscanf_s(mtl, "%f %f %f", &r, &g, &b);

				if (materials.count((std::string)materialName))
					materials[(std::string)materialName].setSpecular(r, g, b);
			}

			if (strcmp(lec, "d") == 0) {
				float a;

				int k = fscanf_s(mtl, "%f", &a);

				if (materials.count((std::string)materialName))
					materials[(std::string)materialName].difuse[3] = a;
			}

			if (strcmp(lec, "Ns") == 0) {
				float b;

				int k = fscanf_s(mtl, "%f", &b);

				if (materials.count((std::string)materialName))
					materials[(std::string)materialName].brightness = b;
			}
		}
	}

	return MTL(materials);
}

vis::GameObject vis::AssetsManager::loadObjModel(FILE* obj) {
	std::vector<std::pair<int, std::string>> materialsIndexes;

	std::vector<math::Vector3> vertices, uvCordinates, normals;
	std::vector<Polygon> polygons;

	int facesCount = 0;

	if (obj != NULL) {
		while (true) {
			char lec[32];
			int s = fscanf_s(obj, "%s", lec, 32);

			if (s == EOF) break;

			if (strcmp(lec, "#") == 0) {
				char buffer[32];
				fgets(buffer, 32, obj);
			}

			else if (strcmp(lec, "v") == 0) {
				vertices.push_back(
					readVector3FromFile(obj)
				);
			}

			else if (strcmp(lec, "vt") == 0) {
				math::Vector3 v = math::Vector3();
				fscanf_s(obj, " %f %f", &v.v[0], &v.v[1]);
				uvCordinates.push_back(v);
			}

			else if (strcmp(lec, "vn") == 0) {
				normals.push_back(
					readVector3FromFile(obj)
				);
			}

			else if (strcmp(lec, "f") == 0) {
				facesCount++;
				std::vector<int> verts, uvs;

				int v = 0,
					uv = 0,
					n = 0;

				while (true) {
					int s = fscanf_s(obj, "%d/", &v);

					if (s != 1) break;

					s = fscanf_s(obj, "%d", &uv);
					s = fscanf_s(obj, "/%d", &n);

					verts.push_back(v);
					uvs.push_back(uv);
				}

				Polygon p = Polygon(verts, uvs, n);
				polygons.push_back(p);
			}
			else if (strcmp(lec, "usemtl") == 0) {
				char name[32];
				fscanf_s(obj, "%s", name, 32);

				materialsIndexes.push_back(std::make_pair(facesCount, name));
			}
		}
	}

	GameObject gameObject = GameObject(vertices, normals, uvCordinates, polygons, materialsIndexes);

	return gameObject;
}