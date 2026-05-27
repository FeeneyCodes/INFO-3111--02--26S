// Pixel Fragment shader
#version 330

in vec3 color;
out vec4 fragment;

uniform vec3 colourRGB;

void main()
{
	//fragment = vec4(color, 1.0);
	vec3 theColour = color * 0.00001f;
	theColour.rgb += colourRGB.rgb;
	fragment = vec4(theColour, 1.0);
}