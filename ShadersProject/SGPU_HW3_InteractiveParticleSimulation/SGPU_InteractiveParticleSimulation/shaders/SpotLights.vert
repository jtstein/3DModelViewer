#version 330 
#extension GL_ARB_explicit_attrib_location : require 
#extension GL_ARB_explicit_uniform_location : require 

// Jordan Stein 101390302
// Homework 2

/* Strict Binding for Cross-hardware Compatability */
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec4 tangent;
layout(location = 3) in vec3 textureCoordinate;
layout(location = 4) in vec3 color;

out vec3 Position;
out vec3 Normal;

/* Uniform variables for Camera and Light Direction */
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;

struct Light {
	vec4 position;
	vec3 direction;
	vec3 intensity;
	//vec3 ambient;
	//vec3 diffuse;
	//vec3 specular;
	float exponent;
	float cutoff;
};

uniform Light light0;
uniform Light light1;
uniform Light light2;

out vec3 interpSurfaceNormal;
out vec3 interpVertexPosition;
out vec2 interpTextureCoord;


void main(void) {

	vec4 vPosition = vec4(position, 1.0f);
	
    vec3 vertexPositionEye = vec3(modelViewMatrix * vPosition);
    interpVertexPosition = vertexPositionEye;
    interpSurfaceNormal = normalize(normalMatrix * normal);
	interpTextureCoord = vec2(textureCoordinate);

	//---------------------------------------------------------------------------- 
	// TBN Matrix for normal mapping.
	//---------------------------------------------------------------------------- 
	vec3 n = interpSurfaceNormal;
	vec3 t = normalize(normalMatrix * vec3(tangent.xyz));
	vec3 b = cross(n, t) * tangent.w;

	//-------------------------------------------------------------------------- 
	// Transform the vertex for the fragment shader. 
	//-------------------------------------------------------------------------- 
    gl_Position = projectionMatrix * modelViewMatrix * vPosition;

}