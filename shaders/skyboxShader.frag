#version 420 core 

in vec3 TexCoords;

uniform samplerCube skybox;

layout(location = 0) out vec4 outColor;

void main() {
  outColor = texture(skybox, TexCoords);
}
