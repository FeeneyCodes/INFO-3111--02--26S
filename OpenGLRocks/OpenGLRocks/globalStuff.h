#pragma once
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

#include "cVAOManager/cVAOManager.h"
#include "cMesh.h"
#include <vector>
#include "cBasicFlyCamera/cBasicFlyCamera.h"
#include "cBasicTextureManager/cBasicTextureManager.h"
#include "cLightManager.h"



void LoadTheModels(cVAOManager* pVAOManager, unsigned int shaderProgramID);

// Anything in this vector will be drawn in the main loop
// (defined in theMain.cpp)
extern std::vector< cMesh* > g_vec_pModelsToDraw;

// Notice this is a pointer. (is in theMain.cpp)
extern cBasicFlyCamera* g_pFlyCamera;

extern cLightManager* g_pLightManager;

extern cBasicTextureManager* g_pTextureManager;

// The light we are modifying
extern int g_selectedLightID; // = 0;

// This will scan through the vector of meshes
// Returns NULL if not found
cMesh* g_pFindMeshByFriendlyName(std::string theNameToFind);

