#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 vertexColour;

out vec4 fragmentColour;

void main(){
	gl_Position = position;
	fragmentColour = vertexColour;
}