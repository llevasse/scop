#version 330 core

in vec3 fragmentColour;

out vec3 color;

void main(){
	// color = vec4(1,1,1,1);
	color = fragmentColour;
}