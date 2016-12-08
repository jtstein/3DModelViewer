#version 330

uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;
uniform sampler2D specularTexture;

varying vec3 surfaceNormal;
varying vec3 lightDir;

void main() {
	vec4 ambientColor = vec4(0.1f, 0.1f, 0.1f, 1.0f);
	vec4 diffuseColor = vec4(0.9f, 0.9f, 0.9f, 1.0f);
	vec4 specularColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	float shininess = 20.0f;
	
	gl_FragColor = ambientColor;
	
	float diffuse = max(0.0f, dot(normalize(surfaceNormal), normalize(lightDir)));
	vec4 diffuseLight = diffuse * diffuseColor;
	gl_FragColor += diffuseLight;
	
	if ( diffuse != 0.0f ) {
		vec3 reflection = normalize(reflect(-normalize(lightDir), normalize(surfaceNormal)));
		float reflectionAngle = max(0.0f, dot(normalize(surfaceNormal), reflection));

		float specularExp = pow(reflectionAngle, shininess);
		vec4 specularLight = specularColor * specularExp;
		gl_FragColor += specularLight;
	}
}