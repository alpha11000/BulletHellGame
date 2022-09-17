#pragma once
#ifndef OBJUTIL
#define	OBJUTIL

#include <GL/glut.h>
#include <iostream>
#include <list>
#include <string>
#include <map>

#include "gameObject.hpp"

namespace visualization {

	struct Material {
		std::string name;
		GLfloat r = 1, g = 1, b = 1, a = 1;

		Material() {}

		Material(GLfloat cR, GLfloat cG, GLfloat cB, GLfloat cA) {
			r = cR;
			g = cG;
			b = cB;
			a = cA;
		}
	};
	
	struct Vector3 {
		GLfloat x = 0, y = 0, z = 0;

		visualization::Vector3() {}

		visualization::Vector3(GLfloat vX, GLfloat vY, GLfloat vZ) {
			x = vX;
			y = vY;
			z = vZ;
		}
	};

	struct Polygon {
		int* verticesIndexes;
		int* uvPositionsIndexes;
		int normalIndex;

		inline Polygon(int pVerticesIndexes[], int pUvPositionsIndexes[], int pNormalIndex) {
			verticesIndexes = pVerticesIndexes;
			uvPositionsIndexes = pUvPositionsIndexes;
			normalIndex = pNormalIndex;
		}
	};

	class ObjectUtil {
	public:
		static Vector3 readVector3FromFile(FILE* file) {
			Vector3* v3 = new Vector3();
			fscanf_s(file, " %f %f %f", &v3->x, &v3->y, &v3->z);

			return *v3;
		}

		static GameObject loadObjModel(FILE* obj, FILE* mtl) {

			std::map<std::string, Material> materials;
			std::list<std::pair<int, Material>> materialsIndexes;

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
						Material* material = new Material(r, g, b, 1);
						material->name = materialName;
						materials[materialName] = *material;
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
						Vector3* v = new Vector3();
						fscanf_s(obj, " %f %f", &v->x, &v->y);
						uvCordinates.push_back(*v);
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

						Polygon* p = new Polygon(vi, uvi, n);
						polygons.push_back(*p);
					}
					else if (strcmp(lec, "usemtl") == 0) {

						char name[32];
						fscanf_s(obj, "%s", name, 32);

						Material* mat = new Material(1, 1, 1, 1);

						if (materials.count((std::string)name)) {
							*mat = materials[(std::string)name];
						}

						mat->name = name;
						auto pr = new std::pair<int, Material>;
						pr->first = facesCount;
						pr->second = *mat;

						materialsIndexes.push_back(*pr);
					}
				}
			}

			auto materialsIndexesArray = new std::pair<int, Material>[materialsIndexes.size()];
			std::copy(materialsIndexes.begin(), materialsIndexes.end(), materialsIndexesArray);

			for (auto mm : materialsIndexes) {
				std::cout << mm.first << "  " << mm.second.name << std::endl;
			}

			GameObject go = GameObject();
			return go;
		}
	};
}

#endif
