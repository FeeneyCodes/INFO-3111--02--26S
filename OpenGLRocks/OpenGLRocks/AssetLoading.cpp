
#include "cVAOManager/cVAOManager.h"
#include <iostream>

void LoadTheModels(cVAOManager* pVAOManager, unsigned int shaderProgramID)
{
    pVAOManager->setBasePath("assets/models");

    sModelDrawInfo migModelInfo;
    if (pVAOManager->LoadModelIntoVAO("mig29_xyz_n.ply",
        migModelInfo,
        shaderProgramID, true, false, false))
    {
        std::cout << "Loaded " << migModelInfo.meshName << std::endl;
        std::cout << "\t" << migModelInfo.numberOfVertices << " vertices" << std::endl;
        std::cout << "\t" << migModelInfo.numberOfTriangles << " triangles" << std::endl;
    }

    // Load 
    sModelDrawInfo shuttleModelInfo;
    pVAOManager->LoadModelIntoVAO("SpaceShuttleOrbiter_xyz_n.ply", 
        shuttleModelInfo, shaderProgramID, true, false, false);

    sModelDrawInfo bunnyModelInfo;
    pVAOManager->LoadModelIntoVAO("bun_zipper_XYZ_N_rgba_uv.ply", 
        bunnyModelInfo, shaderProgramID, true, true, true);

    sModelDrawInfo terrainModelInfo;
    pVAOManager->LoadModelIntoVAO("Terrain_xyz_n.ply", 
        terrainModelInfo, shaderProgramID, true, false, false);

    // The dungeon models from project #1
    pVAOManager->setBasePath("assets/models/Dungeon_models/Dead bodies, etc");


    sModelDrawInfo deadDrawf01ModelInfo;
    pVAOManager->LoadModelIntoVAO("SM_Prop_DeadBody_Dwarf_01.ply", 
        deadDrawf01ModelInfo, shaderProgramID, true, true, true); // <---

    return;
}