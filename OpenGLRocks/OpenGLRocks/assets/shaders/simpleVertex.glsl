// Vertex shader
#version 330   

//uniform mat4 MVP;
uniform mat4 mModel;
uniform mat4 mView;
uniform mat4 mProj;
// Vertices are coming in here...
in vec3 vCol;
in vec3 vPos;
// Going out here
out vec3 color;

void main()
{
	mat4 MVP = mProj * mView * mModel;
	gl_Position = MVP * vec4(vPos, 1.0);
	color = vCol;
}