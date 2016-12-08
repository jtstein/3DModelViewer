#version 330 
#extension GL_ARB_explicit_attrib_location : require 
#extension GL_ARB_explicit_uniform_location : require 

/* 
 * Light model information interpolated between each vertex. This information is 
 * used to compute the light model within the fragment shader based on the 
 * interpolated vector values. 
 */
in vec3 interpSurfaceNormal;
in vec3 interpVertexPosition;
in vec3 interpLightPosition;

out vec4 fragColor;

/* Geometry Inset Shader (Phong Shading) */
void main(void) {
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
	
	vec4 Ka = vec4(1.0f, 1.0f, 1.0f, 1.0f); 
	vec4 Kd = vec4(1.0f, 1.0f, 1.0f, 1.0f); 
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
	float lambertComponent = max(0.0f, dot(interpSurfaceNormal, l)); 
	Idiffuse = (Id * Kd) * lambertComponent; 
	
	//-------------------------------------------------------------------------- 
	// Calculate the specular component based on the camera position and 
	// reflection direction. 
	//------------------------------------------------------------------------- 
	Ispecular = (Is * Ks) * pow(max(dot(r, c), 0.0f), shininess); 
	
	//-------------------------------------------------------------------------- 
	// Calculate the final ADS light value for this vertex. 
	//--------------------------------------------------------------------------
	fragColor = Iambient + Idiffuse + Ispecular;  
}