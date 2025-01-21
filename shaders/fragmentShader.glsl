#version 330 core

in vec3 fragmentColour;
in vec2	TexCoord;
uniform sampler2D ourTexture;

out vec3 color;

void main(){
	color = fragmentColour;
}