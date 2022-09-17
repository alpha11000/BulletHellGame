#pragma once
#ifndef MAIN
#define	MAIN

#include "objectUtil.hpp"
#include "gameObject.hpp"

int main(int argc, char** argv) {
	FILE* obj;
	FILE* mtl = NULL;

	fopen_s(&obj, "C:/Users/Cliente/source/repos/CG_23/porsche.obj", "r");
	fopen_s(&mtl, "C:/Users/Cliente/source/repos/CG_23/porsche.mtl", "r");

	visualization::ObjectUtil::loadObjModel(obj, mtl); 
	 
}

#endif