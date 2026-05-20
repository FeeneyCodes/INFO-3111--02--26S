#version 330

in vec3 color;
out vec4 fragment;

void main()
{
	//fragment = vec4(color, 1.0);
	vec3 theColour = color;
	theColour *= 0.00001f;
	theColour += vec3(1.0);
	fragment = vec4(theColour, 1.0);
}