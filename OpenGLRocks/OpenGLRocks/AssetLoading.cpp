
#include "cVAOManager/cVAOManager.h"
#include <iostream>

void LoadTheModels(cVAOManager* pVAOManager, unsigned int shaderProgramID)
{
    pVAOManager->setBasePath("assets/models");

    sModelDrawInfo migModelInfo;
    if (pVAOManager->LoadModelIntoVAO("mig29.ply",
        migModelInfo,
        shaderProgramID))
    {
        std::cout << "Loaded " << migModelInfo.meshName << std::endl;
        std::cout << "\t" << migModelInfo.numberOfVertices << " vertices" << std::endl;
        std::cout << "\t" << migModelInfo.numberOfTriangles << " triangles" << std::endl;
    }

    // Load 
    sModelDrawInfo shuttleModelInfo;
    pVAOManager->LoadModelIntoVAO("SpaceShuttleOrbiter_xyz_n.ply", shuttleModelInfo, shaderProgramID);

    sModelDrawInfo bunnyModelInfo;
    pVAOManager->LoadModelIntoVAO("bun_zipper_XYZ_N.ply", bunnyModelInfo, shaderProgramID);

    return;
}