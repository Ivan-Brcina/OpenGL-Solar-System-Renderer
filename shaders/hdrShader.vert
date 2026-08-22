#version 420 core 

layout(location = 0) in vec3 aPos;
layout(location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 worldFragmentPos;

out vec2 TexCoords;

void main() {
  TexCoords = aTexCoords;
  worldFragmentPos = vec3(model * vec4(aPos,1.0));
  gl_Position = projection * view * model * vec4(aPos, 1.0);
}
