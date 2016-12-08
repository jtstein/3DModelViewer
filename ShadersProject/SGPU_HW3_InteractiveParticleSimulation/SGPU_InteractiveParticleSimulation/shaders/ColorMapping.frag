#version 330

varying vec3 normalColor;
 
void main() {
	gl_FragColor = vec4(normalColor, 1.0f);
}