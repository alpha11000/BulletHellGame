#pragma once

#include <vector>
#include <GL/glut.h>
#include <iostream>
#include <list>
#include <string>
#include <map>

namespace vis {
	struct Material {
		std::string name;
		GLfloat r, g, b, a;

		Material(std::string name = "error", GLfloat r = 1, GLfloat g = 1, GLfloat b = 1, GLfloat a = 1)
			: name(name), r(r), g(g), b(b), a(a) {}
	};
	
	struct Vector3 {
		GLfloat v[3];

		Vector3(GLfloat x = 0, GLfloat y = 0, GLfloat z = 0) {
			v[0] = x;
			v[1] = y;
			v[2] = z;
		}

	};

	struct Polygon {
		std::vector<int> verticesIndexes;
		std::vector<int> uvPositionsIndexes;
		int normalIndex;

		Polygon(std::vector<int> verticesIndexes = std::vector<int>(), std::vector<int> uvPositionsIndexes = std::vector<int>(), int normalIndex = 0)
			: verticesIndexes(verticesIndexes),
			uvPositionsIndexes(uvPositionsIndexes),
			normalIndex(normalIndex) {}
	};

	class GameObject {
	private:
		std::vector<Vector3> vertices;
		std::vector<Vector3> normals;
		std::vector<Vector3> uvCoordinates;

		std::vector<Polygon> polygons;
		std::vector<std::pair<int, std::string>> materialsIndexes;

	public:
		GameObject(std::vector<Vector3> vertices = std::vector<Vector3>(),
			std::vector<Vector3> normals = std::vector<Vector3>(),
			std::vector<Vector3> uvCoordinates = std::vector<Vector3>(),
			std::vector<Polygon> polygons = std::vector<Polygon>(),
			std::vector<std::pair<int, std::string>> materialsIndexes = std::vector<std::pair<int, std::string> >(1, std::make_pair(0, std::string()))) :
			vertices(vertices),
			normals(normals),
			uvCoordinates(uvCoordinates),
			polygons(polygons),
			materialsIndexes(materialsIndexes) {}

		std::vector<Vector3> getVertices() { return vertices; }
		std::vector<Vector3> getNormals() { return normals; }
		std::vector<Vector3> getUvCordinates() { return uvCoordinates; }
		std::vector<Polygon> getPolygons() { return polygons; }
		std::vector<std::pair<int, std::string>> getMaterialsIndexes() { return materialsIndexes; }
	};

	class ObjectUtil {
	public:
		static Vector3 readVector3FromFile(FILE* file) {
			Vector3 v3;
			fscanf_s(file, " %f %f %f", &v3.v[0], &v3.v[1], &v3.v[2]);

			return v3;
		}

		static std::map<std::string, Material> loadMtl(FILE* mtl) {
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

					if (strcmp(lec, "d") == 0) {
						float a;

						int k = fscanf_s(mtl, "%f", &a);

						if (materials.count((std::string)materialName))
							materials[(std::string)materialName].a = a;
					}
				}
			}

			return materials;
		}

		static GameObject loadObjModel(FILE* obj) {

			std::vector<std::pair<int, std::string>> materialsIndexes;

			std::vector<Vector3> vertices, uvCordinates, normals;
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
						Vector3 v = Vector3();
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
	};
}
