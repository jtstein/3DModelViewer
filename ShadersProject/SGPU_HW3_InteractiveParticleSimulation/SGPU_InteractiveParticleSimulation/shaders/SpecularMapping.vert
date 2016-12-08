#version 330 
#extension GL_ARB_explicit_attrib_location : require 
#extension GL_ARB_explicit_uniform_location : require 

/* Strict Binding for Cross-hardware Compatability */
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec4 tangent;
layout(location = 3) in vec3 textureCoordinate;
layout(location = 4) in vec3 color;

/* Uniform variables for Camera and Light Direction */ 
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;
uniform vec3 lightPosition;

/* 
 * Light model information interpolated between each vertex. This information is 
 * used to compute the light model within the fragment shader based on the 
 * interpolated vector values. 
 */ 
out vec3 interpSurfaceNormal;
out vec3 interpVertexPosition;
out vec3 interpLightPosition;
out vec2 interpTextureCoord;

/* Light direction in tangent space */
out vec3 lightVector;

/* Specular Mapping */
void main(void) {
	vec4 vPosition = vec4(position, 1.0f);
	vec4 lPosition = vec4(lightPosition, 1.0f);
	
	//---------------------------------------------------------------------------- 
	// ADS Interpolated Light Model Vectors 
	//---------------------------------------------------------------------------- 
	interpLightPosition = vec3(modelViewMatrix * lPosition);
    interpVertexPosition = vec3(modelViewMatrix * vPosition);       
    interpSurfaceNormal = normalize(normalMatrix * normal);
	interpTextureCoord = vec2(textureCoordinate);
	
	//---------------------------------------------------------------------------- 
	// Determine the direction of the light from this vertex.
	//---------------------------------------------------------------------------- 
	vec3 lightDirection = vec3(interpLightPosition - interpVertexPosition);
	
	//---------------------------------------------------------------------------- 
	// TBN Matrix. The matrix is left as a set of orthogonal vectors. The vector
	// product with the matrix is the same as using three dot products.
	//---------------------------------------------------------------------------- 
	vec3 n = interpSurfaceNormal;
	vec3 t = normalize(normalMatrix * vec3(tangent.xyz));
	vec3 b = cross(n, t) * tangent.w;
	
	//---------------------------------------------------------------------------- 
	// Determine the light direction in tangent space.
	//---------------------------------------------------------------------------- 
	vec3 v;
	v.x = dot(lightDirection, t);
	v.y = dot(lightDirection, b);
	v.z = dot(lightDirection, n);
	lightVector = normalize(v);
	
	//-------------------------------------------------------------------------- 
	// Transform the vertex for the fragment shader. 
	//-------------------------------------------------------------------------- 
    gl_Position = projectionMatrix * modelViewMatrix * vPosition;
}