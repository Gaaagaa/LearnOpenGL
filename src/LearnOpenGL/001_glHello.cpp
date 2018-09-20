/**
* @file    001_glHello.cpp
* <pre>
* Copyright (c) 2010-2018, Gaaagaa All rights reserved.
* 
* �ļ����ƣ�001_glHello.cpp
* �������ڣ�2018��08��14��
* �ļ���ʶ��
* �ļ�ժҪ��LearnOpenGL ʾ�����򣺻������̡�
* 
* ��ǰ�汾��1.0.0.0
* ��    �ߣ�
* ������ڣ�2018��08��14��
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

//====================================================================

// 
// ������ݡ��ӿڵȵ�ǰ������
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

const GLuint WIDTH  = 800;  ///< ���ڻ�ͼ�ͻ������
const GLuint HEIGHT = 600;  ///< ���ڻ�ͼ�ͻ����߶�

//====================================================================

// 
// �������������
// 

int main(int argc, char * argv[])
{
	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;

	//======================================
	// GLFW initialize

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//======================================
	// create window

	GLFWwindow * window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL - 001_Hello", NULL, NULL);
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
	// Program loop

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		// Render......
		glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
		glClear(GL_COLOR_BUFFER_BIT);

		// Swap the screen buffers
		glfwSwapBuffers(window);

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	//======================================
	// Terminate

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

