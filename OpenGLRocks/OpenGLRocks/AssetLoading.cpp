
#include "cVAOManager/cVAOManager.h"
#include <iostream>

void LoadTheModels(cVAOManager* pVAOManager, unsigned int shaderProgramID)
{
    pVAOManager->setBasePath("assets/models");

    sModelDrawInfo migModelInfo;
    //if (pVAOManager->LoadModelIntoVAO("mig29_xyz_n.ply",
    if (pVAOManager->LoadModelIntoVAO("mig29_xyz_n_rgba_uv.ply",
        migModelInfo,
        shaderProgramID, true, false, false))
    {
        std::cout << "Loaded " << migModelInfo.meshName << std::endl;
        std::cout << "\t" << migModelInfo.numberOfVertices << " vertices" << std::endl;
        std::cout << "\t" << migModelInfo.numberOfTriangles << " triangles" << std::endl;
    }

    // Load 
    sModelDrawInfo shuttleModelInfo;
    //pVAOManager->LoadModelIntoVAO("SpaceShuttleOrbiter_xyz_n.ply", 
    pVAOManager->LoadModelIntoVAO("SpaceShuttleOrbiter_xyz_n_rgba_uv.ply", 
        shuttleModelInfo, shaderProgramID, true, true, true);

    sModelDrawInfo bunnyModelInfo;
    pVAOManager->LoadModelIntoVAO("bun_zipper_XYZ_N_rgba_uv.ply", 
        bunnyModelInfo, shaderProgramID, true, true, true);

    sModelDrawInfo terrainModelInfo;
    //pVAOManager->LoadModelIntoVAO("Terrain_xyz_n.ply", 
    pVAOManager->LoadModelIntoVAO("Terrain_xyz_n_rgba_uv.ply", 
        terrainModelInfo, shaderProgramID, true, true, true);

    // The dungeon models from project #1
    pVAOManager->setBasePath("assets/models/Dungeon_models/Dead bodies, etc");

    sModelDrawInfo deadDrawf01ModelInfo;
    pVAOManager->LoadModelIntoVAO("SM_Prop_DeadBody_Dwarf_01.ply", 
        deadDrawf01ModelInfo, shaderProgramID, true, true, true); // <---



    pVAOManager->setBasePath("assets/models");

    // Load the models for debugging lights
    sModelDrawInfo Isoshphere_flat_3divModelInfo;
    pVAOManager->LoadModelIntoVAO("Isoshphere_flat_3div_xyz_n_rgba_uv.ply", 
        Isoshphere_flat_3divModelInfo, shaderProgramID, true, true, true ); 

    sModelDrawInfo Isoshphere_flat_4divModelInfo;
    pVAOManager->LoadModelIntoVAO("Isoshphere_flat_4div_xyz_n_rgba_uv.ply", 
        Isoshphere_flat_4divModelInfo, shaderProgramID, true, true, true ); 

    sModelDrawInfo Isoshphere_smooth_inverted_normals_ModelInfo;
    pVAOManager->LoadModelIntoVAO("Isoshphere_smooth_inverted_normals_xyz_n_rgba_uv.ply", 
        Isoshphere_smooth_inverted_normals_ModelInfo, shaderProgramID, true, true, true );



    sModelDrawInfo WarehouseModelInfo;
    pVAOManager->LoadModelIntoVAO("Warehouse_xyz_n_rgba_UV (Blender Smart UV project).ply", 
        WarehouseModelInfo, shaderProgramID, true, true, true );


    return;
}