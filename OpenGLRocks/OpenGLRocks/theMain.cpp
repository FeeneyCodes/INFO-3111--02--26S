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

cShaderManager* g_pShaderManager = NULL;
cVAOManager* g_pVAOManager = NULL;

//struct Vertex
//{
//    glm::vec3 position;      // vec2 pos;  x, y
//    glm::vec3 colour;       // 0.0 - 1.0   0-255
//};
//
//Vertex* g_VerticesToDraw = NULL;
//unsigned long g_NumberOfVerticesToDraw = 0;



glm::vec3 eyePosition = glm::vec3(0.0f, 0.0f, -10.0f);
glm::vec3 atPosition = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 upAxis = glm::vec3(0.0f, +1.0f, 0.0f);

// Anything in this vector will be drawn in the main loop
// (declared in globalStuff.h)
std::vector< cMesh* > g_vec_pModelsToDraw;

static void error_callback(int error, const char* description);

static void key_callback(GLFWwindow* window,
    int key,
    int scancode,
    int action,
    int mods);


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

    cMesh* pTerrain = new cMesh("Terrain_xyz_only.ply");
    pTerrain->diffuseRGB = glm::vec3(1.0f, 1.0f, 1.0f);
    pTerrain->bIsWireframe = true;
    ::g_vec_pModelsToDraw.push_back(pTerrain);


    cMesh* pMig = new cMesh("mig29.ply");
    pMig->position.x = 5.0f;
    pMig->position.y = 0.1f;
    pMig->position.z = 0.7f;
    pMig->rotation.y = glm::radians<float>(90.0f);
    pMig->scale = 1.0f;
    pMig->diffuseRGB = glm::vec3(0.7f, 1.0f, 0.3f);

    cMesh* pShuttle = new cMesh("SpaceShuttleOrbiter_xyz_n.ply");
    pShuttle->scale = 1.0f / 200.0f;
    pShuttle->rotation.x = glm::radians<float>(-90.0f);
    pShuttle->diffuseRGB = glm::vec3(0.56f, 0.8f, 0.1f);
    pShuttle->bIsWireframe = true;

    cMesh* pBunny = new cMesh("bun_zipper_XYZ_N.ply");
    pBunny->position.x = -3.0f;
    pBunny->scale = 2.0f;
    pBunny->diffuseRGB = glm::vec3(0.3f, 0.26f, 0.56f);

    cMesh* pBunny2 = new cMesh("bun_zipper_XYZ_N.ply");
    pBunny2->position.x = -3.0f;
    pBunny2->position.y = 2.0f;
    pBunny2->scale = 2.5f;
    pBunny2->diffuseRGB = glm::vec3(0.6f, 0.1f, 0.7f);

    ::g_vec_pModelsToDraw.push_back( pMig );
    ::g_vec_pModelsToDraw.push_back( pShuttle );
    ::g_vec_pModelsToDraw.push_back(pBunny);
    ::g_vec_pModelsToDraw.push_back(pBunny2);

    const GLint mvp_location = glGetUniformLocation(program, "MVP");


    
    while (!glfwWindowShouldClose(window))
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        const float ratio = width / (float)height;

        glViewport(0, 0, width, height);
        // Clear the screen
        //glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Proection and View (camera) only changes once per scene draw

        glm::mat4 matProjection;    // p
        // the "camera"
        glm::mat4 matView = glm::lookAt(eyePosition, atPosition, upAxis);

        // projection matrix
        matProjection = glm::perspective(
            glm::radians(60.0f),        // FOV
            (float)width / (float)height, // Aspect ratio
            0.1f,            // Near plane
            1000.0f);         // Far plane


        for ( std::vector< cMesh* >::iterator it_pMesh = ::g_vec_pModelsToDraw.begin();
              it_pMesh != ::g_vec_pModelsToDraw.end();
              it_pMesh++)
        {
            // *******************************************************
            // STARTOF : Draw THIS model

            cMesh* pCurrentMesh = *it_pMesh;

            // Anything to do with model is now inside a loop

            glm::mat4 mvp;

            // The model matrix when set to the "identity" 
            //  means it's just like it is in the file.
            glm::mat4 matModel = glm::mat4(1.0f);        // m          //mat4x4 m, p, mvp;

            // Movement or placement 
            //glm::mat4 matTranslate = glm::mat4(1.0f);
            glm::mat4 matTranslate
                = glm::translate( glm::mat4(1.0f), 
                                  pCurrentMesh->position );
                                                    
            // Rotation
            glm::mat4 matRotateX
                = glm::rotate( glm::mat4(1.0f),
                               pCurrentMesh->rotation.x,
                               glm::vec3(1.0f, 0.0f, 0.0f) );

            glm::mat4 matRotateY
                = glm::rotate( glm::mat4(1.0f),
                               pCurrentMesh->rotation.y,
                               glm::vec3(0.0f, 1.0f, 0.0f) );

            glm::mat4 matRotateZ 
                = glm::rotate( glm::mat4(1.0f),
                               pCurrentMesh->rotation.z,
                               glm::vec3(0.0f, 0.0f, 1.0f) );

            // Scaling
            glm::mat4 matScaleXYZ
                = glm::scale( glm::mat4(1.0f),
                              glm::vec3( pCurrentMesh->scale,
                                         pCurrentMesh->scale,
                                         pCurrentMesh->scale) );


            // Apply the scale matrix
            matModel = matScaleXYZ * matModel;  // Last applied
            // Apply the rotation matrices
            matModel = matRotateX * matModel;
            matModel = matRotateY * matModel;
            matModel = matRotateZ * matModel;
            // Apply the translation matrix
            matModel = matTranslate * matModel; // 1st applied

            //mvp  -- pvm
            // mvp = p * matView * m;
            mvp = matProjection * matView * matModel;

            //glPointSize(6.0f);

            // GL_LINE gives "wireframe"
            // GL_FILL is default (solid or "filled" triangles)

            glUseProgram(program);

            glUniformMatrix4fv( mvp_location, 
                                1, 
                                GL_FALSE, 
                                (const GLfloat*)&mvp);

            //        glBindVertexArray(vertex_array);

                    //glDrawArrays(GL_TRIANGLES, 0, 3);
                    //glDrawArrays(GL_TRIANGLES, 0, ::g_NumberOfVertices);
                    //glDrawArrays(GL_TRIANGLES, 0, ::g_NumberOfVerticesToDraw);
                    //glDrawArrays(GL_TRIANGLES, 0, 0);

            // Get the location of the colour variable in the shader
            // uniform vec3 colourRGB;
            GLint colourRGB_UL = glGetUniformLocation(program, "colourRGB");

            glUniform3f( colourRGB_UL,
                         pCurrentMesh->diffuseRGB.r,
                         pCurrentMesh->diffuseRGB.g,
                         pCurrentMesh->diffuseRGB.b);

            if (pCurrentMesh->bIsWireframe)
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
            else
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }

            std::string modelToDraw = pCurrentMesh->modelName;

            sModelDrawInfo theModelToDraw;
            if (::g_pVAOManager->FindDrawInfoByModelName( modelToDraw,
                                                          theModelToDraw))
            {
                glBindVertexArray(theModelToDraw.VAO_ID);

                // Uses the index (element) buffer to look up
                //  into the bound vertex buffer and sends those final
                //  vertices to the vertex shader
                glDrawElements(GL_TRIANGLES,
                    theModelToDraw.numberOfIndices, // How many indices
                    GL_UNSIGNED_INT,
                    (void*)0);      // What index to start from

                //disable VAO(and everything else)
                glBindVertexArray(0);
            }

            // ENDOF: Drawing THIS model
            // ************************************************


        }//for ( std::vector<cMesh*>::iterator it_pMesh
		
        // Print out the camera's location
        std::stringstream ssWindowText;
        ssWindowText << "Camera (xyz): "
            << eyePosition.x << ", "
            << eyePosition.y << ", "
            << eyePosition.z;

        glfwSetWindowTitle( window, ssWindowText.str().c_str() );

        // Everything is drawn so "present" the "back buffer"

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean things up
    delete ::g_pShaderManager;

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

    const float CAMERASPEED = 1.0f;
    // Camera 
    if (key == GLFW_KEY_W)          // Forward (along Z axis)
    {
        eyePosition.z += CAMERASPEED;
    }
    if (key == GLFW_KEY_S)          // Backwards
    {
        eyePosition.z -= CAMERASPEED;
    }
    if (key == GLFW_KEY_A)          // Left (along X axis)
    {
        eyePosition.x -= CAMERASPEED;
    }
    if (key == GLFW_KEY_D)          // Right
    {
        eyePosition.x += CAMERASPEED;
    }
    // Up and down
    if (key == GLFW_KEY_Q)      // Up (along Y axis)
    {
        eyePosition.y += CAMERASPEED;
    }
    if (key == GLFW_KEY_E)      // Down?
    {
        eyePosition.y -= CAMERASPEED;
    }

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