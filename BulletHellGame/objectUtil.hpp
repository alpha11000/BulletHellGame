#pragma once

#include <GL/glut.h>
#include <iostream>
#include <list>
#include <vector>
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
		GLfloat x, y, z;

		Vector3(GLfloat x = 0, GLfloat y = 0, GLfloat z = 0) 
			: x(x), y(y), z(z) {}
	};

	struct Polygon {
		int* verticesIndexes;
		int* uvPositionsIndexes;
		int normalIndex;

		Polygon(int* verticesIndexes = nullptr, int uvPositionsIndexes[] = nullptr, int normalIndex = -1)
			: verticesIndexes(verticesIndexes),
			uvPositionsIndexes(uvPositionsIndexes),
			normalIndex(normalIndex) {}
	};

	class GameObject {
	private:
		Vector3* vertices;
		Vector3* normals;
		Vector3* uvCoordinates;

		Polygon* polygons;

		std::vector<std::pair<int, Material>> materialsIndexes;

	public:
		GameObject(Vector3* vertices = nullptr,
			Vector3* normals = nullptr,
			Vector3* uvCoordinates = nullptr,
			Polygon* polygons = nullptr,
			std::vector<std::pair<int, Material>> materialsIndexes = std::vector< std::pair<int, vis::Material> >(1, std::make_pair(0, vis::Material()))) :
			vertices(vertices),
			normals(normals),
			uvCoordinates(uvCoordinates),
			polygons(polygons),
			materialsIndexes(materialsIndexes) {}

		Vector3* getVertices() { return vertices; }
		Vector3* getNormals() { return normals; }
		Vector3* getUvCordinates() { return uvCoordinates; }
		Polygon* getPolygons() { return polygons; }
		std::vector<std::pair<int, Material>> getMaterialsIndexes() { return materialsIndexes; }
	};

	class ObjectUtil {
	public:
		static Vector3 readVector3FromFile(FILE* file) {
			Vector3 v3;
			fscanf_s(file, " %f %f %f", &v3.x, &v3.y, &v3.z);

			return v3;
		}

		static GameObject loadObjModel(FILE* obj, FILE* mtl) {
			std::map<std::string, Material> materials;
			std::vector<std::pair<int, Material>> materialsIndexes;

			auto err_mat = Material();
			materials[err_mat.name] = err_mat;

			std::list<Vector3> vertices, uvCordinates, normals;
			std::list<Polygon> polygons;

			if (mtl != NULL) {
				char materialName[32];
				
				while (true) {
					char lec[64];
					int s = fscanf_s(mtl, "%s", lec, 64);
					
					if (s == EOF) break;

					if (strcmp(lec, "newmtl") == 0) {
						fscanf_s(mtl, "%s", &materialName, 32);
					}

					if (strcmp(lec,"Kd") == 0) {
						float r, g, b;

						int k = fscanf_s(mtl, "%f %f %f", &r, &g, &b);
						Material material = Material(materialName, r, g, b, 1);
						materials[materialName] = material;
					}

					if (strcmp(lec,"d") == 0) {
						float a;

						int k = fscanf_s(mtl, "%f", &a);

						if(materials.count((std::string)materialName))
							materials[(std::string)materialName].a = a;
					}
				}
			}

			int facesCount = 0;

			if (obj != NULL) {
				while (true) {
					char lec[32];
					int s = fscanf_s(obj, "%s", lec, 32);

					if (s == EOF) break;

					if (strcmp(lec, "#") == 0) {
						char buffer[32];
						fgets(buffer, 32, obj);
						std::cout << buffer << std::endl;
					}

					else if (strcmp(lec, "v") == 0) {
						vertices.push_back(
							readVector3FromFile(obj)
						);
					}

					else if (strcmp(lec, "vt") == 0) {
						Vector3 v = Vector3();
						fscanf_s(obj, " %f %f", &v.x, &v.y);
						uvCordinates.push_back(v);
					}

					else if (strcmp(lec, "vn") == 0) {
						normals.push_back(
							readVector3FromFile(obj)
						);
					}

					else if (strcmp(lec, "f") == 0) {
						facesCount++;
						std::list<int> verts, uvs;

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
						
						int* vi = new int[verts.size()];
						int* uvi = new int[uvs.size()];

						std::copy(verts.begin(), verts.end(), vi);
						std::copy(uvs.begin(), uvs.end(), uvi);

						Polygon p = Polygon(vi, uvi, n);
						polygons.push_back(p);
					}
					else if (strcmp(lec, "usemtl") == 0) {
						char name[32];
						fscanf_s(obj, "%s", name, 32);

						Material mat = err_mat;

						if (materials.count(name))
							mat = materials[name];
						
						
						materialsIndexes.push_back(std::make_pair(facesCount, mat));
					}
				}
			}
			auto verticesArray = new Vector3[vertices.size()],
				uvCordinatesArray = new Vector3[uvCordinates.size()],
				normalsArray = new Vector3[normals.size()];

			auto polygonsArray = new Polygon[polygons.size()];

			std::copy(vertices.begin(), vertices.end(), verticesArray);
			std::copy(uvCordinates.begin(), uvCordinates.end(), uvCordinatesArray);
			std::copy(normals.begin(), normals.end(), normalsArray);
			std::copy(polygons.begin(), polygons.end(), polygonsArray);


			for (auto mm : materialsIndexes) {
				std::cout << mm.first << "  " << mm.second.name << std::endl;
			}

			GameObject gameObject = GameObject(verticesArray, normalsArray, uvCordinatesArray, polygonsArray, materialsIndexes);

			return gameObject;
		}
	};
}
