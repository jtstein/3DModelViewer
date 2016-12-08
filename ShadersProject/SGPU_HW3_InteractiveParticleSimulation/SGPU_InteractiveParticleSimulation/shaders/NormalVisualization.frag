#version 330 
#extension GL_ARB_explicit_attrib_location : require 
#extension GL_ARB_explicit_uniform_location : require 

out vec4 fragColor;

void main(void) {
	 fragColor = vec4(0.1f, 1.0f, 0.1f, 1.0f);
}