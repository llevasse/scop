#version 330 core

in vec3 fragmentColour;
in vec2	TexCoord;
uniform sampler2D ourTexture;
uniform float textureOppacity = 0;

out vec3 color;

void main(){
	vec4 tmp = mix(texture(ourTexture, TexCoord), vec4(fragmentColour, 1), textureOppacity);
	color = vec3(tmp.x, tmp.y, tmp.z);
}