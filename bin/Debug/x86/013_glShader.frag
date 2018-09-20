#version 330 core

in  vec3 vtxColor;
in  vec2 vtxCoord;

out vec4 outColor;

uniform sampler2D outTexture;

void main()
{
	outColor = texture(outTexture, vtxCoord) * vec4(vtxColor, 1.0F);
}
