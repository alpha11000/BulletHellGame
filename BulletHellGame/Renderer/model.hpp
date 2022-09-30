#pragma once

#include <string>
#include <Vector>
#include <map>

#include "../Util/math.hpp"

namespace vis {
	struct Material {
		std::string name;
		float difuse[4];
		float specular[4];
		float brightness = 0;

		Material(std::string name = "error", float r = 1, float g = 1, float b = 1, float a = 1);

		void setSpecular(float r, float g, float b);
	};

	struct MTL {
		std::map<std::string, Material> mtlMaterials;

		MTL(std::map<std::string, Material> mtlMaterials = std::map<std::string, Material>());
	};

	struct Polygon {
		std::vector<int> verticesIndexes;
		std::vector<int> uvPositionsIndexes;
		int normalIndex;

		Polygon(std::vector<int> verticesIndexes = std::vector<int>(), std::vector<int> uvPositionsIndexes = std::vector<int>(), int normalIndex = 0);
	};

	class GameObject {
	private:
		std::vector<math::Vector3> vertices;
		std::vector<math::Vector3> normals;
		std::vector<math::Vector3> uvCoordinates;

		std::vector<Polygon> polygons;
		std::vector<std::pair<int, std::string>> materialsIndexes;

	public:
		GameObject(
			std::vector<math::Vector3> vertices = std::vector<math::Vector3>(),
			std::vector<math::Vector3> normals = std::vector<math::Vector3>(),
			std::vector<math::Vector3> uvCoordinates = std::vector<math::Vector3>(),
			std::vector<Polygon> polygons = std::vector<Polygon>(),
			std::vector<std::pair<int, std::string>> materialsIndexes = std::vector<std::pair<int, std::string> >(1, std::make_pair(0, std::string())));

		inline std::vector<math::Vector3>& getVertices() { return vertices; }
		inline std::vector<math::Vector3>& getNormals() { return normals; }
		inline std::vector<math::Vector3>& getUvCordinates() { return uvCoordinates; }
		inline std::vector<Polygon>& getPolygons() { return polygons; }
		inline std::vector<std::pair<int, std::string>>&
			getMaterialsIndexes() { return materialsIndexes; }
	};
}
