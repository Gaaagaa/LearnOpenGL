/**
* @file    006_glTriangle.cpp
* <pre>
* Copyright (c) 2010-2018, Gaaagaa All rights reserved.
* 
* �ļ����ƣ�006_glTriangle.cpp
* �������ڣ�2018��08��15��
* �ļ���ʶ��
* �ļ�ժҪ��LearnOpenGL ʾ�����򣺻��������Ρ�
* 
* ��ǰ�汾��1.0.0.0
* ��    �ߣ�
* ������ڣ�2018��08��15��
* �汾ժҪ��
* 
* ȡ���汾��
* ԭ����  ��
* ������ڣ�
* �汾ժҪ��
* </pre>
*/

#include <iostream>
#include <thread>
#include <chrono>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

////////////////////////////////////////////////////////////////////////////////

// 
// ǰ�õ����ݡ������ȵ������붨��
// 

/**********************************************************/
/**
* @brief ���̰����¼�֪ͨ�Ļص������ӿڡ�
* 
* @param [in ] window   : Ŀ�괰�ڶ���
* @param [in ] key      : ����ֵ��
* @param [in ] scancode : �����롣
* @param [in ] action   : ����������
* @param [in ] mode     : ��ǰģʽ��
* 
* @return void
*         
*/
void key_callback(GLFWwindow * window, int key, int scancode, int action, int mode);

const GLuint WIDTH  = 800; ///< ���ڻ�ͼ�ͻ������
const GLuint HEIGHT = 600; ///< ���ڻ�ͼ�ͻ����߶�

///< ��ɫ�� 1
const GLchar * vertexShaderSrc1 =
			"#version 330 core\n"
			"layout (location = 0) in vec3 position;\n"
			"void main()\n"
			"{\n"
				"gl_Position = vec4(position.x, position.y, position.z, 1.0F);\n"
			"}\0";
const GLchar * fragmentShaderSrc1 =
			"#version 330 core\n"
			"out vec4 color;\n"
			"void main()\n"
			"{\n"
				"color = vec4(1.0F, 0.5F, 0.2F, 1.0F);\n"
			"}\0";

///< ��ɫ�� 2
const GLchar * vertexShaderSrc2 =
			"#version 330 core\n"
			"layout (location = 0) in vec3 position;\n"
			"void main()\n"
			"{\n"
				"gl_Position = vec4(position.x, position.y, position.z, 1.0F);\n"
			"}\0";
const GLchar * fragmentShaderSrc2 =
			"#version 330 core\n"
			"out vec4 color;\n"
			"void main()\n"
			"{\n"
				"color = vec4(1.0F, 0.2F, 1.0F, 1.0F);\n"
			"}\0";

//====================================================================

// 
// ������ں���
// 

int main(int argc, char * argv[])
{
	GLint  success      = 0;
	GLchar infoLog[512] = { 0 };

	//======================================

	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;

	//======================================
	// Initialize GLFW

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//======================================
	// Create window

	GLFWwindow * window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL - 004 Triangle", NULL, NULL);
	if (NULL == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);

	//======================================
	// Initialize GLEW

	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		glfwTerminate();
		return -1;
	}

	//======================================
	// Define the viewport dimensions

	int cx = 0;
	int cy = 0;
	glfwGetFramebufferSize(window, &cx, &cy);
	glViewport(0, 0, cx, cy);

	//======================================
	// Build and compile shader program1

	// Vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSrc1, NULL);
	glCompileShader(vertexShader);
	// check for compile time errors
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSrc1, NULL);
	glCompileShader(fragmentShader);
	// check for compile time errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "1 ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Create program and link shaders
	GLuint shaderProgram1 = glCreateProgram();
	glAttachShader(shaderProgram1, vertexShader);
	glAttachShader(shaderProgram1, fragmentShader);
	glLinkProgram(shaderProgram1);
	// check for linking errors
	glGetProgramiv(shaderProgram1, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram1, 512, NULL, infoLog);
		std::cout << "1 ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//======================================
	// Build and compile shader program2

	// Vertex shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSrc2, NULL);
	glCompileShader(vertexShader);
	// check for compile time errors
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "2 ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Fragment shader
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSrc2, NULL);
	glCompileShader(fragmentShader);
	// check for compile time errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Create program and link shaders
	GLuint shaderProgram2 = glCreateProgram();
	glAttachShader(shaderProgram2, vertexShader);
	glAttachShader(shaderProgram2, fragmentShader);
	glLinkProgram(shaderProgram2);
	// check for linking errors
	glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram2, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//======================================
	// Set up vertex data (and buffer(s)) and attribute pointers

	GLfloat vertices1[] =
	{
		 0.5F,  0.5F, 0.0F,
		 0.5F, -0.4F, 0.0F,
		-0.4F,  0.5F, 0.0F,

		 0.4F, -0.5F, 0.0F,
		-0.5F, -0.5F, 0.0F,
		-0.5F,  0.4F, 0.0F
	};

	GLfloat vertices2[] =
	{
		 0.4F,  0.4F, 0.0F,
		 0.4F, -0.2F, 0.0F,
		-0.2F,  0.4F, 0.0F,

		 0.2F, -0.4F, 0.0F,
		-0.4F, -0.4F, 0.0F,
		-0.4F,  0.2F, 0.0F
	};

	GLuint VAO[2] = { 0, 0 };
	GLuint VBO[2] = { 0, 0 };

	glGenVertexArrays(2, VAO);
	glGenBuffers(2, VBO);

	glBindVertexArray(VAO[0]);
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
		glEnableVertexAttribArray(0);
	}
	glBindVertexArray(0);

	glBindVertexArray(VAO[1]);
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
		glEnableVertexAttribArray(0);
	}
	glBindVertexArray(0);

	//======================================
	// Main loop

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		// Render ....
		glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw triangle

		// shader program1
		glUseProgram(shaderProgram1);
		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// shader program2
		glUseProgram(shaderProgram2);
		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(window);

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	//======================================
	// Cleanup and terminate

	glDeleteProgram(shaderProgram1);
	glDeleteProgram(shaderProgram2);
	glDeleteVertexArrays(2, VAO);
	glDeleteBuffers(2, VBO);

	glfwTerminate();

	//======================================

	return 0;
}

//====================================================================

/**********************************************************/
/**
* @brief ���̰����¼�֪ͨ�Ļص������ӿڡ�
*
* @param [in ] window   : Ŀ�괰�ڶ���
* @param [in ] key      : ����ֵ��
* @param [in ] scancode : �����롣
* @param [in ] action   : ����������
* @param [in ] mode     : ��ǰģʽ��
*
* @return void
*
*/
void key_callback(GLFWwindow * window, int key, int scancode, int action, int mode)
{
	std::cout << "key : " << key << std::endl;

	if ((GLFW_KEY_ESCAPE == key) && (GLFW_PRESS == action))
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}



