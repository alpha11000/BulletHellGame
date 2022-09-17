#pragma once
#ifndef MAIN
#define	MAIN

#include "objectUtil.hpp"
#include "gameObject.hpp"

int main(int argc, char** argv) {
	FILE* obj;
	FILE* mtl = NULL;

	fopen_s(&obj, "../res/porsche.obj", "r");
	fopen_s(&mtl, "../res/porsche.mtl", "r");

	visualization::ObjectUtil::loadObjModel(obj, mtl); 
	 
	std::cout << "Aqui";

}

#endif