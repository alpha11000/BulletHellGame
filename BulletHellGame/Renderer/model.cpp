#include "model.hpp"

vis::Material::Material(std::string name, float r, float g, float b, float a) {
	difuse[0] = r;
	difuse[1] = g;
	difuse[2] = b;
	difuse[3] = a;
}

void vis::Material::setSpecular(float r, float g, float b) {
	specular[0] = r;
	specular[1] = g;
	specular[2] = b;
}

vis::MTL::MTL(std::map<std::string, Material> mtlMaterials) {
	this->mtlMaterials = mtlMaterials;
}

vis::Polygon::Polygon(std::vector<int> verticesIndexes, std::vector<int> uvPositionsIndexes, int normalIndex)
	: verticesIndexes(verticesIndexes),
	uvPositionsIndexes(uvPositionsIndexes),
	normalIndex(normalIndex) {}

vis::GameObject::GameObject(
	std::vector<math::Vector3> vertices,
	std::vector<math::Vector3> normals,
	std::vector<math::Vector3> uvCoordinates,
	std::vector<Polygon> polygons,
	std::vector<std::pair<int, std::string>> materialsIndexes) :
	vertices(vertices),
	normals(normals),
	uvCoordinates(uvCoordinates),
	polygons(polygons),
	materialsIndexes(materialsIndexes) {}