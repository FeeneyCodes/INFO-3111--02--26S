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

#include "cShaderManager/cShaderManager.h"

cShaderManager* g_pShaderManager = NULL;

struct Vertex
{
    glm::vec3 position;      // vec2 pos;  x, y
    glm::vec3 colour;       // 0.0 - 1.0   0-255
};

Vertex* g_Vertices = NULL;
unsigned long g_NumberOfVertices = 0;

//Vertex vertices[3] =
//{
//    //     X      Y     Z         R    G     B
//    { { -0.6f, -0.4f, 0.0f }, { 1.0f, 0.0f, 0.0f } },
//    { {  0.6f, -0.4f, 0.0f }, { 0.0f, 1.0f, 0.0f } },
//    { {  0.0f,  0.6f, 0.0f }, { 0.0f, 0.0f, 1.0f } },
//};


glm::vec3 eyePosition = glm::vec3(0.0f, 0.0f, -4.0f);
glm::vec3 atPosition = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 upAxis = glm::vec3(0.0f, +1.0f, 0.0f);

//static const char* vertex_shader_text =
//"#version 330\n"    
//"uniform mat4 MVP;\n"
//"in vec3 vCol;\n"
//"in vec3 vPos;\n"
//"out vec3 color;\n"
//"void main()\n"
//"{\n"
//"    gl_Position = MVP * vec4(vPos, 1.0);\n"
//"    color = vCol;\n"
//"}\n";
//
//static const char* fragment_shader_text =
//"#version 330\n"
//"in vec3 color;\n"
//"out vec4 fragment;\n"
//"void main()\n"
//"{\n"
//"    fragment = vec4(color, 1.0);\n"
//"}\n";

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


bool LoadModelFromFile(std::string fileName)
{
    std::ifstream theFile(fileName);

    if (!theFile.is_open())
    {
        return false;
    }

    // Read until I hit "vertex"
    std::string aToken;
    while (theFile >> aToken)
    {
        if (aToken == "vertex")
        {
            break;
        }
    }
    //unsigned long numberOfVertices = 0;
    theFile >> ::g_NumberOfVertices;


    // Read until I hit "face"
    while (theFile >> aToken)
    {
        if (aToken == "face")
        {
            break;
        }
    }
    unsigned long numberOfTriangles = 0;
    theFile >> numberOfTriangles;

    // Read until I hit "end_header"
    while (theFile >> aToken)
    {
        if (aToken == "end_header")
        {
            break;
        }
    }
    // Now the list of vertices
    // Create a run time array in c ("C style array")
    //Vertex* vertices = new Vertex[numberOfVertices];
    ::g_Vertices = new Vertex[::g_NumberOfVertices];

    for (unsigned long index = 0; index != ::g_NumberOfVertices; index++)
    {
        // -0.113944 0.168176 -0.404122 0.811943 0.485765 0.323699 
        theFile >> ::g_Vertices[index].position.x;
        theFile >> ::g_Vertices[index].position.y;
        theFile >> ::g_Vertices[index].position.z;

        float discard = 0.0f;
        theFile >> discard;
        theFile >> discard;
        theFile >> discard;

        ::g_Vertices[index].colour.r = 1.0f;
        ::g_Vertices[index].colour.g = 1.0f;
        ::g_Vertices[index].colour.b = 1.0f;
    }


 //   Vertex* vertices = new Vertex[3];
 //   
 //   vertices[0].position.x = -0.6f, -0.4f, 0.0f }, { 1.0f, 0.0f, 0.0f };
    //    { {  0.6f, -0.4f, 0.0f }, { 0.0f, 1.0f, 0.0f } },
    //    { {  0.0f,  0.6f, 0.0f }, { 0.0f, 0.0f, 1.0f } },
    //};


    return true;
}


int main(void)
{
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

    // NOTE: OpenGL error checks have been omitted for brevity

    if ( ! LoadModelFromFile("assets//models//mig29.ply") )
    {
        std::cout << "File didn't load" << std::endl;
        return -1;
    }


    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

    GLsizeiptr sizeOfVertexArrayInByes
                     = ::g_NumberOfVertices * sizeof(Vertex);

    glBufferData( GL_ARRAY_BUFFER, 
                  sizeOfVertexArrayInByes,
                  (void*)::g_Vertices, 
                  GL_STATIC_DRAW);


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



    //const GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    //glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    //glCompileShader(vertex_shader);

    //const GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    //glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    //glCompileShader(fragment_shader);

    //const GLuint program = glCreateProgram();
    //glAttachShader(program, vertex_shader);
    //glAttachShader(program, fragment_shader);
    //glLinkProgram(program);

    GLuint program = ::g_pShaderManager->getIDFromFriendlyName("BasicShader");

    const GLint mvp_location = glGetUniformLocation(program, "MVP");


    GLuint vertex_array;
    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);
    
    // In the shader, where is the "vPos" variable?
    const GLint vpos_location = glGetAttribLocation(program, "vPos");
    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer( vpos_location, 
                           3,                       // vec2  --> vec3
                           GL_FLOAT, 
                           GL_FALSE,                // "Normalized"
                           sizeof(Vertex),          // number of bytes
                           (void*)offsetof(Vertex, position));

    const GLint vcol_location = glGetAttribLocation(program, "vCol");
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer( vcol_location, 
                           3, 
                           GL_FLOAT, 
                           GL_FALSE,
                           sizeof(Vertex), 
                           (void*)offsetof(Vertex, colour));

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
            

 //       mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
 
        //glm::vec3 eyePosition = glm::vec3( 0.0f, 0.0f, eyeZValue);
        //glm::vec3 atPosition = glm::vec3( 0.0f, 0.0f, 0.0f );
        //glm::vec3 upAxis = glm::vec3( 0.0f, +1.0f, 0.0f );

 //       eyeZValue += 0.005f;

        // the "camera"
        glm::mat4 matView = glm::lookAt(eyePosition, atPosition, upAxis);

        // projection matrix
        p = glm::perspective( 60.0f,        // FOV
                              (float)width / (float)height, // Aspect ratio
                              0.1f,            // Near plane
                              1000.0f);         // Far plane
                              

 //       mat4x4_mul(mvp, p, m);
        m = glm::mat4(1.0f);        // Identity matrix
        // combine the rotation
        m = rotateZ * m;
        
        //mvp  -- pvm
        mvp = p * matView * m;

        //glPointSize(6.0f);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glUseProgram(program);

        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)&mvp);

        glBindVertexArray(vertex_array);

        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawArrays(GL_TRIANGLES, 0, ::g_NumberOfVertices);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean things up
    delete ::g_pShaderManager;

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}