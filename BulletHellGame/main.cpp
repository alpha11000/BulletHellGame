#include "Header.h"
#include "objectUtil.hpp"

int main(int argc, char** argv) {
	FILE* obj;
	FILE* mtl = NULL;

	fopen_s(&obj, "../res/porsche.obj", "r");
	fopen_s(&mtl, "../res/porsche.mtl", "r");

	vis::ObjectUtil::loadObjModel(obj, mtl); 


}
