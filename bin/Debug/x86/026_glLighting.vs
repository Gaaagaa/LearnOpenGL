#version 330 core

layout (location = 0) in vec3 vPosition;

uniform mat4 mtx4M = mat4(1.0F, 0.0F, 0.0F, 0.0F,
						  0.0F, 1.0F, 0.0F, 0.0F,
						  0.0F, 0.0F, 1.0F, 0.0F,
						  0.0F, 0.0F, 0.0F, 1.0F);
uniform mat4 mtx4V = mat4(1.0F, 0.0F, 0.0F, 0.0F,
						  0.0F, 1.0F, 0.0F, 0.0F,
						  0.0F, 0.0F, 1.0F, 0.0F,
						  0.0F, 0.0F, 0.0F, 1.0F);
uniform mat4 mtx4P = mat4(1.0F, 0.0F, 0.0F, 0.0F,
						  0.0F, 1.0F, 0.0F, 0.0F,
						  0.0F, 0.0F, 1.0F, 0.0F,
						  0.0F, 0.0F, 0.0F, 1.0F);

void main()
{
	gl_Position = mtx4P * mtx4V * mtx4M * vec4(vPosition, 1.0F);
}
