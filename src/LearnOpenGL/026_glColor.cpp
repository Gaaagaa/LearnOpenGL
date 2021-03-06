/**
* @file    026_glColor.cpp
* <pre>
* Copyright (c) 2018, Gaaagaa All rights reserved.
* 
* 文件名称：026_glColor.cpp
* 创建日期：2018年09月21日
* 文件标识：
* 文件摘要：LearnOpenGL 示例程序：颜色。
* 
* 当前版本：1.0.0.0
* 作    者：
* 完成日期：2018年09月21日
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

const GLuint WIDTH  = 800;  ///< 窗口绘图客户区的宽度
const GLuint HEIGHT = 600;  ///< 窗口绘图客户区的高度

/**
* @struct glmCameraParam
* @brief  摄像机位置控制参数。
*/
typedef struct glmCameraParam
{
	GLfloat    cameraSpeed;  ///< 摄像机移动速度
	glm::vec3  cameraPos;    ///< 摄像机位置向量
	glm::vec3  cameraFront;  ///< 摄像机方向向量（方向向量 = cameraPos + cameraFront）
	glm::vec3  cameraUp;     ///< 摄像机向上向量

	GLfloat    mouseXPos;    ///< 记录鼠标移动的水平位置
	GLfloat    mouseYPos;    ///< 记录鼠标移动的垂直位置
	GLfloat    viewPitch;    ///< 视图俯仰角
	GLfloat    viewYaw;      ///< 视图偏航角
	GLfloat    viewZoom;     ///< 视图缩放量
} glmCameraParam;

/**********************************************************/
/**
* @brief 修改摄像机位置控制参数中的视图偏移相关字段值。
*
* @param [in ] xCamera : 摄像机位置控制参数。
* @param [in ] xpos    : 水平坐标。
* @param [in ] ypos    : 垂直坐标。
*
* @return void
*
*/
void view_offset(glmCameraParam * xCamera, double xpos, double ypos);

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

/**********************************************************/
/**
* @brief 窗口按键事件通知的回操作时，调用该接口，
*        更新摄像机位置控制参数，实现画面移动效果；
*        该接口仅由 key_callback() 调用。
* 
* @param [in ] window   : 窗口对象。
* @param [in ] key      : 按键值。
* @param [in ] scancode : 按键码。
* @param [in ] action   : 按键动作。
* @param [in ] mode     : 按键模式。
* 
* @return bool
*         - 返回 true ，表示按键消息被有效处理；
*         - 返回 false，表示按键消息未被处理。
*/
bool key_movement(GLFWwindow * window, int key, int scancode, int action, int mode);

/**********************************************************/
/**
* @brief 窗口鼠标事件通知的回调函数接口。
* 
* @param [in ] window : 窗口对象。
* @param [in ] xpos   : 水平坐标。
* @param [in ] ypos   : 垂直坐标。
* 
* @return void
*         
*/
void mouse_callback(GLFWwindow * window, double xpos, double ypos);

/**********************************************************/
/**
* @brief 窗口鼠标滚轮事件通知的回调函数接口。
*
* @param [in ] window  : 窗口对象。
* @param [in ] xoffset : 水平偏移量。
* @param [in ] yoffset : 垂直偏移量。
*
* @return void
*
*/
void scroll_callback(GLFWwindow * window, double xoffset, double yoffset);

//====================================================================

// 
// 程序入口主函数
// 

int main(int argc, char * argv[])
{
	int nerror = -1;

	//======================================

	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
	if (argc < 5)
	{
		std::cout << "<usage>\n\t" << argv[0] << " <Lighting.vs> <Lighting.frag> <Lamp.vs> <Lamp.frag>" << std::endl;
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

	GLFWwindow * window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL - 026 - glColor", NULL, NULL);
	if (NULL == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// glfwSetCursorPosCallback(window, &mouse_callback);
	glfwSetKeyCallback(window, &key_callback);
	glfwSetScrollCallback(window, &scroll_callback);

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
	// Create shader objects

	char * xerrlog = NULL;

	vxGLShader xLightShader;
	nerror = xLightShader.create(argv[1], argv[2], &xerrlog);
	if (0 != nerror)
	{
		std::cout << "create shader_light error : " << nerror << std::endl;
		if (NULL != xerrlog)
		{
			std::cout << "error information : \n" << xerrlog << std::endl;
			free(xerrlog);
			xerrlog = NULL;
		}

		glfwTerminate();
		return -1;
	}

	vxGLShader xLampShader;
	nerror = xLampShader.create(argv[3], argv[4], &xerrlog);
	if (0 != nerror)
	{
		std::cout << "create shader_lamp error : " << nerror << std::endl;
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
		// --- Positions ---
		-0.5F, -0.5F, -0.5F,
		 0.5F, -0.5F, -0.5F,
		 0.5F,  0.5F, -0.5F,
		 0.5F,  0.5F, -0.5F,
		-0.5F,  0.5F, -0.5F,
		-0.5F, -0.5F, -0.5F,

		-0.5F, -0.5F,  0.5F,
		 0.5F, -0.5F,  0.5F,
		 0.5F,  0.5F,  0.5F,
		 0.5F,  0.5F,  0.5F,
		-0.5F,  0.5F,  0.5F,
		-0.5F, -0.5F,  0.5F,

		-0.5F,  0.5F,  0.5F,
		-0.5F,  0.5F, -0.5F,
		-0.5F, -0.5F, -0.5F,
		-0.5F, -0.5F, -0.5F,
		-0.5F, -0.5F,  0.5F,
		-0.5F,  0.5F,  0.5F,

		 0.5F,  0.5F,  0.5F,
		 0.5F,  0.5F, -0.5F,
		 0.5F, -0.5F, -0.5F,
		 0.5F, -0.5F, -0.5F,
		 0.5F, -0.5F,  0.5F,
		 0.5F,  0.5F,  0.5F,

		-0.5F, -0.5F, -0.5F,
		 0.5F, -0.5F, -0.5F,
		 0.5F, -0.5F,  0.5F,
		 0.5F, -0.5F,  0.5F,
		-0.5F, -0.5F,  0.5F,
		-0.5F, -0.5F, -0.5F,

		-0.5F,  0.5F, -0.5F,
		 0.5F,  0.5F, -0.5F,
		 0.5F,  0.5F,  0.5F,
		 0.5F,  0.5F,  0.5F,
		-0.5F,  0.5F,  0.5F,
		-0.5F,  0.5F, -0.5F
	};

	GLuint lightVAO = 0;
	GLuint cubeVBO  = 0;
	GLuint lampVAO  = 0;

	glGenBuffers(1, &cubeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	{
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	{
		// Position attribute
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	glBindVertexArray(0);

	glGenVertexArrays(1, &lampVAO);
	glBindVertexArray(lampVAO);
	{
		// Position attribute
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	glBindVertexArray(0);

	//======================================

	glm::mat4 mtxIdentity(1.0F);

	glmCameraParam xCameraParam;
	xCameraParam.cameraSpeed = 0.1F;
	xCameraParam.cameraPos   = glm::vec3(0.0F, 0.0F,  5.0F);
	xCameraParam.cameraFront = glm::vec3(0.0F, 0.0F, -1.0F);
	xCameraParam.cameraUp    = glm::vec3(0.0F, 1.0F,  0.0F);
	xCameraParam.mouseXPos   = cx / 2.0F;
	xCameraParam.mouseYPos   = cy / 2.0F;
	xCameraParam.viewPitch   = 0.0F;
	xCameraParam.viewYaw     = -90.0F;
	xCameraParam.viewZoom   = 45.0F;
	glfwSetWindowUserPointer(window, &xCameraParam);

	view_offset(&xCameraParam, xCameraParam.mouseXPos, xCameraParam.mouseYPos);

	//======================================

	// Light attributes
	glm::vec3 v3LampPosition(1.2F, 1.0F, 2.0F);

	GLfloat deltaTime  = 0.0F;
	GLfloat updateTime = (GLfloat)glfwGetTime();

	//======================================
	// Program loop

	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		//======================================

		// Render ......
		glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(xLightShader.program_id());
		{
			glUniform3f(glGetUniformLocation(xLightShader.program_id(), "objectColor"), 1.0F, 0.5F, 0.31F);
			glUniform3f(glGetUniformLocation(xLightShader.program_id(), "lightColor" ), 1.0F, 0.5F, 1.0F );

			glm::mat4 mtx4M = glm::translate(mtxIdentity, glm::vec3(0.0F, 0.0F, 0.0F));
			glm::mat4 mtx4V = glm::lookAt(xCameraParam.cameraPos, xCameraParam.cameraPos + xCameraParam.cameraFront, xCameraParam.cameraUp);
			glm::mat4 mtx4P = glm::perspective(xCameraParam.viewZoom, ((GLfloat)WIDTH) / ((GLfloat)HEIGHT), 0.1F, 100.0F);

			glUniformMatrix4fv(glGetUniformLocation(xLightShader.program_id(), "mtx4M"), 1, GL_FALSE, glm::value_ptr(mtx4M));
			glUniformMatrix4fv(glGetUniformLocation(xLightShader.program_id(), "mtx4V"), 1, GL_FALSE, glm::value_ptr(mtx4V));
			glUniformMatrix4fv(glGetUniformLocation(xLightShader.program_id(), "mtx4P"), 1, GL_FALSE, glm::value_ptr(mtx4P));

			glBindVertexArray(lightVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
		}

		glUseProgram(xLampShader.program_id());
		{
			glm::mat4 mtx4M = glm::scale(glm::translate(mtxIdentity, v3LampPosition), glm::vec3(0.2F));
			glm::mat4 mtx4V = glm::lookAt(xCameraParam.cameraPos, xCameraParam.cameraPos + xCameraParam.cameraFront, xCameraParam.cameraUp);
			glm::mat4 mtx4P = glm::perspective(xCameraParam.viewZoom, ((GLfloat)WIDTH) / ((GLfloat)HEIGHT), 0.1F, 100.0F);

			glUniformMatrix4fv(glGetUniformLocation(xLampShader.program_id(), "mtx4M"), 1, GL_FALSE, glm::value_ptr(mtx4M));
			glUniformMatrix4fv(glGetUniformLocation(xLampShader.program_id(), "mtx4V"), 1, GL_FALSE, glm::value_ptr(mtx4V));
			glUniformMatrix4fv(glGetUniformLocation(xLampShader.program_id(), "mtx4P"), 1, GL_FALSE, glm::value_ptr(mtx4P));

			glBindVertexArray(lampVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
		}

		// Swap the screen buffers
		glfwSwapBuffers(window);

		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		//======================================
		// speed

		GLfloat timeValue = (GLfloat)glfwGetTime();
		deltaTime  = timeValue - updateTime;
		updateTime = timeValue;

		xCameraParam.cameraSpeed = 5.0F * deltaTime;

		//======================================
	}

	//======================================
	// Cleanup and terminate

	glDeleteVertexArrays(1, &lightVAO);
	glDeleteVertexArrays(1, &lampVAO);
	glDeleteBuffers(1, &cubeVBO);

	glfwTerminate();

	//======================================

	return 0;
}

//====================================================================


/**********************************************************/
/**
* @brief 修改摄像机位置控制参数中的视图偏移相关字段值。
* 
* @param [in ] xCamera : 摄像机位置控制参数。
* @param [in ] xpos    : 水平坐标。
* @param [in ] ypos    : 垂直坐标。
* 
* @return void
*         
*/
void view_offset(glmCameraParam * xCamera, double xpos, double ypos)
{
	GLfloat xOffset = (GLfloat)xpos - xCamera->mouseXPos;
	GLfloat yOffset = xCamera->mouseYPos - (GLfloat)ypos;
	xCamera->mouseXPos = (GLfloat)xpos;
	xCamera->mouseYPos = (GLfloat)ypos;

	const GLfloat sensitivity = 0.05F;
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	xCamera->viewYaw   += xOffset;
	xCamera->viewPitch += yOffset;

	if (xCamera->viewPitch > 89.0F)
		xCamera->viewPitch = 89.0F;
	else if (xCamera->viewPitch < -89.0F)
		xCamera->viewPitch = -89.0F;

	glm::vec3 cameraFront;
	cameraFront.x = cos(glm::radians(xCamera->viewYaw)) * cos(glm::radians(xCamera->viewPitch));
	cameraFront.y = sin(glm::radians(xCamera->viewPitch));
	cameraFront.z = sin(glm::radians(xCamera->viewYaw)) * cos(glm::radians(xCamera->viewPitch));
	xCamera->cameraFront = glm::normalize(cameraFront);
}

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
	if ((GLFW_KEY_ESCAPE == key) && (GLFW_PRESS == action))
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
		return;
	}

	key_movement(window, key, scancode, action, mode);
}

/**********************************************************/
/**
* @brief 窗口按键事件通知的回操作时，调用该接口，
*        更新摄像机位置控制参数，实现画面移动效果；
*        该接口仅由 key_callback() 调用。
*
* @param [in ] window   : 窗口对象。
* @param [in ] key      : 按键值。
* @param [in ] scancode : 按键码。
* @param [in ] action   : 按键动作。
* @param [in ] mode     : 按键模式。
*
* @return bool
*         - 返回 true ，表示按键消息被有效处理；
*         - 返回 false，表示按键消息未被处理。
*/
bool key_movement(GLFWwindow * window, int key, int scancode, int action, int mode)
{
	//======================================
	// Camera control

	if ((GLFW_PRESS != action) && (GLFW_REPEAT != action))
	{
		return false;
	}

	glmCameraParam * xCamera = (glmCameraParam *)glfwGetWindowUserPointer(window);
	if (NULL == xCamera)
	{
		return false;
	}

	GLfloat cameraSpeed = (GLFW_REPEAT != action) ? xCamera->cameraSpeed : (xCamera->cameraSpeed * 2.0F);

	switch (key)
	{
	case GLFW_KEY_W:
		xCamera->cameraPos += cameraSpeed * xCamera->cameraFront;
		break;

	case GLFW_KEY_S:
		xCamera->cameraPos -= cameraSpeed * xCamera->cameraFront;
		break;

	case GLFW_KEY_A:
		xCamera->cameraPos -= cameraSpeed * glm::normalize(glm::cross(xCamera->cameraFront, xCamera->cameraUp));
		break;

	case GLFW_KEY_D:
		xCamera->cameraPos += cameraSpeed * glm::normalize(glm::cross(xCamera->cameraFront, xCamera->cameraUp));
		break;

	case GLFW_KEY_LEFT:
		view_offset(xCamera, xCamera->mouseXPos - ((GLFW_REPEAT != action) ? 4.0F : 8.0F), xCamera->mouseYPos);
		break;

	case GLFW_KEY_UP:
		view_offset(xCamera, xCamera->mouseXPos, xCamera->mouseYPos + ((GLFW_REPEAT != action) ? 4.0F : 8.0F));
		break;

	case GLFW_KEY_RIGHT:
		view_offset(xCamera, xCamera->mouseXPos + ((GLFW_REPEAT != action) ? 4.0F : 8.0F), xCamera->mouseYPos);
		break;

	case GLFW_KEY_DOWN:
		view_offset(xCamera, xCamera->mouseXPos, xCamera->mouseYPos - ((GLFW_REPEAT != action) ? 4.0F : 8.0F));
		break;

	default:
		return false;
		break;
	}

	//======================================

	std::cout << "View : "
			  << "PY("    << xCamera->viewPitch     << ", " << xCamera->viewYaw       << ") "
			  << "Pos("   << xCamera->cameraPos.x   << ", " << xCamera->cameraPos.y   << ", " << xCamera->cameraPos.z   << ") "
			  << "Front(" << xCamera->cameraFront.x << ", " << xCamera->cameraFront.y << ", " << xCamera->cameraFront.z << ") "
			  << "Up("    << xCamera->cameraUp.x    << ", " << xCamera->cameraUp.y    << ", " << xCamera->cameraUp.z    << ")" << std::endl;

	//======================================

	return true;
}

/**********************************************************/
/**
* @brief 窗口鼠标事件通知的回调函数接口。
*
* @param [in ] window : 窗口对象。
* @param [in ] xpos   : 水平坐标。
* @param [in ] ypos   : 垂直坐标。
*
* @return void
*
*/
void mouse_callback(GLFWwindow * window, double xpos, double ypos)
{
	glmCameraParam * xCamera = (glmCameraParam *)glfwGetWindowUserPointer(window);
	if (NULL == xCamera)
	{
		return;
	}

	static GLboolean mouseFirstInput = GL_TRUE;
	if (mouseFirstInput)
	{
		xCamera->mouseXPos = (GLfloat)xpos;
		xCamera->mouseYPos = (GLfloat)ypos;
		mouseFirstInput = GL_FALSE;
	}

	view_offset(xCamera, xpos, ypos);
}

/**********************************************************/
/**
* @brief 窗口鼠标滚轮事件通知的回调函数接口。
*
* @param [in ] window  : 窗口对象。
* @param [in ] xoffset : 水平偏移量。
* @param [in ] yoffset : 垂直偏移量。
*
* @return void
*
*/
void scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
	glmCameraParam * xCamera = (glmCameraParam *)glfwGetWindowUserPointer(window);
	if (NULL == xCamera)
	{
		return;
	}

	GLfloat deltaValue = (GLfloat)(0.1F * yoffset);
	if ((xCamera->viewZoom >= 1.0F) && (xCamera->viewZoom <= 45.0F))
		xCamera->viewZoom -= deltaValue;

	if (xCamera->viewZoom < 1.0F)
		xCamera->viewZoom = 1.0F;
	if (xCamera->viewZoom > 45.0F)
		xCamera->viewZoom = 45.0F;

	std::cout << "viewScale : " << xCamera->viewZoom << std::endl;
}
