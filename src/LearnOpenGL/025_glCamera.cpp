/**
* @file    025_glCamera.cpp
* <pre>
* Copyright (c) 2018, Gaaagaa All rights reserved.
* 
* �ļ����ƣ�025_glCamera.cpp
* �������ڣ�2018��09��20��
* �ļ���ʶ��
* �ļ�ժҪ��LearnOpenGL ʾ�������������
* 
* ��ǰ�汾��1.0.0.0
* ��    �ߣ�
* ������ڣ�2018��09��20��
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

// SOIL
#include <SOIL/SOIL.h>

// GLM
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

////////////////////////////////////////////////////////////////////////////////

//====================================================================

// 
// ������ݡ��ӿڵ�ǰ������
// 


const GLuint WIDTH  = 800;  ///< ���ڻ�ͼ�ͻ����Ŀ��
const GLuint HEIGHT = 600;  ///< ���ڻ�ͼ�ͻ����ĸ߶�

/**
* @struct glmCameraParam
* @brief  �����λ�ÿ��Ʋ�����
*/
typedef struct glmCameraParam
{
	GLfloat    cameraSpeed;  ///< ������ƶ��ٶ�
	glm::vec3  cameraPos;    ///< �����λ������
	glm::vec3  cameraFront;  ///< ����������������������� = cameraPos + cameraFront��
	glm::vec3  cameraUp;     ///< �������������

	GLfloat    mouseXPos;    ///< ��¼����ƶ���ˮƽλ��
	GLfloat    mouseYPos;    ///< ��¼����ƶ��Ĵ�ֱλ��
	GLfloat    viewPitch;    ///< ��ͼ������
	GLfloat    viewYaw;      ///< ��ͼƫ����
	GLfloat    viewZoom;     ///< ��ͼ������
} glmCameraParam;

/**********************************************************/
/**
* @brief �޸������λ�ÿ��Ʋ����е���ͼƫ������ֶ�ֵ��
*
* @param [in ] xCamera : �����λ�ÿ��Ʋ�����
* @param [in ] xpos    : ˮƽ���ꡣ
* @param [in ] ypos    : ��ֱ���ꡣ
*
* @return void
*
*/
void view_offset(glmCameraParam * xCamera, double xpos, double ypos);

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

/**********************************************************/
/**
* @brief ���ڰ����¼�֪ͨ�Ļز���ʱ�����øýӿڣ�
*        ���������λ�ÿ��Ʋ�����ʵ�ֻ����ƶ�Ч����
*        �ýӿڽ��� key_callback() ���á�
* 
* @param [in ] window   : ���ڶ���
* @param [in ] key      : ����ֵ��
* @param [in ] scancode : �����롣
* @param [in ] action   : ����������
* @param [in ] mode     : ����ģʽ��
* 
* @return bool
*         - ���� true ����ʾ������Ϣ����Ч����
*         - ���� false����ʾ������Ϣδ������
*/
bool key_movement(GLFWwindow * window, int key, int scancode, int action, int mode);

/**********************************************************/
/**
* @brief ��������¼�֪ͨ�Ļص������ӿڡ�
* 
* @param [in ] window : ���ڶ���
* @param [in ] xpos   : ˮƽ���ꡣ
* @param [in ] ypos   : ��ֱ���ꡣ
* 
* @return void
*         
*/
void mouse_callback(GLFWwindow * window, double xpos, double ypos);

/**********************************************************/
/**
* @brief �����������¼�֪ͨ�Ļص������ӿڡ�
*
* @param [in ] window  : ���ڶ���
* @param [in ] xoffset : ˮƽƫ������
* @param [in ] yoffset : ��ֱƫ������
*
* @return void
*
*/
void scroll_callback(GLFWwindow * window, double xoffset, double yoffset);

//====================================================================

// 
// �������������
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

	GLFWwindow * window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL - 025 - glCamera", NULL, NULL);
	if (NULL == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetKeyCallback(window, &key_callback);
	// glfwSetCursorPosCallback(window, &mouse_callback);
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

	glmCameraParam xCameraParam;
	xCameraParam.cameraSpeed = 0.1F;
	xCameraParam.cameraPos   = glm::vec3(0.0F, 0.0F,  3.0F);
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

		glBindTexture(GL_TEXTURE_2D, texture);

		glUseProgram(xshader.program_id());
		{
			GLfloat   xtime = (GLfloat)glfwGetTime();
			glm::mat4 mtx4V = glm::lookAt(xCameraParam.cameraPos,
										  xCameraParam.cameraPos + xCameraParam.cameraFront,
										  xCameraParam.cameraUp);

			mtx4P = glm::perspective(xCameraParam.viewZoom, ((GLfloat)WIDTH) / ((GLfloat)HEIGHT), 0.1F, 100.0F);

			glUniformMatrix4fv(glGetUniformLocation(xshader.program_id(), "mtx4V"), 1, GL_FALSE, glm::value_ptr(mtx4V));
			glUniformMatrix4fv(glGetUniformLocation(xshader.program_id(), "mtx4P"), 1, GL_FALSE, glm::value_ptr(mtx4P));

			glBindVertexArray(VAO);
#if 0
			for (GLuint i = 0; i < 10; ++i)
			{
				glm::mat4 mtx4M;
				mtx4M = glm::translate(mtxIdentity, cubePositions[i]);
				mtx4M = glm::rotate(mtx4M, glm::radians(i * 10.0F) + xtime * i, glm::vec3(1.0F, 0.3F, 0.5F));
				glUniformMatrix4fv(glGetUniformLocation(xshader.program_id(), "mtx4M"), 1, GL_FALSE, glm::value_ptr(mtx4M));

				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
#else
			glDrawArrays(GL_TRIANGLES, 0, 36);
#endif
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
* @brief �޸������λ�ÿ��Ʋ����е���ͼƫ������ֶ�ֵ��
* 
* @param [in ] xCamera : �����λ�ÿ��Ʋ�����
* @param [in ] xpos    : ˮƽ���ꡣ
* @param [in ] ypos    : ��ֱ���ꡣ
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
	if ((GLFW_KEY_ESCAPE == key) && (GLFW_PRESS == action))
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
		return;
	}

	key_movement(window, key, scancode, action, mode);
}

/**********************************************************/
/**
* @brief ���ڰ����¼�֪ͨ�Ļز���ʱ�����øýӿڣ�
*        ���������λ�ÿ��Ʋ�����ʵ�ֻ����ƶ�Ч����
*        �ýӿڽ��� key_callback() ���á�
*
* @param [in ] window   : ���ڶ���
* @param [in ] key      : ����ֵ��
* @param [in ] scancode : �����롣
* @param [in ] action   : ����������
* @param [in ] mode     : ����ģʽ��
*
* @return bool
*         - ���� true ����ʾ������Ϣ����Ч����
*         - ���� false����ʾ������Ϣδ������
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
* @brief ��������¼�֪ͨ�Ļص������ӿڡ�
*
* @param [in ] window : ���ڶ���
* @param [in ] xpos   : ˮƽ���ꡣ
* @param [in ] ypos   : ��ֱ���ꡣ
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
* @brief �����������¼�֪ͨ�Ļص������ӿڡ�
*
* @param [in ] window  : ���ڶ���
* @param [in ] xoffset : ˮƽƫ������
* @param [in ] yoffset : ��ֱƫ������
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
