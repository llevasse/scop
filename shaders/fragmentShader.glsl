#version 330 core

in vec4 fragmentColour;

out vec4 color;

void main(){
	// color = vec4(1,1,1,1);
	color = fragmentColour;
}