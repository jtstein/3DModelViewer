#version 330

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 normalMatrix;
uniform vec3 lightPosition;

attribute vec3 position;
attribute vec3 normal;
attribute vec4 tangent;
attribute vec2 textureCoordinate;

varying vec2 textureCoord;
varying vec3 p;
varying vec3 surfaceNormal;
varying vec3 lightDir;

varying vec3 lightVec;
varying vec3 eyeVec;
varying vec3 halfVec;

void main(void) {
	vec4 vertexPos = vec4(position, 1.0);
	p = (modelViewMatrix * vertexPos).xyz;
	lightDir = normalize(lightPosition - position);
	
	vec3 n = normalize(( vec4(normal, 1.0f)).xyz);
	vec3 t = normalize(( tangent).xyz);
	vec3 b = cross(n, t) * tangent.w;
	
	//if ( vertexPos.z < 0.0f ) n *= -1.0f;
	
	vec3 v;
	v.x = dot(lightDir, t);
	v.y = dot(lightDir, b);
	v.z = dot(lightDir, n);
	lightVec = normalize(v);
	
	v.x = dot(p, t);
	v.y = dot(p, b);
	v.z = dot(p, n);
	eyeVec = normalize(v);
	
	vec3 halfVector = normalize(p + lightDir);
	v.x = dot(halfVector, t);
	v.y = dot(halfVector, b);
	v.z = dot(halfVector, n);
	halfVec = v;
	
	textureCoord = textureCoordinate;
	surfaceNormal = normalize(normal);
	gl_Position = projectionMatrix * modelViewMatrix * vertexPos;
}