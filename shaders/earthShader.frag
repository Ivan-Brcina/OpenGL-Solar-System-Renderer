#version 420 core

in VS_OUT {
  vec2 TexCoords;
  vec3 WorldFragPos;
  vec4 LightSpaceFragPos;
  vec3 WorldLightDirection;
  vec3 WorldNormal;
} fs_in;

uniform sampler2D dayDiffuse;
uniform sampler2D specularMap;
uniform sampler2D nightDiffuse;
uniform sampler2D depthMap;
uniform vec3 cameraPos;

out vec4 outColor;

float calculateShadow(vec4 fragPositionLightSpace) {
  vec3 projCoords = fragPositionLightSpace.xyz / fragPositionLightSpace.w;
  projCoords = projCoords * 0.5 + 0.5;
  float currentDepth = projCoords.z;

  if (currentDepth > 1.0)
    return 0.0;

  float bias = max(0.001 * (1.0 - dot(fs_in.WorldNormal, fs_in.WorldLightDirection)), 0.0001);

  float shadow = 0.0;
  vec2 texelSize = 1.0 / textureSize(depthMap, 0);

  int range = 3;
  for (int x = -range; x <= range; ++x)
      for (int y = -range; y <= range; ++y) {
        float pcfDepth = texture(depthMap, projCoords.xy + vec2(x, y) * texelSize).r;
        shadow += (currentDepth - bias > pcfDepth) ? 1.0 : 0.0;
      }
  shadow /= float((2*range+1) * (2*range+1));

  return shadow;
}

void main() {
  vec3 N = normalize(fs_in.WorldNormal);
  vec3 L = normalize(fs_in.WorldLightDirection);

  float NdotL = dot(N, L);
  float dayFactor = smoothstep(-0.2, 0.2, NdotL);

  float shadow = calculateShadow(fs_in.LightSpaceFragPos);

  vec3 dayColor = texture(dayDiffuse, fs_in.TexCoords).rgb;
  vec3 nightColor = texture(nightDiffuse, fs_in.TexCoords).rgb;
  vec3 surfaceColor;

  if(shadow >= 1.0) {
    surfaceColor = nightColor;
  } else {
    surfaceColor = mix(nightColor, dayColor, dayFactor);
  }

  float ambientStrength = mix(0.4, 0.01, dayFactor);
  vec3 ambient = ambientStrength * vec3(1.0);

  float diff = max(NdotL, 0.0);
  vec3 diffuse = diff * vec3(1.0);

  vec3 viewDir = normalize(cameraPos - fs_in.WorldFragPos);
  vec3 halfwayDir = normalize(L + viewDir);
  float specAmount = pow(max(dot(N, halfwayDir), 0.0), 128.0);
  float specularColor = texture(specularMap, fs_in.TexCoords).r;
  vec3 specular = specAmount * specularColor * vec3(1.0) * step(0.0, diff);

  vec3 lighting = (ambient + (1.0 - shadow) * (diffuse )) * surfaceColor + specular * (1.0 - shadow);

  outColor = vec4(lighting, 1.0);
}

