#version 330 core

in  vec3 poscolor;
out vec4 outcolor;

void main()
{
	outcolor = vec4(poscolor, 1.0F);
}
