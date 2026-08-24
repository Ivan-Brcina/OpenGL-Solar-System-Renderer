#version 420 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;
uniform mat4 lightSpaceMatrix;
uniform vec3 lightDirection;

out VS_OUT {
  vec2 TexCoords;
  vec3 WorldFragPos;
  vec4 LightSpaceFragPos;
  vec3 WorldLightDirection;
  vec3 WorldNormal;
} vs_out;

void main() {
  vs_out.TexCoords = aTexCoords;
  vs_out.WorldFragPos = vec3(model * vec4(aPos, 1.0));
  vs_out.LightSpaceFragPos = lightSpaceMatrix * model * vec4(aPos, 1.0);
  vs_out.WorldLightDirection = lightDirection;
  vs_out.WorldNormal = normalize(normalMatrix * aNormal);
  gl_Position = projection * view * model * vec4(aPos, 1.0);
}
