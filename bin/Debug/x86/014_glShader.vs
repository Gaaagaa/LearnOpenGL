#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 lctColor;
layout (location = 2) in vec2 texCoord;

out vec3 vtxColor;
out vec2 vtxCoord;

void main()
{
	gl_Position = vec4(position, 1.0F);
	vtxColor    = lctColor;
	vtxCoord    = vec2(texCoord.x, 1.0F - texCoord.y);
}
