#version 460 core
out vec4 fragColor;
in vec2 TexCoords;
uniform sampler2D ourTexture;
void main(){
	fragColor = texture(ourTexture, TexCoords);
}
