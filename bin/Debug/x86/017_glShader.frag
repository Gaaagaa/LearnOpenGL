#version 330 core

in  vec3 vtxColor;
in  vec2 vtxCoord;

out vec4 outColor;

uniform sampler2D outTexture1;
uniform sampler2D outTexture2;
uniform float     alphaValue;

void main()
{
	outColor = mix(texture(outTexture1, vtxCoord), texture(outTexture2, vtxCoord), alphaValue);
}
