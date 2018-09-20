/**
* @file    001_glHello.cpp
* <pre>
* Copyright (c) 2010-2018, Gaaagaa All rights reserved.
* 
* 文件名称：001_glHello.cpp
* 创建日期：2018年08月14日
* 文件标识：
* 文件摘要：LearnOpenGL 示例程序：基本流程。
* 
* 当前版本：1.0.0.0
* 作    者：
* 完成日期：2018年08月14日
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
#include <GLFW/glfw3.h>

////////////////////////////////////////////////////////////////////////////////

//====================================================================

// 
// 相关数据、接口等的前置声明
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

const GLuint WIDTH  = 800;  ///< 窗口绘图客户区宽度
const GLuint HEIGHT = 600;  ///< 窗口绘图客户区高度

//====================================================================

// 
// 程序入口主函数
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
	std::cout << "Key : " << key << std::endl;

	if ((GLFW_KEY_ESCAPE == key) && (GLFW_PRESS == action))
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

