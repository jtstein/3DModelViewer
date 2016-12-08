#version 330 
#extension GL_ARB_explicit_attrib_location : require 
#extension GL_ARB_explicit_uniform_location : require 

uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;
uniform sampler2D specularTexture;

/* 
 * Light model information interpolated between each vertex. This information is 
 * used to compute the light model within the fragment shader based on the 
 * interpolated vector values. 
 */
in vec3 interpSurfaceNormal;
in vec3 interpVertexPosition;
in vec3 interpLightPosition;
in vec2 interpTextureCoord;

/* Light direction in tangent space */
in vec3 lightVector;

/* Specular Mapping */
void main(void) {
	//---------------------------------------------------------------------------- 
	// Determine the normal for this fragment based on the normal texture.
	//---------------------------------------------------------------------------- 
	vec3 fragmentNormal = normalize(texture2D(normalTexture, interpTextureCoord).xyz * 2.0 - 1.0);
	
	//-------------------------------------------------------------------------- 
	// Light, camera, and reflection direction calculations.
	//-------------------------------------------------------------------------- 
	vec3 l = normalize(interpLightPosition - interpVertexPosition);
	vec3 c = normalize(-interpVertexPosition);
	vec3 r = normalize(-reflect(l, interpSurfaceNormal));
	
	//-------------------------------------------------------------------------- 
	// Light and material properties. 
	//-------------------------------------------------------------------------- 
	vec4 Ia = vec4(0.1f, 0.1f, 0.1f, 1.0f); 
	vec4 Id = vec4(0.9f, 0.9f, 0.9f, 1.0f); 
	vec4 Is = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	
	//--------------------------------------------------------------------------
	// Material Properties
	//--------------------------------------------------------------------------
	vec4 Ka = vec4(0.1f, 0.1f, 0.1f, 1.0f);
	vec4 Kd = vec4(0.8f, 0.8f, 0.8f, 1.0f);
	vec4 Ks = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	float shininess = 16.0f;
	
	vec4 Iambient = vec4(0.0f);
	vec4 Idiffuse = vec4(0.0f);
	vec4 Ispecular = vec4(0.0f);
        
	//-------------------------------------------------------------------------- 
	// Assign the vertex color as the ambient color. 
	//--------------------------------------------------------------------------
	Iambient = Ia * Ka;
 
	//-------------------------------------------------------------------------- 
	// Calculate the diffuse component based on the surface normal and the light 
	// direction and add it to the vertex color. 
	//-------------------------------------------------------------------------- 
	float lambertComponent = max(0.0f, dot(lightVector, fragmentNormal)); 
	Idiffuse = texture2D(diffuseTexture, interpTextureCoord);
	Idiffuse *= (Id * Kd) * lambertComponent;
	
	//-------------------------------------------------------------------------- 
	// Calculate the specular component based on the camera position and 
	// reflection direction. The reflection vector is scaled by the provided
	// intensity value read from the specular map.
	//-------------------------------------------------------------------------
	Ispecular = (Is * Ks) * pow(max(dot(r, c), 0.0f), shininess);
	Ispecular *= texture2D(specularTexture, interpTextureCoord);
	
	//-------------------------------------------------------------------------- 
	// Calculate the final ADS light value for this vertex. 
	//--------------------------------------------------------------------------
	gl_FragColor = Iambient + Idiffuse + Ispecular;  
}