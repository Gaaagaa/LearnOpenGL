/**
* @file    012_glShader.cpp
* <pre>
* Copyright (c) 2010-2018, Gaaagaa All rights reserved.
* 
* �ļ����ƣ�012_glShader.cpp
* �������ڣ�2018��08��20��
* �ļ���ʶ��
* �ļ�ժҪ��LearnOpenGL ʾ��������ɫ����
* 
* ��ǰ�汾��1.0.0.0
* ��    �ߣ�
* ������ڣ�2018��08��20��
* �汾ժҪ��
* 
* ȡ���汾��
* ԭ����  ��
* ������ڣ�
* �汾ժҪ��
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

////////////////////////////////////////////////////////////////////////////////

//====================================================================

// 
// ������ݡ��ӿڵ�ǰ������
// 

/**********************************************************/
/**
* @brief ���ڰ����¼�֪ͨ�Ļص������ӿڡ�
* 
* @param [in ] window   : ���ڶ���
* @param [in ] key      : ����ֵ��
* @param [in ] scancode : �����롣
* @param [in ] action   : ����������
* @param [in ] mode     : ����ģʽ��
* 
* @return void
*         
*/
void key_callback(GLFWwindow * window, int key, int scancode, int action, int mode);

const GLuint WIDTH  = 800;  ///< ���ڻ��ƵĿͻ������
const GLuint HEIGHT = 600;  ///< ���ڻ��ƵĿͻ����߶�

//====================================================================

// 
// �������������
// 

int main(int argc, char * argv[])
{
	int nerror = -1;

	//======================================

	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
	if (argc < 3)
	{
		std::cout << "<usage>\n\t" << argv[0] << " <vertex.vs> <fragment.frag>" << std::endl;
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

	GLFWwindow * window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL - 012 - glShader", NULL, NULL);
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
		 0.5F, -0.5F, 0.0F,  1.0F, 0.0F, 0.0F,
		-0.5F, -0.5F, 0.0F,  0.0F, 1.0F, 0.0F,
		 0.0F,  0.5F, 0.0F,  0.0F, 0.0F, 1.0F
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
	// Program loop

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		// Render ......
		glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(xshader.program_id());
		{
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 3);
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

	glfwTerminate();

	//======================================

	return 0;
}

//====================================================================

/**********************************************************/
/**
* @brief ���ڰ����¼�֪ͨ�Ļص������ӿڡ�
*
* @param [in ] window   : ���ڶ���
* @param [in ] key      : ����ֵ��
* @param [in ] scancode : �����롣
* @param [in ] action   : ����������
* @param [in ] mode     : ����ģʽ��
*
* @return void
*
*/
void key_callback(GLFWwindow * window, int key, int scancode, int action, int mode)
{
	std::cout << "Key : " << key << std::endl;

	if ((GLFW_KEY_ESCAPE == key) && (GLFW_PRESS == action))
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

