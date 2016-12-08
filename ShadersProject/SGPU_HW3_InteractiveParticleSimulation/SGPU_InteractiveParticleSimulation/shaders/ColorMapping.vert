#version 330

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 normalMatrix;

attribute vec3 position;
attribute vec3 normal;
attribute vec4 tangent;
attribute vec2 textureCoordinate;

varying vec3 normalColor;

void main(void) {
	vec4 vertexPos = vec4(position, 1.0);
	normalColor = normalize(normal);
	gl_Position = projectionMatrix * modelViewMatrix * vertexPos;
}