#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 vertexColour;

out vec3 fragmentColour;

void main(){
	gl_Position = vec4(position.x,position.y, 0, 1);
	fragmentColour = vertexColour;
}