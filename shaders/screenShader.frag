#version 420 core 

in vec2 TexCoords;
out vec4 outColor;

uniform sampler2D hdrImage;
uniform sampler2D blurImage;
uniform float exposure;

void main() {
  float gamma    = 2.2;
  vec3 hdrColor  = texture(hdrImage, TexCoords).rgb;
  vec3 blurColor = texture(blurImage, TexCoords).rgb;
  hdrColor      += blurColor;
  vec3 result    = vec3(1.0) - exp(-hdrColor * exposure);
  result         = pow(result, vec3(1.0 / gamma));
  outColor       = vec4(result,1.0);
}
