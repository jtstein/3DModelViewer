#version 330

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 normalMatrix;
uniform vec3 lightPosition;

attribute vec3 position;
attribute vec3 normal;
attribute vec4 tangent;
attribute vec2 textureCoordinate;

varying vec3 surfaceNormal;
varying vec3 lightDir;

void main(void) {
	vec4 vertexPos = vec4(position, 1.0);
	lightDir = normalize(lightPosition - position);
	surfaceNormal = normalize(normalMatrix * vec4(normal, 1.0f)).xyz;
	gl_Position = projectionMatrix * modelViewMatrix * vertexPos;
}