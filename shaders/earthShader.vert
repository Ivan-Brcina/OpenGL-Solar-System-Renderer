// #version 420 core
//
// layout(location = 0) in vec3 aPos;
// layout(location = 1) in vec3 aNormal;
// layout(location = 2) in vec2 aTexCoords;
// layout(location = 3) in vec3 aTangent;
//
// uniform mat4 model;
// uniform mat4 view;
// uniform mat4 projection;
// uniform mat3 normalMatrix;
// uniform vec3 lightDirection;
// uniform vec3 cameraPos;
// uniform mat4 lightSpaceMatrix;
//
// out VS_OUT {
//   vec2 TexCoords;
//   vec3 TangentFragPos;
//   vec3 TangentCameraPos;
//   vec3 TangentLightDir;
//   vec4 LightSpaceFragPos;
//   vec3 WorldLightDirection;
//   vec3 WorldNormal;
// } vs_out;
//
// void main() {
//
//   vec3 T = normalize(vec3(model * vec4(aTangent,0.0)));
//   vec3 N = normalize(normalMatrix * aNormal);
//
//   T = normalize(T - dot(T,N) * N);
//
//   vec3 B = cross(T,N);
//   mat3 TBN = transpose(mat3(T,B,N));
//
//   vs_out.TexCoords = aTexCoords;
//   vs_out.TangentFragPos = TBN * vec3(model * vec4(aPos,1.0));
//   vs_out.TangentCameraPos = TBN * cameraPos;
//   vs_out.TangentLightDir = normalize(TBN * lightDirection);
//
//   vs_out.LightSpaceFragPos = lightSpaceMatrix * model * vec4(aPos,1.0);
//   vs_out.WorldLightDirection = lightDirection;
//   vs_out.WorldNormal = normalMatrix * aNormal;
//
//   gl_Position = projection * view * model * vec4(aPos, 1.0f);
// }
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
