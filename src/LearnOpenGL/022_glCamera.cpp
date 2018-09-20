/**
* @file    022_glCamera.cpp
* <pre>
* Copyright (c) 2010-2018, Gaaagaa All rights reserved.
*
* 文件名称：022_glCamera.cpp
* 创建日期：2018年08月21日
* 文件标识：
* 文件摘要：LearnOpenGL 示例程序：摄像机。
*
* 当前版本：1.0.0.0
* 作    者：
* 完成日期：2018年08月21日
* 版本摘要：
*
* 取代版本：
* 原作者  ：
* 完成日期：
* 版本摘要：
* </pre>
*/

#include "vxGLShader.h"

#include <iostream>
#include <thread>
#include <chrono>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// SOIL
#include <SOIL/SOIL.h>

// GLM
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

////////////////////////////////////////////////////////////////////////////////

//====================================================================

// 
// 相关数据、接口的前置声明
// 

/**********************************************************/
/**
* @brief 窗口按键事件通知的回调函数接口。
*
* @param [in ] window   : 窗口对象。
* @param [in ] key      : 按键值。
* @param [in ] scancode : 按键码。
* @param [in ] action   : 按键动作。
* @param [in ] mode     : 按键模式。
*
* @return void
*
*/
void key_callback(GLFWwindow * window, int key, int scancode, int action, int mode);

const GLuint WIDTH  = 800;  ///< 窗口绘图客户区的宽度
const GLuint HEIGHT = 600;  ///< 窗口绘图客户区的高度

//====================================================================

// 
// 程序入口主函数
// 

int main(int argc, char * argv[])
{
	int nerror = -1;

	//======================================

	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
	if (argc < 4)
	{
		std::cout << "<usage>\n\t" << argv[0] << " <vertex.vs> <fragment.frag> <texture.jpg>" << std::endl;
		return -1;
	}

	//======================================
	// GLFW initialize

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//======================================
	// create window

	GLFWwindow * window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL - 022 - glCamera", NULL, NULL);
	if (NULL == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);

	//======================================
	// GLEW initialize

	glewExperimental = GL_FALSE;
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
	// Create shader object

	char * xerrlog = NULL;

	vxGLShader xshader;
	nerror = xshader.create(argv[1], argv[2], &xerrlog);
	if (0 != nerror)
	{
		std::cout << "create shader error : " << nerror << std::endl;
		if (NULL != xerrlog)
		{
			std::cout << "error information : \n" << xerrlog << std::endl;
			free(xerrlog);
			xerrlog = NULL;
		}

		glfwTerminate();
		return -1;
	}

	//======================================
	// Set up vertex data (and buffer(s)) and attribute pointers

	GLfloat vertices[] =
	{
		// --- Positions --- Texture Coords ---
		-0.5F, -0.5F, -0.5F,  0.0F, 0.0F,
		 0.5F, -0.5F, -0.5F,  1.0F, 0.0F,
		 0.5F,  0.5F, -0.5F,  1.0F, 1.0F,
		 0.5F,  0.5F, -0.5F,  1.0F, 1.0F,
		-0.5F,  0.5F, -0.5F,  0.0F, 1.0F,
		-0.5F, -0.5F, -0.5F,  0.0F, 0.0F,

		-0.5F, -0.5F,  0.5F,  0.0F, 0.0F,
		 0.5F, -0.5F,  0.5F,  1.0F, 0.0F,
		 0.5F,  0.5F,  0.5F,  1.0F, 1.0F,
		 0.5F,  0.5F,  0.5F,  1.0F, 1.0F,
		-0.5F,  0.5F,  0.5F,  0.0F, 1.0F,
		-0.5F, -0.5F,  0.5F,  0.0F, 0.0F,

		-0.5F,  0.5F,  0.5F,  1.0F, 0.0F,
		-0.5F,  0.5F, -0.5F,  1.0F, 1.0F,
		-0.5F, -0.5F, -0.5F,  0.0F, 1.0F,
		-0.5F, -0.5F, -0.5F,  0.0F, 1.0F,
		-0.5F, -0.5F,  0.5F,  0.0F, 0.0F,
		-0.5F,  0.5F,  0.5F,  1.0F, 0.0F,

		 0.5F,  0.5F,  0.5F,  1.0F, 0.0F,
		 0.5F,  0.5F, -0.5F,  1.0F, 1.0F,
		 0.5F, -0.5F, -0.5F,  0.0F, 1.0F,
		 0.5F, -0.5F, -0.5F,  0.0F, 1.0F,
		 0.5F, -0.5F,  0.5F,  0.0F, 0.0F,
		 0.5F,  0.5F,  0.5F,  1.0F, 0.0F,

		-0.5F, -0.5F, -0.5F,  0.0F, 1.0F,
		 0.5F, -0.5F, -0.5F,  1.0F, 1.0F,
		 0.5F, -0.5F,  0.5F,  1.0F, 0.0F,
		 0.5F, -0.5F,  0.5F,  1.0F, 0.0F,
		-0.5F, -0.5F,  0.5F,  0.0F, 0.0F,
		-0.5F, -0.5F, -0.5F,  0.0F, 1.0F,

		-0.5F,  0.5F, -0.5F,  0.0F, 1.0F,
		 0.5F,  0.5F, -0.5F,  1.0F, 1.0F,
		 0.5F,  0.5F,  0.5F,  1.0F, 0.0F,
		 0.5F,  0.5F,  0.5F,  1.0F, 0.0F,
		-0.5F,  0.5F,  0.5F,  0.0F, 0.0F,
		-0.5F,  0.5F, -0.5F,  0.0F, 1.0F
	};

	GLuint VAO = 0;
	GLuint VBO = 0;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)0);
		glEnableVertexAttribArray(0);
		// TexCoord attribute
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
	}
	glBindVertexArray(0);

	//======================================
	// Load and create a texture

	GLuint texture = 0;
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);
	{
		// Set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// Set the texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		// Load image, create texture and generate mipmaps
		GLubyte * image = SOIL_load_image(argv[3], &cx, &cy, NULL, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, cx, cy, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	//======================================

	glm::mat4 mtxIdentity(1.0F);
	glm::mat4 mtx4P = glm::perspective(glm::radians(45.0F), ((GLfloat)WIDTH) / ((GLfloat)HEIGHT), 0.1F, 100.0F);

	glm::vec3 cubePositions[] =
	{
		glm::vec3( 0.0F,  0.0F,  0.0F),
		glm::vec3( 2.0F,  5.0F, -15.0F),
		glm::vec3(-1.5F, -2.2F, -2.5F),
		glm::vec3(-3.8F, -2.0F, -12.3F),
		glm::vec3( 2.4F, -0.4F, -3.5F),
		glm::vec3(-1.7F,  3.0F, -7.5F),
		glm::vec3( 1.3F, -2.0F, -2.5F),
		glm::vec3( 1.5F,  2.0F, -2.5F),
		glm::vec3( 1.5F,  0.2F, -1.5F),
		glm::vec3(-1.3F,  1.0F, -1.5F)
	};

	//======================================
	// Program loop

	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		// Render ......
		glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindTexture(GL_TEXTURE_2D, texture);

		glUseProgram(xshader.program_id());
		{
			GLfloat   xtime = (GLfloat)glfwGetTime();
			GLfloat   camX  = (GLfloat)sin(xtime) * 10.0F;
			GLfloat   camY  = (GLfloat)cos(xtime) * 10.0F;
			glm::mat4 mtx4V = glm::lookAt(glm::vec3(camX, 0.0F, camY),
										  glm::vec3(0.0F, 0.0F, 0.0F),
										  glm::vec3(0.0F, 1.0F, 0.0F));

			glUniformMatrix4fv(glGetUniformLocation(xshader.program_id(), "mtx4V"), 1, GL_FALSE, glm::value_ptr(mtx4V));
			glUniformMatrix4fv(glGetUniformLocation(xshader.program_id(), "mtx4P"), 1, GL_FALSE, glm::value_ptr(mtx4P));

			glBindVertexArray(VAO);

			for (GLuint i = 0; i < 10; ++i)
			{
				glm::mat4 mtx4M;
				mtx4M = glm::translate(mtxIdentity, cubePositions[i]);
				mtx4M = glm::rotate(mtx4M, glm::radians(i * 10.0F) + xtime * i, glm::vec3(1.0F, 0.3F, 0.5F));
				glUniformMatrix4fv(glGetUniformLocation(xshader.program_id(), "mtx4M"), 1, GL_FALSE, glm::value_ptr(mtx4M));

				glDrawArrays(GL_TRIANGLES, 0, 36);
			}

			glBindVertexArray(0);
		}

		// Swap the screen buffers
		glfwSwapBuffers(window);

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	//======================================
	// Cleanup and terminate

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteTextures(1, &texture);

	glfwTerminate();

	//======================================

	return 0;
}

//====================================================================

/**********************************************************/
/**
* @brief 窗口按键事件通知的回调函数接口。
*
* @param [in ] window   : 窗口对象。
* @param [in ] key      : 按键值。
* @param [in ] scancode : 按键码。
* @param [in ] action   : 按键动作。
* @param [in ] mode     : 按键模式。
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

