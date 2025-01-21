#version 330 core

in vec3 fragmentColour;
in vec2	TexCoord;
uniform sampler2D ourTexture;

out vec3 color;

void main(){
	// color = vec4(1,1,1,1);
	// color = fragmentColour;
	vec4 tmp = texture(ourTexture, TexCoord);
	color = vec3(tmp.x, tmp.y, tmp.z);
}