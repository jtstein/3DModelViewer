#version 330 
#extension GL_ARB_explicit_attrib_location : require 
#extension GL_ARB_explicit_uniform_location : require 

/*
 * Geometric definition of a particle within the vertex buffer object. This
 * data is updated every frame (when the particle array is updated on the CPU).
 */
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 velocity;
layout(location = 2) in vec3 force;
layout(location = 3) in vec3 color;
layout(location = 4) in float mass;
layout(location = 5) in float lifetime;

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;

/*
 * Vertex data to be written to the Geometry shader. If any of the data from the
 * vertex is required for the fragment shader (to be later interpolated), 
 * then it can be first passed to the geometry shader. This data can also be
 * used within the geometry shader.
 */
out Vertex {
	vec3 position;
	vec3 velocity;
	vec3 force;
	vec3 color;
	float mass;
	float lifetime;
} vertex;

void main(void) {
	//--------------------------------------------------------------------------
	// All of the properties of this particle are passed to the geometry shader.
	//--------------------------------------------------------------------------
	vertex.position = position;
	vertex.velocity = velocity;
	vertex.force = force;
	vertex.color = color;
	vertex.mass = mass;
	vertex.lifetime = lifetime;
	
	//--------------------------------------------------------------------------
	// The final vertex position is not calculated here because more vertex
	// operations are going to be performed in the geometry shader.
	//--------------------------------------------------------------------------
	gl_Position = vec4(position, 1.0f);
}