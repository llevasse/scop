#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 vertexColour;
layout(location = 2) in vec2 textureCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fragmentColour;
out	vec2 TexCoord;

void main(){
	mat4 matrix = projection * model;
	gl_Position = matrix * vec4(position.x,position.y, position.z, 1);
	fragmentColour = vertexColour;
	TexCoord = textureCoord;
}