#version 330 
#extension GL_ARB_explicit_attrib_location : require 
#extension GL_ARB_explicit_uniform_location : require 

/* 
 * The input to this geometry shader is each triangle (individually) to be
 * processed based on its three vertices. The output of this shader will be
 * a triangle_strip, even though only one triangle is created (the inset
 * triangle).
 */
layout (triangles) in;
layout (triangle_strip) out;
layout (max_vertices = 3) out;

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;
uniform vec3 lightPosition;

/* Uniform scale that defines the distance of the inset (0 = no inset) */
uniform float scale;

/* Input array of vertices (since its a triangle-based shader there will be 3). */
in Vertex {
	vec3 position;
	vec3 normal;
	vec4 tangent;
	vec3 textureCoord;
	vec3 color;
} vertex[];

/* Interpolated output provided to the fragment shader for Phong shading */
out vec3 interpLightPosition;
out vec3 interpSurfaceNormal;
out vec3 interpVertexPosition;

/* 
 * This function calculates the inset of the current vertex of the triangle. This
 * direction is defined as the sum of the two vectors that form the sides of the
 * triangle. The result is then normalized to provide a directional vector.
 */
vec3 CalculateInset(int index, float inset) {
	vec3 insetDir;
	vec3 a, b, c, uDir, vDir;
	a = vertex[0].position;
	b = vertex[1].position;
	c = vertex[2].position;
	
	if ( index == 0 ) {
		uDir = normalize(b - a);
		vDir = normalize(c - a);
	}
	
	if ( index == 1 ) {
		uDir = normalize(a - b);
		vDir = normalize(c - b);
	}
	
	if ( index == 2 ) {
		uDir = normalize(a - c);
		vDir = normalize(b - c);
	}
	
	insetDir = -normalize(uDir + vDir);
	return insetDir * inset;
}

/* 
 * For each vertex (3) in the triangle, calculate the inset direction and then
 * copy the input vertex position, add insetDir * scale, and then emit the new
 * vertex. This will result in each vertex being 'pushed' into the center of
 * the original triangle. This will result in all of the faces of the object
 * being inset from their original position.
 */
void main() {
	for ( int i = 0; i < 3; i++ ) {
		//----------------------------------------------------------------------
		// Inset the current vertex of the triangle.
		//----------------------------------------------------------------------
		vec3 dispPosition = vertex[i].position + CalculateInset(i, scale);
		vec4 projPosition = projectionMatrix * modelViewMatrix * vec4(dispPosition, 1.0f);
		
		//----------------------------------------------------------------------
		// Calculate the surface normal and vertex position for the Phong
		// fragment shader.
		//----------------------------------------------------------------------
		interpSurfaceNormal = normalize(normalMatrix * vertex[i].normal);
		interpVertexPosition = vec3(modelViewMatrix * vec4(vertex[i].position, 1.0f));
		
		gl_Position = projPosition;
		EmitVertex();
	}
	
	//--------------------------------------------------------------------------
	// Complete and emit the primitive.
	//--------------------------------------------------------------------------
	EndPrimitive();
}
