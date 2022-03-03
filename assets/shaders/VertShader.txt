#version 430 core
// Per-vertex inputs
layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec3 vNormalIn;
layout(location = 2) in vec2 vTexCoordIn;

// Uniform data inputs are the same for all vertices
uniform mat4 modelMat;
uniform mat4 invModelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

uniform vec4 worldSpaceLightPos = {1,0.0,1,1};

// These per-vertex outputs must correspond to the per-fragment inputs in the fragment shader
out vec3 vNormalV;
out vec3 eyeSpaceLightPosV;
out vec3 eyeSpaceVertPosV;
out vec2 texCoord;

void main()
{
	// Perform vertex transformations
	gl_Position = projMat * viewMat * modelMat * vPosition;
	
	// Vector from eye to vertex position, in eye-space
	eyeSpaceVertPosV = vec3(viewMat * modelMat * vPosition);
	// Vector from vertex position to light position, in eye-space
	eyeSpaceLightPosV = vec3(viewMat * worldSpaceLightPos);

	// Vertex normal, in eye-space
	vNormalV = mat3(viewMat * modelMat) * vNormalIn;

	// Pass through the texture coordinate
	texCoord = vTexCoordIn;
}

