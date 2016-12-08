#version 330 
#extension GL_ARB_explicit_attrib_location : require 
#extension GL_ARB_explicit_uniform_location : require 

uniform sampler2D diffuseTexture;

/*
 * Generated texture coordinate and particle color defined within the geometry
 * shader. This information is passed along to define how the texture is
 * applied to the surface of the geometry generated in the geometry shader.
 */
in vec2 vertexUV;
in vec3 vertexColor;
in float velocity;
in float lifetime;

out vec4 fragColor;

void main(void) {
	vec4 particleTex = texture2D(diffuseTexture, vertexUV.xy);
	vec4 alpha = velocity * 0.08f * texture2D(diffuseTexture, vertexUV.xy).aaaa;
	
	//--------------------------------------------------------------------------
	// Combine the particle texture (diffuse texture) and the color of the
	// particle generated in the main vertex buffer object.
	//--------------------------------------------------------------------------
	fragColor = particleTex * alpha * vec4(vertexColor, 1.0f);
}
