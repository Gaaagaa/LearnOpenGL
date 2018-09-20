#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 vtxcolor;

out vec3 poscolor;

uniform float xoffset;

void main()
{
	gl_Position = vec4(position.x + xoffset, position.y, position.z, 1.0F);
	poscolor    = vtxcolor;
}
