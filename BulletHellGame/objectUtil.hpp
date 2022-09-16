#pragma once

#include <string>

#include <GL/glut.h>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <iostream>
#include <string>
#include <map>

#include "object.hpp"

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

		inline Vector3(){}

		inline Vector3(GLfloat vX, GLfloat vY, GLfloat vZ) {
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
		static GameObject loadObjModel(FILE* obj, FILE* mtl) {

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

						materials[materialName].a = a;
					}
				}
			}

			for (auto m : materials) {
				Material mat = m.second;
				std::cout << m.first << "( " << mat.r << " " << mat.g << " " << mat.b << ")" << std::endl;
			}

			GameObject go = GameObject();
			return go;
		}
	};
}
