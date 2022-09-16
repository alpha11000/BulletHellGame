#include "objectUtil.hpp"

using namespace visualization;

int main(int argc, char** argv) {
	FILE* obj;
	FILE* mtl;

	fopen_s(&obj, "C:/Users/Cliente/source/repos/CG_23/porsche.obj", "r");
	fopen_s(&mtl, "C:/Users/Cliente/source/repos/CG_23/porsche.mtl", "r");

	visualization::ObjectUtil::loadObjModel(obj, mtl);

}