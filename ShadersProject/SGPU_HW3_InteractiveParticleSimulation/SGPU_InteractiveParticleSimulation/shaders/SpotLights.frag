#version 330 
#extension GL_ARB_explicit_attrib_location : require 
#extension GL_ARB_explicit_uniform_location : require 

// Jordan Stein 101390302
// Homework 2

in vec3 lightVector; 

uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;
uniform sampler2D specularTexture;

in vec3 Position;
in vec3 Normal;

struct Light {
	vec4 position;
	vec3 direction; // target
	vec3 intensity; // includes ambient,diffuse,specular
	//vec3 ambient;
	//vec3 diffuse;
	//vec3 specular;
	float exponent;
	float cutoff;
};

uniform Light light0;
uniform Light light1;
uniform Light light2;

in vec3 interpSurfaceNormal;
in vec3 interpVertexPosition;
in vec2 interpTextureCoord;

uniform vec3 Kd; // Diffuse reflectivity
uniform vec3 Ka; // Ambient reflectivity
uniform vec3 Ks; // Specular reflectivity
uniform float Shininess; // Specular shininess factor

layout( location = 0 ) out vec4 fragColor;

vec3 adsWithSpotlight( Light Spot )
{
    vec3 s = normalize( vec3( Spot.position) - interpVertexPosition );
    vec3 spotDir = normalize( Spot.direction);
    float angle = acos( dot(-s, spotDir) );
    float cutoff = radians( clamp( Spot.cutoff, 0.0, 90.0 ) );
    vec3 ambient = Spot.intensity * Ka;

    if( angle > cutoff ) {
        float spotFactor = pow( dot(s, spotDir), Spot.exponent );
        vec3 v = normalize(vec3(-interpVertexPosition));
        vec3 h = normalize( v + s );

        return
            ambient +
            spotFactor * Spot.intensity * (
              Kd * max( dot(s, interpSurfaceNormal), 0.0 ) +
              Ks * pow( max( dot(h,interpSurfaceNormal), 0.0 ), Shininess )
           );
    } else {
        return ambient;
    }
}

void main(void) {

	// TODO: Phong-based spotlights for material surface modeling using diffuse, normal, and specular textures

	fragColor = ( vec4(adsWithSpotlight(light0),1.0) + vec4(adsWithSpotlight(light1),1.0) + vec4(adsWithSpotlight(light2),1.0) ) * texture2D(diffuseTexture, interpTextureCoord);
}