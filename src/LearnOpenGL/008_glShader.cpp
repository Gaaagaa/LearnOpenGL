/**
* @file    008_glShader.cpp
* <pre>
* Copyright (c) 2010-2018, Gaaagaa All rights reserved.
* 
* 文件名称：008_glShader.cpp
* 创建日期：2018年08月18日
* 文件标识：
* 文件摘要：LearnOpenGL 示例程序：着色器。
* 
* 当前版本：1.0.0.0
* 作    者：
* 完成日期：2018年08月18日
* 版本摘要：
* 
* 取代版本：
* 原作者  ：
* 完成日期：
* 版本摘要：
* </pre>
*/

#include <iostream>
#include <thread>
#include <chrono>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW//glfw3.h>

////////////////////////////////////////////////////////////////////////////////

// 
// 前置的数据、函数接口等的声明和定义
// 

/**********************************************************/
/**
* @brief 键盘按键事件通知的回调操作接口。
* 
* @param [in ] window   : 目标窗口对象。
* @param [in ] key      : 按键值。
* @param [in ] scancode : 按键码。
* @param [in ] action   : 按键动作。
* @param [in ] mode     : 当前模式。
* 
* @return void
*         
*/
void key_callback(GLFWwindow * window, int key, int scancode, int action, int mode);

const GLuint WIDTH  = 800;  ///< 窗口绘图客户区宽度
const GLuint HEIGHT = 600;  ///< 窗口绘图客户区高度

///< 着色器
const GLchar * vertexShaderSource =
			"#version 330 core\n"
			"layout (location = 0) in vec3 position;\n"
			"layout (location = 1) in vec3 incolor;\n"
			"out vec3 outcolor;\n"
			"void main()\n"
			"{\n"
				"gl_Position = vec4(position.x, position.y, position.z, 1.0F);"
				"outcolor = incolor;\n"
			"}\0";
const GLchar * fragmentShaderSource =
			"#version 330 core\n"
			"in vec3 outcolor;\n"
			"out vec4 color;\n"
			"void main()\n"
			"{\n"
				"color = vec4(outcolor, 1.0F);\n"
			"}\0";

//====================================================================

// 
// 程序入口函数
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

	GLFWwindow * window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL - 008 - Shader", NULL, NULL);
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
	// Define the viewport dimentions

	int cx = 0;
	int cy = 0;
	glfwGetFramebufferSize(window, &cx, &cy);
	glViewport(0, 0, cx, cy);

	//======================================
	// Build and compile shader program

	// Vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
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
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// check for compile time errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Create program and link shaders
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//======================================
	// Set up vertex data (and buffer(s)) and attribute pointers

	GLfloat vertices[] =
	{
		-0.5F, -0.5F, 0.0F, 1.0F, 0.0F, 0.0F,
		 0.5F, -0.5F, 0.0F, 0.0F, 1.0F, 0.0F,
		 0.0F,  0.5F, 0.0F, 0.0F, 0.0F, 1.0F
	};

	GLuint VAO = 0;
	GLuint VBO = 0;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
	}
	glBindVertexArray(0);

	//======================================
	// Main loop

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		// Render ......
		glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		{
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 3);
			glBindVertexArray(0);
		}

		glfwSwapBuffers(window);

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	//======================================
	// Cleaup and terminate

	glDeleteProgram(shaderProgram);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();

	//======================================

	return 0;
}

//====================================================================

/**********************************************************/
/**
* @brief 键盘按键事件通知的回调操作接口。
*
* @param [in ] window   : 目标窗口对象。
* @param [in ] key      : 按键值。
* @param [in ] scancode : 按键码。
* @param [in ] action   : 按键动作。
* @param [in ] mode     : 当前模式。
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
