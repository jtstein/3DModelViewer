

uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;
uniform sampler2D specularTexture;

varying vec2 textureCoord;
varying vec3 p;
varying vec3 surfaceNormal;
varying vec3 lightDir;

varying vec3 lightVec;
varying vec3 eyeVec;
varying vec3 halfVec;
 
void main() {
	vec3 ambientColor = vec3(0.1f, 0.1f, 0.1f);
	vec3 diffuseColor = vec3(0.9f, 0.9f, 0.9f);
	vec3 specularColor = vec3(1.0f, 1.0f, 1.0f);
	float shininess = 4.0f;
	
	vec3 pixelNormal = normalize( texture2D(normalTexture, textureCoord).xyz * 2.0 - 1.0);
	float lamberFactor = max(0.0f, dot(lightVec, pixelNormal));
	
	vec4 ambientLight = vec4(ambientColor, 1.0f);

	if ( lamberFactor >= 0.0f ) {
		vec4 diffuseMap = texture2D(diffuseTexture, textureCoord);
		vec4 diffuseLight = vec4(diffuseColor, 1.0f);
		
		ambientLight = vec4(ambientColor, 1.0f) * diffuseMap;
		
		vec4 specularMap = texture2D(specularTexture, textureCoord);
		vec4 specularLight = vec4(specularColor, 1.0f);

		gl_FragColor = diffuseMap ;
		gl_FragColor *= max(0.0f, dot(surfaceNormal, lightDir)) * lamberFactor;
		
		vec3 reflection = normalize(reflect(-lightDir, surfaceNormal));
		float reflectionAngle = max(0.0, dot(normalize(surfaceNormal), reflection));
		float specularWeight = 10.0f;
		float specularPower = clamp(dot(lightDir, surfaceNormal), 0.0f, 1.0f) * specularWeight;
		
		float specularExp = pow(clamp(dot(reflect(lightVec, pixelNormal), eyeVec), 0.0, 1.0), shininess);
		vec4 specularFinal = specularMap * specularLight * specularExp * specularPower;
		specularFinal *= max(0.0, dot(surfaceNormal, lightDir));
		
		gl_FragColor += specularFinal;
	}
	
	gl_FragColor += ambientLight;
}