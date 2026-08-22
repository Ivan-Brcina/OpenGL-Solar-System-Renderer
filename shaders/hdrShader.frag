#version 420 core 

in vec3 worldFragmentPos;
in vec2 TexCoords;

uniform sampler2D sunTex;
uniform float emissionStrength;
uniform vec3 cameraPos;

layout(location = 0) out vec4 outColor;
layout(location = 1) out vec4 brightColor;


void main() {

  float distanceFromSun = distance(cameraPos, worldFragmentPos);
  float whitenessFactor = smoothstep(50.0, 80.0, distanceFromSun);

  vec3 sunColor = texture(sunTex,TexCoords).rgb;
  vec3 white = vec3(2.0);
  vec3 finalColor = mix(sunColor, white, whitenessFactor) * emissionStrength;
  outColor = vec4(finalColor, 1.0);

  float brightness = dot(outColor.rgb, vec3(0.2126, 0.7152, 0.0722));

  if(brightness > 1.0) {
    brightColor = outColor;
  } else {
    brightColor = vec4(0.0,0.0,0.0,1.0);
  }
}
