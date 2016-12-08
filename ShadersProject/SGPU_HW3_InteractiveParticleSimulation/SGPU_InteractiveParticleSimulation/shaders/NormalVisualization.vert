#version 330 
#extension GL_ARB_explicit_attrib_location : require 
#extension GL_ARB_explicit_uniform_location : require 

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec4 tangent;
layout(location = 3) in vec3 textureCoordinate;
layout(location = 4) in vec3 color;

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;

out Vertex {
	vec3 position;
	vec3 normal;
	vec4 tangent;
	vec3 textureCoord;
	vec3 color;
} vertex;

void main(void) {
	vertex.position = position;
	vertex.normal = normal;
	vertex.tangent = tangent;
	vertex.textureCoord = textureCoordinate;
	vertex.color = color;

    gl_Position = vec4(position, 1.0f);
}