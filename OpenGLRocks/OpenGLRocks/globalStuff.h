#pragma once

#include "cVAOManager/cVAOManager.h"
#include "cMesh.h"
#include <vector>

// This is shared stuff that ISN'T OpenGL specific,
//	but is for our entire (or most of it) program.

// C and C++ tips
// You header should only have:
//	- funtion signatures
//  - class headers
//  - variables that have "extern" in front of them
// (and templates)
// 
// NO CODE
// NO VARIABLES 


void LoadTheModels(cVAOManager* pVAOManager, unsigned int shaderProgramID);

// Anything in this vector will be drawn in the main loop
// (defined in theMain.cpp)
extern std::vector< cMesh* > g_vec_pModelsToDraw;
