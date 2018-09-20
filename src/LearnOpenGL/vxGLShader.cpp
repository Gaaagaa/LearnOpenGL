/**
* @file    vxGLShader.cpp
* <pre>
* Copyright (c) 2010-2018, Gaaagaa All rights reserved.
* 
* �ļ����ƣ�vxGLShader.cpp
* �������ڣ�2018��08��20��
* �ļ���ʶ��
* �ļ�ժҪ��ͨ���ⲿ�����ļ����أ�������OpenGL ��ɫ���Ĺ����ࡣ
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
* @brief ��ȡ�ļ���С��
* 
* @param [in ] xpath  : �ļ�·����
* @param [out] nerror : ����β�Ϊ NULL����ɷ��ز�����Ӧ�Ĵ����롣
* 
* @return size_t
*         - ���� �ļ���С��
*         - ���� -1������ ������ nerror ��ֵ��
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
* @brief ��ȡ�����ļ������������С�
* 
* @param [in    ] xpath   : �ļ�·����
* @param [out   ] xbuffer : ��������Ļ��档
* @param [in,out] nsize   : ��Σ���ʾ�����С���زΣ���ʾ��ȡ������Ч���ݳ��ȡ�
* 
* @return int
*         - �ɹ������� 0��
*         - ʧ�ܣ����� ���������롣
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
		// ������Ч�Լ��

		if ((NULL == xpath) || (NULL == xbuffer) || (NULL == nsize) || (*nsize <= 0))
		{
			break;
		}

		//======================================

		// ���ļ�
		xfile = fopen(xpath, "r");
		if (NULL == xfile)
		{
			nerror = errno;
			break;
		}

		// ��ȡ�ļ�
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

		// ���ö�ȡ����������Ч���ȣ���Ϊ���ز���
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
* @brief ���ⲿ�����ļ�������ɫ����
* 
* @param [in ] xpath   : �ⲿ�����ļ����ļ�·����
* @param [in ] ntype   : ��������ɫ�����ͣ�GL_VERTEX_SHADER��GL_FRAGMENT_SHADER����
* @param [in ] nerror  : �������صĴ�����ֵ��
* @param [in ] xerrlog : �������صı��������־��Ϣ�������е���free()�����Ƿ񻺴棩��
* 
* @return GLuint
*         - �����ɹ������ص���ɫ�� ID��
*         - ����ʧ�ܣ����� 0������ �����루nerror���� ���������־��Ϣ��xerr_log����
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
		// ��������ļ���������Ļ��棬��ȡ��ɫ������Դ

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
		// ������ɫ��

		// ����������
		xshader = glCreateShader(ntype);
		glShaderSource(xshader, 1, &xbuffer, NULL);
		glCompileShader(xshader);

		// ��������
		glGetShaderiv(xshader, GL_COMPILE_STATUS, &xtmerr);
		if (!xtmerr)
		{
			// ���ر���Ĵ�����־��Ϣ
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
* @brief ������ɫ����
* 
* @param [in ] xpath_vshader : ������ɫ���������ⲿ�����ļ�·����
* @param [in ] xpath_fshader : Ƭ����ɫ���������ⲿ�����ļ�·����
* @param [in ] xerrlog       : �������صı��������־��Ϣ�������е���free()�����Ƿ񻺴棩��
* 
* @return int
*         - �ɹ������� 0��
*         - ʧ�ܣ����� �����롣
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
		// ������ɫ��

		vshader = create_shader(xpath_vshader, GL_VERTEX_SHADER, &nerror, xerrlog);
		if (0 == vshader)
		{
			break;
		}

		//======================================
		// Ƭ����ɫ��

		fshader = create_shader(xpath_fshader, GL_FRAGMENT_SHADER, &nerror, xerrlog);
		if (0 == fshader)
		{
			break;
		}

		//======================================
		// ��ɫ������

		program = glCreateProgram();
		glAttachShader(program, vshader);
		glAttachShader(program, fshader);
		glLinkProgram(program);

		// check for linking errors
		glGetProgramiv(program, GL_LINK_STATUS, &xtmerr);
		if (!xtmerr)
		{
			// ���ش�����־��Ϣ
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
* @brief ������ɫ����
*/
void vxGLShader::destroy()
{
	if (0 != m_pid)
	{
		glDeleteProgram(m_pid);
		m_pid = 0;
	}
}
