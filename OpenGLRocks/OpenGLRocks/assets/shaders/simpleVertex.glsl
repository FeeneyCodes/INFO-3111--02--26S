// Vertex shader
#version 330   

//uniform mat4 MVP;
uniform mat4 mModel;
uniform mat4 mView;
uniform mat4 mProj;

// Vertices are coming in here...
in vec4 vertexColour;		// RGBA
in vec4 vertexPosition;		// XYZ (w not used)
in vec4 vertexNormal;		// XYZ (w not used)
in vec4 vertexUVx2;			// 2 sets of UVs (because it's a vec4)

out vec4 vModelColour;
out vec4 vNormal;
out vec4 vWorldPosition;
out vec4 vVertexUVx2;

void main()
{
	mat4 MVP = mProj * mView * mModel;
	gl_Position = MVP * vertexPosition;
	
	// World position of this vertex
	vWorldPosition = mModel * vertexPosition;
	
	// Calculating the "inverse transpose" of the model matrix
	// (which takes away any translation and scaling and leaves ONLY rotation)
	vNormal = inverse(transpose(mModel)) * vertexNormal;
	// Make sure the normal is STILL a unit length 
	// (aka "noramlized")
	vNormal = normalize(vNormal);
	
	// Copy these unchanged
	vModelColour = vertexColour;
	vVertexUVx2 = vertexUVx2;
}