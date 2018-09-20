/**
* @file    vxGLShader.cpp
* <pre>
* Copyright (c) 2010-2018, Gaaagaa All rights reserved.
* 
* 文件名称：vxGLShader.cpp
* 创建日期：2018年08月20日
* 文件标识：
* 文件摘要：通过外部配置文件加载（创建）OpenGL 着色器的工具类。
* 
* 当前版本：1.0.0.0
* 作    者：
* 完成日期：2018年08月20日
* 版本摘要：
* 
* 取代版本：
* 原作者  ：
* 完成日期：
* 版本摘要：
* </pre>
*/

#include "vxGLShader.h"

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

////////////////////////////////////////////////////////////////////////////////

/**********************************************************/
/**
* @brief 读取文件大小。
* 
* @param [in ] xpath  : 文件路径。
* @param [out] nerror : 若入参不为 NULL，则可返回操作对应的错误码。
* 
* @return size_t
*         - 返回 文件大小；
*         - 返回 -1，请检查 错误码 nerror 的值。
*/
static size_t file_size(const char * xpath, int * nerror)
{
	struct stat xstat;
	memset(&xstat, 0, sizeof(struct stat));

	if (NULL == xpath)
	{
		if (NULL != nerror)
			*nerror = -1;
		return (size_t)-1;
	}

	if (0 != stat(xpath, &xstat))
	{
		if (NULL != nerror)
			*nerror = errno;
		return (size_t)-1;
	}

	return xstat.st_size;
}

/**********************************************************/
/**
* @brief 读取磁盘文件数据至缓存中。
* 
* @param [in    ] xpath   : 文件路径。
* @param [out   ] xbuffer : 数据输出的缓存。
* @param [in,out] nsize   : 入参，表示缓存大小；回参，表示读取到的有效数据长度。
* 
* @return int
*         - 成功，返回 0；
*         - 失败，返回 操作错误码。
*/
static int file_read(const char * xpath, char * xbuffer, size_t * nsize)
{
	int nerror = -1;

	FILE * xfile = NULL;
	size_t nread = 0;
	size_t count = 0;

	do
	{
		//======================================
		// 参数有效性检测

		if ((NULL == xpath) || (NULL == xbuffer) || (NULL == nsize) || (*nsize <= 0))
		{
			break;
		}

		//======================================

		// 打开文件
		xfile = fopen(xpath, "r");
		if (NULL == xfile)
		{
			nerror = errno;
			break;
		}

		// 读取文件
		count = 0;
		while (count < *nsize)
		{
			nread = fread(xbuffer + count, sizeof(char), *nsize - count, xfile);
			if (nread <= 0)
			{
				break;
			}

			count += nread;
		}

		// 设置读取到的数据有效长度，作为返回参数
		*nsize = count;

		//======================================
		nerror = 0;
	} while (0);

	if (NULL != xfile)
	{
		fclose(xfile);
		xfile = NULL;
	}

	return nerror;
}

//====================================================================

/**********************************************************/
/**
* @brief 由外部配置文件创建着色器。
* 
* @param [in ] xpath   : 外部配置文件的文件路径。
* @param [in ] ntype   : 创建的着色器类型（GL_VERTEX_SHADER、GL_FRAGMENT_SHADER）。
* @param [in ] nerror  : 操作返回的错误码值。
* @param [in ] xerrlog : 操作返回的编译错误日志信息（需自行调用free()操作是否缓存）。
* 
* @return GLuint
*         - 操作成功，返回的着色器 ID；
*         - 操作失败，返回 0，请检查 错误码（nerror）与 编译错误日志信息（xerr_log）。
*/
GLuint create_shader(const char * xpath, int ntype, int * nerror, char ** xerrlog)
{
	int verror = -1;

	char  * xbuffer = NULL;
	size_t  nbsize  = 0;
	size_t  nvsize  = 0;

	GLuint  xshader = 0;
	GLint   xtmerr  = 0;
	GLsizei xlength = 0;

	do
	{
		//======================================

		if (NULL == xpath)
		{
			break;
		}

		//======================================
		// 申请加载文件数据所需的缓存，读取着色器代码源

		nbsize = file_size(xpath, &verror);
		if ((size_t)-1 == nbsize)
		{
			break;
		}

		nbsize  = (nbsize / 4096 + 1) * 4096;
		xbuffer = (char *)calloc(nbsize, sizeof(char));
		if (NULL == xbuffer)
		{
			verror = -1;
			break;
		}

		nvsize = nbsize;
		verror = file_read(xpath, xbuffer, &nvsize);
		if (0 != verror)
		{
			break;
		}
		xbuffer[nvsize] = '\0';

		//======================================
		// 创建着色器

		// 创建并编译
		xshader = glCreateShader(ntype);
		glShaderSource(xshader, 1, &xbuffer, NULL);
		glCompileShader(xshader);

		// 检测编译结果
		glGetShaderiv(xshader, GL_COMPILE_STATUS, &xtmerr);
		if (!xtmerr)
		{
			// 返回编译的错误日志信息
			if (NULL != xerrlog)
			{
				glGetShaderiv(xshader, GL_INFO_LOG_LENGTH, &xlength);
				if ((xlength > 0) &&
					(NULL != (*xerrlog = (char *)calloc((xlength / 64 + 1) * 64, sizeof(char)))))
				{
					glGetShaderInfoLog(xshader, xlength, NULL, *xerrlog);
				}
			}

			verror = -2;
			break;
		}

		//======================================
		verror = 0;
	} while (0);

	//======================================
	// cleanup

	if ((0 != verror) && (0 != xshader))
	{
		glDeleteShader(xshader);
		xshader = 0;
	}

	if (NULL != xbuffer)
	{
		free(xbuffer);
		xbuffer = NULL;
	}

	if (NULL != nerror)
	{
		*nerror = verror;
	}

	//======================================

	return xshader;
}

////////////////////////////////////////////////////////////////////////////////
// vxGLShader

//====================================================================

// 
// vxGLShader : constructor/destructor
// 

vxGLShader::vxGLShader()
	: m_pid(0)
{

}

vxGLShader::~vxGLShader()
{
	destroy();
}

//====================================================================

// 
// vxGLShader : public interfaces
// 

/**********************************************************/
/**
* @brief 创建着色器。
* 
* @param [in ] xpath_vshader : 顶点着色器描述的外部配置文件路径。
* @param [in ] xpath_fshader : 片段着色器描述的外部配置文件路径。
* @param [in ] xerrlog       : 操作返回的编译错误日志信息（需自行调用free()操作是否缓存）。
* 
* @return int
*         - 成功，返回 0；
*         - 失败，返回 错误码。
*/
int vxGLShader::create(const char * xpath_vshader, const char * xpath_fshader, char ** xerrlog)
{
	int nerror = -1;

	GLuint  vshader = 0;
	GLuint  fshader = 0;
	GLuint  program = 0;
	GLint   xtmerr  = 0;
	GLsizei xlength = 0;

	do 
	{
		//======================================

		if (0 != program_id())
		{
			destroy();
		}

		//======================================
		// 顶点着色器

		vshader = create_shader(xpath_vshader, GL_VERTEX_SHADER, &nerror, xerrlog);
		if (0 == vshader)
		{
			break;
		}

		//======================================
		// 片段着色器

		fshader = create_shader(xpath_fshader, GL_FRAGMENT_SHADER, &nerror, xerrlog);
		if (0 == fshader)
		{
			break;
		}

		//======================================
		// 着色器程序

		program = glCreateProgram();
		glAttachShader(program, vshader);
		glAttachShader(program, fshader);
		glLinkProgram(program);

		// check for linking errors
		glGetProgramiv(program, GL_LINK_STATUS, &xtmerr);
		if (!xtmerr)
		{
			// 返回错误日志信息
			if (NULL != xerrlog)
			{
				glGetProgramiv(program, GL_INFO_LOG_LENGTH, &xlength);
				if ((xlength > 0) &&
					(NULL != (*xerrlog = (char *)calloc((xlength / 64 + 1) * 64, sizeof(char)))))
				{
					glGetProgramInfoLog(program, xlength, NULL, *xerrlog);
				}
			}

			nerror = -1;
			break;
		}

		m_pid = program;
		program = 0;

		//======================================
		nerror = 0;
	} while (0);

	//======================================
	// cleanup

	if (0 != program)
	{
		glDeleteProgram(program);
		program = 0;
	}

	if (0 != vshader)
	{
		glDeleteShader(vshader);
		vshader = 0;
	}

	if (0 != fshader)
	{
		glDeleteShader(fshader);
		fshader = 0;
	}

	if (0 != nerror)
	{
		destroy();
	}

	//======================================

	return nerror;
}

/**********************************************************/
/**
* @brief 销毁着色器。
*/
void vxGLShader::destroy()
{
	if (0 != m_pid)
	{
		glDeleteProgram(m_pid);
		m_pid = 0;
	}
}
