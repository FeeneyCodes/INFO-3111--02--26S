//#define GLAD_GL_IMPLEMENTATION
//#include <glad/glad.h>
////#include "include/glad/glad.h"
//#define GLFW_INCLUDE_NONE
//#include <GLFW/glfw3.h>
#include "globalOpenGLStuff.h"

//#include "linmath.h"      // Another math library we aren't using
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp>
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string>
#include <vector>

#include <iostream>     // io stream
#include <fstream>      // file stream
#include <sstream>      // string stream

#include "cShaderManager/cShaderManager.h"
#include "cVAOManager/cVAOManager.h"

#include "globalStuff.h"

#include "cLightHelper/cLightHelper.h"

cShaderManager* g_pShaderManager = NULL;
cVAOManager* g_pVAOManager = NULL;

cBasicFlyCamera* g_pFlyCamera = NULL;

cLightManager* g_pLightManager = NULL;

bool g_bDrawDebugLightBalls = true;

//glm::vec3 eyePosition = glm::vec3(0.0f, 0.0f, -10.0f);
//glm::vec3 atPosition = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 upAxis = glm::vec3(0.0f, +1.0f, 0.0f);

// Anything in this vector will be drawn in the main loop
// (declared in globalStuff.h)
std::vector< cMesh* > g_vec_pModelsToDraw;

int g_selectedLightID = 0;

static void error_callback(int error, const char* description);

static void key_callback(GLFWwindow* window,
    int key,
    int scancode,
    int action,
    int mods);

// Set the callbacks for the mouse
// https://www.glfw.org/docs/3.3/input_guide.html#input_mouse
//
// Set with glfwSetCursorPosCallback(window, cursor_position_callback);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
//
// Set with glfwSetCursorEnterCallback(window, cursor_enter_callback);
void cursor_enter_callback(GLFWwindow* window, int entered);
//
// Set with glfwSetMouseButtonCallback(window, mouse_button_callback);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
//
// Set with glfwSetScrollCallback(window, scroll_callback);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// These are in the mouse_keyboard_async.cpp file
void handleMouseAsync(GLFWwindow* window);
void handleKeyboardAsync(GLFWwindow* window);

// In DrawMesh.cpp
void DrawMesh(cMesh* pCurrentMesh, GLuint program);


int main(void)
{

//    int myArray[100] = { 0 };   // STACK
//    int* pMyArray = new int[10000];   // HEAP
//    std::vector<int> myVector;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Core sometimes doesn't work on some cards (gives black screen)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1080, 760, "OpenGL Triangle", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);
    // Also the mouse callbacks
    glfwSetCursorPosCallback(window, cursor_position_callback);   
    glfwSetCursorEnterCallback(window, cursor_enter_callback);    
    glfwSetMouseButtonCallback(window, mouse_button_callback);    
    glfwSetScrollCallback(window, scroll_callback);


    glfwMakeContextCurrent(window);
    //gladLoadGL(glfwGetProcAddress);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);

    std::cout << glfwGetVersionString() << std::endl;
    GLint GLMajor, GLMinor;
    glGetIntegerv(GL_MAJOR_VERSION, &GLMajor);
    glGetIntegerv(GL_MINOR_VERSION, &GLMinor);
    std::cout << "OpenGL Version " << GLMajor << "." << GLMinor << std::endl;

    // cShaderManager* g_pShaderManager = NULL;
    ::g_pShaderManager = new cShaderManager();

    cShaderManager::cShader vertexShader;
    vertexShader.fileName = "simpleVertex.glsl";

    cShaderManager::cShader fragmentShader;
    fragmentShader.fileName = "simpleFragment.glsl";

    ::g_pShaderManager->setBasePath("assets//shaders//");

    if ( ! ::g_pShaderManager->createProgramFromFile( "BasicShader",
                                                      vertexShader,
                                                      fragmentShader))
    {
        std::cout << "Oh no! All is lost!." << std::endl;
        std::cout << ::g_pShaderManager->getLastError() << std::endl;
        return -1;
    }
    std::cout << "Shaders compiled OK." << std::endl;

    GLuint program = ::g_pShaderManager->getIDFromFriendlyName("BasicShader");

    glUseProgram(program);

    // The VAO manager (to load the models)
    ::g_pVAOManager = new cVAOManager();

    LoadTheModels(::g_pVAOManager, program);

    // Add some models to draw...
    cMesh* pDeadDwarf = new cMesh("SM_Prop_DeadBody_Dwarf_01.ply");
    //pDeadDwarf->bIsWireframe = true;
    pDeadDwarf->scale = 0.1f;
    pDeadDwarf->position.z = 50.0f;
    ::g_vec_pModelsToDraw.push_back(pDeadDwarf);

    //cMesh* pTerrain = new cMesh("Terrain_xyz_n.ply");
    cMesh* pTerrain = new cMesh("Terrain_xyz_n_rgba_uv.ply");
    pTerrain->diffuseRGB = glm::vec3(1.0f, 1.0f, 1.0f);
    pTerrain->specularPower = 10000.0f;
    //pTerrain->bIsWireframe = true;
    // This is set so that the models are in a "low" point on the terrain.
    // The terrain is also rotated to put a "low" point under the models.
    pTerrain->rotation.x = glm::radians<float>(-90.0f);
    pTerrain->rotation.y = glm::radians<float>(90.0f);
    pTerrain->position.y = -30.0f;
    pTerrain->position.z = 200.0f;
    pTerrain->scale = 2.0f;
    ::g_vec_pModelsToDraw.push_back(pTerrain);


    //cMesh* pMig = new cMesh("mig29.ply");
    cMesh* pMig = new cMesh("mig29_xyz_n_rgba_uv.ply");
    pMig->position.x = 5.0f;
    pMig->position.y = 0.1f;
    pMig->position.z = 0.7f;
    pMig->rotation.y = glm::radians<float>(90.0f);
    pMig->scale = 1.0f;
    pMig->diffuseRGB = glm::vec3(0.7f, 1.0f, 0.3f);

    //cMesh* pShuttle = new cMesh("SpaceShuttleOrbiter_xyz_n.ply");
    cMesh* pShuttle = new cMesh("SpaceShuttleOrbiter_xyz_n_rgba_uv.ply");
    pShuttle->scale = 1.0f / 200.0f;
    pShuttle->rotation.x = glm::radians<float>(-90.0f);
    pShuttle->diffuseRGB = glm::vec3(0.56f, 0.8f, 0.1f);
    pShuttle->position.y = -3.0f;
    //
    pShuttle->specularHighlightColourRGB = glm::vec3(1.0f, 1.0f, 1.0f);
    pShuttle->specularPower = 1000.0f;
    //pShuttle->bIsWireframe = true;

//    cMesh* pBunny = new cMesh("bun_zipper_XYZ_N.ply");
    cMesh* pBunny = new cMesh("bun_zipper_XYZ_N_rgba_uv.ply");
    pBunny->position.x = -3.0f;
    pBunny->scale = 2.0f;
    pBunny->diffuseRGB = glm::vec3(0.3f, 0.26f, 0.56f);

    //cMesh* pBunny2 = new cMesh("bun_zipper_XYZ_N.ply");
    cMesh* pBunny2 = new cMesh("bun_zipper_XYZ_N_rgba_uv.ply");
    pBunny2->position.x = -3.0f;
    pBunny2->position.y = 2.0f;
    pBunny2->scale = 2.5f;
    pBunny2->diffuseRGB = glm::vec3(0.6f, 0.1f, 0.7f);

    // Light debug objects
    //cMesh* pIsoshphere_flat_3div = new cMesh("Isoshphere_flat_3div_xyz_n_rgba_uv.ply");
    cMesh* pIsoshphere_flat_3div = new cMesh("Isoshphere_smooth_inverted_normals_xyz_n_rgba_uv.ply");
    pIsoshphere_flat_3div->diffuseRGB = glm::vec3(1.0f, 1.0f, 1.0f);
    pIsoshphere_flat_3div->position = glm::vec3(0.0f);
    pIsoshphere_flat_3div->bIsWireframe = true;
    pIsoshphere_flat_3div->bIsVisible = false; // **********
    pIsoshphere_flat_3div->friendlyName = "DebugSphere1";
    ::g_vec_pModelsToDraw.push_back(pIsoshphere_flat_3div);

    cMesh* pIsoshphere_flat_4div = new cMesh("Isoshphere_flat_4div_xyz_n_rgba_uv.ply");
    pIsoshphere_flat_4div->diffuseRGB = glm::vec3(1.0f, 1.0f, 1.0f);
    pIsoshphere_flat_4div->position = glm::vec3(0.0f);
    pIsoshphere_flat_4div->bIsVisible = false;  // *************
    pIsoshphere_flat_4div->friendlyName = "DebugSphere2";
    ::g_vec_pModelsToDraw.push_back(pIsoshphere_flat_4div);
       


    cMesh* pWarehouse = new cMesh("Warehouse_xyz_n_rgba_UV (Blender Smart UV project).ply");
    pWarehouse->diffuseRGB = glm::vec3(1.0f, 1.0f, 1.0f);
    pWarehouse->position = glm::vec3(0.0f);
    pWarehouse->friendlyName = "Warehouse";
    ::g_vec_pModelsToDraw.push_back(pWarehouse);





    ::g_vec_pModelsToDraw.push_back( pMig );
    ::g_vec_pModelsToDraw.push_back( pShuttle );
    ::g_vec_pModelsToDraw.push_back(pBunny);
    ::g_vec_pModelsToDraw.push_back(pBunny2);


    // Create the fly camera
    ::g_pFlyCamera = new cBasicFlyCamera();
    ::g_pFlyCamera->setEyeLocation(0.0f, 0.0f, -10.0f);


    // Create the light manager, too
    ::g_pLightManager = new cLightManager();

    ::g_pLightManager->SetUpUniformShaderLocations(program);

    //::g_pLightManager->myLights[0].bIsOn = true;
    //// Just above the origin
    //::g_pLightManager->myLights[0].position = glm::vec3(0.0f, 20.0f, 0.0f);
    //// Light is white coloured
    //::g_pLightManager->myLights[0].diffuseRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    //// 
    //::g_pLightManager->myLights[0].lightType = sLight::POINT_LIGHT;

    //::g_pLightManager->myLights[0].attenuationConstant = 0.0f;
    //::g_pLightManager->myLights[0].attenuationLinear = 0.01f;
    //::g_pLightManager->myLights[0].attenuationQuadratic = 0.001f;

        // Setting up spot light
    sLight* pSpotLight = &(::g_pLightManager->myLights[0]);
    pSpotLight->lightType = sLight::SPOT_LIGHT;
    pSpotLight->bIsOn = true;
    pSpotLight->position = glm::vec3(0.0f, 20.0f, 0.0f);
    pSpotLight->direction = glm::vec3(0.0f, -1.0f, 0.0f);
    // NEVER FORGET TO NORMALIZE DIRECTION
    pSpotLight->direction = glm::normalize(pSpotLight->direction);
    pSpotLight->diffuseRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    pSpotLight->attenuationConstant = 0.1f;
    pSpotLight->attenuationLinear = 0.01f;
    pSpotLight->attenuationQuadratic = 0.001f;

    // Cone angles
    pSpotLight->spotInnerAngle = 15.0f;	// Degrees
    pSpotLight->spotOuterAngle = 45.0f;

    sLight* pFlashLight = &(::g_pLightManager->myLights[1]);
    pFlashLight->lightType = sLight::SPOT_LIGHT;
    pFlashLight->bIsOn = true;


//    const GLint mvp_location = glGetUniformLocation(program, "MVP");
 
    while (!glfwWindowShouldClose(window))
    {
        pFlashLight->position = g_pFlyCamera->getEyeLocation();

        glm::vec3 cameraLookAt = g_pFlyCamera->getTargetLocation();
        glm::vec3 cameraDirection = cameraLookAt - g_pFlyCamera->getEyeLocation();
        pFlashLight->direction = glm::normalize(cameraDirection);

        pFlashLight->attenuationConstant = 0.1f;
        pFlashLight->attenuationLinear = 0.01f;
        pFlashLight->attenuationQuadratic = 0.001f;


        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        const float ratio = width / (float)height;

        glViewport(0, 0, width, height);
        // Clear the screen
        //glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        glEnable(GL_DEPTH_TEST);    // Turn on depth checking
        glDepthFunc(GL_LESS);       // The compare function

        // Proection and View (camera) only changes once per scene draw

        // the "camera"
        glm::mat4 matView 
            = glm::lookAt( ::g_pFlyCamera->getEyeLocation(),        // eyePosition, 
                           ::g_pFlyCamera->getTargetLocation(),     // atPosition, 
                           upAxis);

        // projection matrix // p
        glm::mat4 matProjection = glm::perspective(
            glm::radians(60.0f),        // FOV
            (float)width / (float)height, // Aspect ratio
            0.1f,            // Near plane
            10000.0f);         // Far plane

        // uniform mat4 mView;
        // uniform mat4 mProj;
        // 1. Get the uniform location
        // 2. Pass data to shader with glUniform
        GLint mView_UL = glGetUniformLocation(program, "mView");
        GLint mProj_UL = glGetUniformLocation(program, "mProj");

        glUniformMatrix4fv( mView_UL, 1, GL_FALSE, 
                            (const GLfloat*)&matView);

        glUniformMatrix4fv( mProj_UL, 1, GL_FALSE,
                            (const GLfloat*)&matProjection);



        // Copy light infor for this frame
        ::g_pLightManager->CopyLightInfoToShader(program);

        // Tell shader where the camera eye is (for specular)
        // uniform vec3 eyeLocation;
        GLint eyeLocation_UL = glGetUniformLocation(program, "eyeLocation");
        glm::vec3 theEyeLocation = ::g_pFlyCamera->getEyeLocation();
        glUniform3f(eyeLocation_UL,
                    theEyeLocation.x, theEyeLocation.y, theEyeLocation.z);



        for ( std::vector< cMesh* >::iterator it_pMesh = ::g_vec_pModelsToDraw.begin();
              it_pMesh != ::g_vec_pModelsToDraw.end();
              it_pMesh++)
        {
            // *******************************************************
            // STARTOF : Draw THIS model

            cMesh* pCurrentMesh = *it_pMesh;

            DrawMesh(pCurrentMesh, program );

            // ENDOF: Drawing THIS model
            // ************************************************
        }//for ( std::vector<cMesh*>::iterator it_pMesh

        
        if (::g_bDrawDebugLightBalls)
        {


            // Draw other stuff
            cMesh* pDebugBall = ::g_pFindMeshByFriendlyName("DebugSphere1");
            pDebugBall->bIsVisible = true;
            pDebugBall->bDoNotLight = true;

            // Draw a small white ball where the light is
            pDebugBall->position = ::g_pLightManager->myLights[::g_selectedLightID].position;
            pDebugBall->diffuseRGB = glm::vec3(1.0f, 1.0f, 1.0f);   // WHITE
            pDebugBall->scale = 0.1f;
            DrawMesh(pDebugBall, program);

            cLightHelper myLightHelper;
            // Draw a sphere at 75% brightness
            float distance75percent = myLightHelper.calcApproxDistFromAtten(
                0.75f,   // Target brighness
                0.01f,   // Accuracy
                100000.0f,   // Infinitely far from the light
                ::g_pLightManager->myLights[::g_selectedLightID].attenuationConstant,
                ::g_pLightManager->myLights[::g_selectedLightID].attenuationLinear,
                ::g_pLightManager->myLights[::g_selectedLightID].attenuationQuadratic);
            // 
            pDebugBall->scale = distance75percent;
            pDebugBall->diffuseRGB = glm::vec3(1.0f, 1.0f, 0.0f);   // YELLOW
            DrawMesh(pDebugBall, program);



            // at 50% brightness
            float distance50percent = myLightHelper.calcApproxDistFromAtten(
                0.50f,   // Target brighness
                0.01f,   // Accuracy
                100000.0f,   // Infinitely far from the light
                ::g_pLightManager->myLights[::g_selectedLightID].attenuationConstant,
                ::g_pLightManager->myLights[::g_selectedLightID].attenuationLinear,
                ::g_pLightManager->myLights[::g_selectedLightID].attenuationQuadratic);
            // 
            pDebugBall->scale = distance50percent;
            pDebugBall->diffuseRGB = glm::vec3(1.0f, 0.0f, 0.0f);   // RED
            DrawMesh(pDebugBall, program);

            // at 25% brightness
            float distance25percent = myLightHelper.calcApproxDistFromAtten(
                0.25f,   // Target brighness
                0.01f,   // Accuracy
                100000.0f,   // Infinitely far from the light
                ::g_pLightManager->myLights[::g_selectedLightID].attenuationConstant,
                ::g_pLightManager->myLights[::g_selectedLightID].attenuationLinear,
                ::g_pLightManager->myLights[::g_selectedLightID].attenuationQuadratic);
            // 
            pDebugBall->scale = distance25percent;
            pDebugBall->diffuseRGB = glm::vec3(0.0f, 1.0f, 0.0f);   // GREEN
            DrawMesh(pDebugBall, program);

            // at 10% brightness
            float distance10percent = myLightHelper.calcApproxDistFromAtten(
                0.10f,   // Target brighness
                0.01f,   // Accuracy
                100000.0f,   // Infinitely far from the light
                ::g_pLightManager->myLights[::g_selectedLightID].attenuationConstant,
                ::g_pLightManager->myLights[::g_selectedLightID].attenuationLinear,
                ::g_pLightManager->myLights[::g_selectedLightID].attenuationQuadratic);
            // 
            pDebugBall->scale = distance10percent;
            pDebugBall->diffuseRGB = glm::vec3(0.0f, 1.0f, 1.0f);   // CYAN (light blue)
            DrawMesh(pDebugBall, program);


            // at 2% brightness (basically where it stops lighting, black)
            float distance2percent = myLightHelper.calcApproxDistFromAtten(
                0.02f,   // Target brighness
                0.01f,   // Accuracy
                100000.0f,   // Infinitely far from the light
                ::g_pLightManager->myLights[::g_selectedLightID].attenuationConstant,
                ::g_pLightManager->myLights[::g_selectedLightID].attenuationLinear,
                ::g_pLightManager->myLights[::g_selectedLightID].attenuationQuadratic);
            // 
            pDebugBall->scale = distance2percent;
            pDebugBall->diffuseRGB = glm::vec3(0.0f, 0.2f, 1.0f);   // Purple-BLUE (Pure BLUE is hard to see)
            DrawMesh(pDebugBall, program);



            pDebugBall->bIsVisible = false;
        }

		
        // Print out the camera's location
        std::stringstream ssWindowText;
        // Add this:
        glm::vec3 eyePosition = ::g_pFlyCamera->getEyeLocation();

        ssWindowText << "Camera (xyz): "
            << eyePosition.x << ", "
            << eyePosition.y << ", "
            << eyePosition.z;

        // Light info as well
        ssWindowText << " Light[" << ::g_selectedLightID << "]: "
            << ::g_pLightManager->myLights[::g_selectedLightID].position.x << ", "
            << ::g_pLightManager->myLights[::g_selectedLightID].position.y << ", "
            << ::g_pLightManager->myLights[::g_selectedLightID].position.z << "  "
            << "lin: " << ::g_pLightManager->myLights[::g_selectedLightID].attenuationLinear << " "
            << "quad: " << ::g_pLightManager->myLights[::g_selectedLightID].attenuationQuadratic;


        glfwSetWindowTitle( window, ssWindowText.str().c_str() );

        // Everything is drawn so "present" the "back buffer"

        glfwSwapBuffers(window);
        // This is GLFWs thing that updates the mouse and keyboard
        glfwPollEvents();

        // Update the async keyboard and mouse (for the fly camera)
        handleMouseAsync(window);
        handleKeyboardAsync(window);
    }

    // Clean things up
    delete ::g_pShaderManager;
    delete ::g_pVAOManager;
    delete ::g_pFlyCamera;

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}


static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);

    return;
}

static void key_callback(GLFWwindow* window,
    int key,
    int scancode,
    int action,
    int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    // Shift (and only shift) down
    if (mods && GLFW_MOD_SHIFT == GLFW_MOD_SHIFT)
    {

        if (key == GLFW_KEY_9 && action == GLFW_PRESS)
        {
            ::g_bDrawDebugLightBalls = true;
        }
        if (key == GLFW_KEY_0 && action == GLFW_PRESS)
        {
            ::g_bDrawDebugLightBalls = false;
        }
    }

    //if (key == GLFW_KEY_UP && action == GLFW_PRESS)
    //{
    //    g_MeshIndex++;
    //}
    //if (key == GLFW_KEY_DOWN)
    //{
    //    g_MeshIndex--;
    //}

 //   const float CAMERASPEED = 1.0f;
 //   // Camera 
 //   if (key == GLFW_KEY_W)          // Forward (along Z axis)
 //   {
 //       eyePosition.z += CAMERASPEED;
 //   }
 //   if (key == GLFW_KEY_S)          // Backwards
 //   {
 //       eyePosition.z -= CAMERASPEED;
 //   }
 //   if (key == GLFW_KEY_A)          // Left (along X axis)
 //   {
 //       eyePosition.x -= CAMERASPEED;
 //   }
 //   if (key == GLFW_KEY_D)          // Right
 //   {
 //       eyePosition.x += CAMERASPEED;
 //   }
 //   // Up and down
 //   if (key == GLFW_KEY_Q)      // Up (along Y axis)
 //   {
 //       eyePosition.y += CAMERASPEED;
 //   }
 //   if (key == GLFW_KEY_E)      // Down?
 //   {
 //       eyePosition.y -= CAMERASPEED;
 //   }

    return;
}



//bool LoadModelFromFile(std::string fileName)
//{
//    std::ifstream theFile(fileName);
//
//    if (!theFile.is_open())
//    {
//        return false;
//    }
//
//    // Read until I hit "vertex"
//    std::string aToken;
//    while (theFile >> aToken)
//    {
//        if (aToken == "vertex")
//        {
//            break;
//        }
//    }
//    unsigned long numberOfVerticesInTheFile = 0;
//    theFile >> numberOfVerticesInTheFile;
//
//
//    // Read until I hit "face"
//    while (theFile >> aToken)
//    {
//        if (aToken == "face")
//        {
//            break;
//        }
//    }
//    unsigned long numberOfTrianglesInTheFile = 0;
//    theFile >> numberOfTrianglesInTheFile;
//
//    // Read until I hit "end_header"
//    while (theFile >> aToken)
//    {
//        if (aToken == "end_header")
//        {
//            break;
//        }
//    }
//    // Now the list of vertices
//    // Create a run time array in c ("C style array")
//    // Vertex* vertices = new Vertex[numberOfVertices];
//    //::g_Vertices = new Vertex[::g_NumberOfVertices];
//
//    // Make a copy of the vertices as they are in the file
//    Vertex* pVerticesInFile = new Vertex[numberOfVerticesInTheFile];
//
//    //element vertex 3014
//    //property float x
//    //property float y
//    //property float z
//    //property float nx
//    //property float ny
//    //property float nz
//
//    for (unsigned long index = 0; index != numberOfVerticesInTheFile; index++)
//    {
//        // -0.113944 0.168176 -0.404122 0.811943 0.485765 0.323699 
//        theFile >> pVerticesInFile[index].position.x;
//        theFile >> pVerticesInFile[index].position.y;
//        theFile >> pVerticesInFile[index].position.z;
//
//        float discard = 0.0f;
//        theFile >> discard;
//        theFile >> discard;
//        theFile >> discard;
//
//        pVerticesInFile[index].colour.r = 1.0f;
//        pVerticesInFile[index].colour.g = 1.0f;
//        pVerticesInFile[index].colour.b = 1.0f;
//
//        // HACK:
//        //g_vecVertices.push_back(::g_Vertices[index]);
//
//    }
//
//    // Now I read the triangles (aka "faces")
//
//
//
//    struct cTriangle
//    {
//        unsigned long vertex_0;
//        unsigned long vertex_1;
//        unsigned long vertex_2;
//    };
//    cTriangle* pTriangles = new cTriangle[numberOfTrianglesInTheFile];
//
//    // and load them...
//    for (unsigned int index = 0; index != numberOfTrianglesInTheFile; index++)
//    {
//        // 3 5 14 21 
//        int discard = 0;
//        theFile >> discard;         // 3
//        theFile >> pTriangles[index].vertex_0;
//        theFile >> pTriangles[index].vertex_1;
//        theFile >> pTriangles[index].vertex_2;
//    }
//
//    // Make the thing that OpenGL wants
//    // Now many actual vertices are being drawn.
//    // Number of triangles * number of vertices per triangle
//
//    ::g_NumberOfVerticesToDraw = numberOfTrianglesInTheFile * 3;
//
//    //Vertex* g_VerticesToDraw = NULL;
//    ::g_VerticesToDraw = new Vertex[::g_NumberOfVerticesToDraw];
//
//    // Go through the triangles 
//    // Look up each vertex that corresponds to that triangle's vertex
//    // Load THAT vertex into the final array
//    unsigned int vertIndex = 0;
//    for (unsigned int triIndex = 0; triIndex != numberOfTrianglesInTheFile; triIndex++)
//    {
//        ::g_VerticesToDraw[vertIndex + 0].position.x
//            = pVerticesInFile[pTriangles[triIndex].vertex_0].position.x;
//        ::g_VerticesToDraw[vertIndex + 0].position.y
//            = pVerticesInFile[pTriangles[triIndex].vertex_0].position.y;
//        ::g_VerticesToDraw[vertIndex + 0].position.z
//            = pVerticesInFile[pTriangles[triIndex].vertex_0].position.z;
//
//
//        ::g_VerticesToDraw[vertIndex + 1].position.x
//            = pVerticesInFile[pTriangles[triIndex].vertex_1].position.x;
//        ::g_VerticesToDraw[vertIndex + 1].position.y
//            = pVerticesInFile[pTriangles[triIndex].vertex_1].position.y;
//        ::g_VerticesToDraw[vertIndex + 1].position.z
//            = pVerticesInFile[pTriangles[triIndex].vertex_1].position.z;
//
//        ::g_VerticesToDraw[vertIndex + 2].position
//            = pVerticesInFile[pTriangles[triIndex].vertex_2].position;
//
//        ::g_VerticesToDraw[vertIndex].colour = glm::vec3(1.0f);
//        ::g_VerticesToDraw[vertIndex + 1].colour = glm::vec3(1.0f);
//        ::g_VerticesToDraw[vertIndex + 2].colour = glm::vec3(1.0f);
//
//
//        // Increment the vertex index by 3 to get to the next set of 3 
//        vertIndex += 3;
//    }
//
//    return true;
//}