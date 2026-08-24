#version 420 core 

in vec2 TexCoords;
in vec3 Normal;
in vec4 lightSpaceFragPos;

uniform sampler2D moonDiffuse;
uniform sampler2D depthMap;
uniform vec3 lightDirection;

layout(location = 0) out vec4 outColor;

float calculateShadow(vec4 fragPositionLightSpace) {
 
  vec3 projCoords = fragPositionLightSpace.xyz / fragPositionLightSpace.w;
  projCoords = projCoords * 0.5 + 0.5;
  float closestDepthValue = texture(depthMap, projCoords.xy).r;
  float currentDepth = projCoords.z;

  if (currentDepth > 1.0)
    return 0.0f;

  float bias = max(0.001 * (1.0 - dot(Normal, lightDirection)), 0.0001);
  float shadow = 0.0;
  vec2 texelSize = 1.0 / textureSize(depthMap, 0);
  for (int x = -1; x <= 1; ++x) {
    for (int y = -1; y <= 1; ++y) {
      float pcfDepth = texture(depthMap, projCoords.xy + vec2(x, y) * texelSize).r;
      shadow += (currentDepth - bias > pcfDepth) ? 1.0 : 0.0;
    }
  }
  shadow /= 9.0;

  return shadow;
}

void main() {
  float diff = max(dot(Normal, lightDirection), 0.0);
  float ambient = 0.01;
  float shadow = calculateShadow(lightSpaceFragPos);
  outColor = vec4(texture(moonDiffuse, TexCoords).rgb * (ambient + (1.0 - shadow) * diff), 1.0);
}
