#version 330 
#extension GL_ARB_explicit_attrib_location : require 
#extension GL_ARB_explicit_uniform_location : require 

/*
 * This geometry shader takes in a point and generates a new triangle strip
 * with 4 vertices based on the definition of the points (a), (b), (c), and (d)
 * that form a square plane that always faces the camera.
 */
layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 4) out;

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;
uniform float size;

/*
 * Definition of the particles properties defined in the vertex shader.
 */
in Vertex {
	vec3 position;
	vec3 velocity;
	vec3 force;
	vec3 color;
	float mass;
	float lifetime;
} vertex[];

/*
 * Properties that will be exported to the fragment shader. These are specific
 * to each vertex that is generated in this geometry shader. These properties
 * must be set prior to each call to EmitVertex().
 */
out vec2 vertexUV;
out vec3 vertexColor;
out float velocity;
out float lifetime;

void main(void) {
	mat4 MV = modelViewMatrix;
	lifetime = vertex[0].lifetime;
	velocity = length(vertex[0].velocity);
	
	//--------------------------------------------------------------------------
	// Forming the billboard vectors that are orthogonal to the view of the
	// camera. This will ensure that the geometry generated for each particle
	// is always facing the camera.
	//--------------------------------------------------------------------------
	vec3 right = vec3(MV[0][0], MV[1][0], MV[2][0]);
	vec3 up = vec3(MV[0][1], MV[1][1], MV[2][1]);
	vec3 P = gl_in[0].gl_Position.xyz;
	
	//--------------------------------------------------------------------------
	// The new vertices will be converted from global coordinates to eye
	// coordinates as they are generated.
	//--------------------------------------------------------------------------
	mat4 VP = projectionMatrix * modelViewMatrix;
	
	//--------------------------------------------------------------------------
	// Generate the point (a) for the billboard particle surface.
	//--------------------------------------------------------------------------
	vec3 va = P - (right + up) * size;
	gl_Position = VP * vec4(va, 1.0f);
	vertexUV = vec2(0.0f, 0.0f);
	vertexColor = vertex[0].color;
	EmitVertex();
	
	//--------------------------------------------------------------------------
	// Generate the point (b) for the billboard particle surface.
	//--------------------------------------------------------------------------
	vec3 vb = P - (right - up) * size;
	gl_Position = VP * vec4(vb, 1.0f);
	vertexUV = vec2(0.0f, 1.0f);
	vertexColor = vertex[0].color;
	EmitVertex();
	
	//--------------------------------------------------------------------------
	// Generate the point (d) for the billboard particle surface. The vertices
	// (d) and (c) are out of order because the new geometry is formed as a 
	// triangle strip.
	//--------------------------------------------------------------------------
	vec3 vd = P + (right - up) * size;
	gl_Position = VP * vec4(vd, 1.0f);
	vertexUV = vec2(1.0f, 0.0f);
	vertexColor = vertex[0].color;
	EmitVertex();
	
	//--------------------------------------------------------------------------
	// Generate the point (c) for the billboard particle surface.
	//--------------------------------------------------------------------------
	vec3 vc = P + (right + up) * size;
	gl_Position = VP * vec4(vc, 1.0f);
	vertexUV = vec2(1.0f, 1.0f);
	vertexColor = vertex[0].color;
	EmitVertex();
	
	EndPrimitive();
}