#pragma once

// This is so glad and GLFW (and opengl.h) are
// loaded in the right order, because we can't 
// control that with Visual Studio (no make file)

#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
//#include "include/glad/glad.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

