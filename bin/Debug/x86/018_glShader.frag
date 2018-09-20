#version 330 core

in  vec3 vtxColor;
in  vec2 vtxCoord;

out vec4 outColor;

uniform sampler2D outTexture1;

void main()
{
	outColor = texture(outTexture1, vtxCoord) * vec4(vtxColor, 1.0F);
}
