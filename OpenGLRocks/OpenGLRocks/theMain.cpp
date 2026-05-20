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
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "cShaderManager/cShaderManager.h"
#include "cVAOManager/cVAOManager.h"

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



glm::vec3 eyePosition = glm::vec3(0.0f, 0.0f, -2.0f);
glm::vec3 atPosition = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 upAxis = glm::vec3(0.0f, +1.0f, 0.0f);



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
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

    sModelDrawInfo migModelInfo;
    if (::g_pVAOManager->LoadModelIntoVAO( "assets//models//bun_zipper_XYZ_N.ply",
                                           migModelInfo,
                                           program))
    {
        std::cout << "Loaded " << migModelInfo.meshName << std::endl;
        std::cout << "\t" << migModelInfo.numberOfVertices << " vertices" << std::endl;
        std::cout << "\t" << migModelInfo.numberOfTriangles << " triangles" << std::endl;
    }

    ::g_pVAOManager->LoadModelIntoVAO("assets//models//mig27.ply",
        migModelInfo,
        program);


    const GLint mvp_location = glGetUniformLocation(program, "MVP");


    //std::vector<cMesh> vecModelsToDraw;
    
    while (!glfwWindowShouldClose(window))
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        const float ratio = width / (float)height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        glm::mat4 m;             //mat4x4 m, p, mvp;
        glm::mat4 p;
        glm::mat4 mvp;
        //mat4x4_identity(m);
        
 //       mat4x4_rotate_Z(m, m, (float)glfwGetTime());
        glm::mat4 rotateZ = glm::rotate( glm::mat4(1.0f), 
                                         0.0f, // (float)glfwGetTime(),         // Angle
                                         glm::vec3(0.0f, 0.0f, 1.0f));
            


        // the "camera"
        glm::mat4 matView = glm::lookAt(eyePosition, atPosition, upAxis);

        // projection matrix
        p = glm::perspective( 60.0f,        // FOV
                              (float)width / (float)height, // Aspect ratio
                              0.001f,            // Near plane
                              100.0f);         // Far plane
                              

 //       mat4x4_mul(mvp, p, m);
        m = glm::mat4(1.0f);        // Identity matrix
        // combine the rotation
        m = rotateZ * m;
        
        //mvp  -- pvm
        mvp = p * matView * m;

        //glPointSize(6.0f);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glUseProgram(program);

        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)&mvp);

//        glBindVertexArray(vertex_array);

        //glDrawArrays(GL_TRIANGLES, 0, 3);
        //glDrawArrays(GL_TRIANGLES, 0, ::g_NumberOfVertices);
        //glDrawArrays(GL_TRIANGLES, 0, ::g_NumberOfVerticesToDraw);
        //glDrawArrays(GL_TRIANGLES, 0, 0);

        //enable VAO(and everything else)
        sModelDrawInfo theModelToDraw;
        if (::g_pVAOManager->FindDrawInfoByModelName( "assets//models//bun_zipper_XYZ_N.ply",
                                                      theModelToDraw))
        {
            glBindVertexArray(theModelToDraw.VAO_ID);

            // Uses the index (element) buffer to look up
            //  into the bound vertex buffer and sends those final
            //  vertices to the vertex shader
            glDrawElements(GL_TRIANGLES,
                           theModelToDraw.numberOfIndices, // How many indices
                           GL_UNSIGNED_INT,
                           (void*) 0);      // What index to start from

            //disable VAO(and everything else)
            glBindVertexArray(0); 	
        }
		


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

    const float CAMERASPEED = 0.01f;
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