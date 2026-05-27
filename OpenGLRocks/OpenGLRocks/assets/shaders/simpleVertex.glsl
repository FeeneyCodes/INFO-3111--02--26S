// Vertex shader
#version 330   

uniform mat4 MVP;

// Vertices are coming in here...
in vec3 vCol;
in vec3 vPos;

// Going out here
out vec3 color;

void main()
{
	gl_Position = MVP * vec4(vPos, 1.0);
	color = vCol;
}