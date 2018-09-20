/**
* @file    vxGLShader.h
* <pre>
* Copyright (c) 2010-2018, Gaaagaa All rights reserved.
* 
* �ļ����ƣ�vxGLShader.h
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

#ifndef __VXGLSHADER_H__
#define __VXGLSHADER_H__

////////////////////////////////////////////////////////////////////////////////
// vxGLShader

/**
* @class vxGLShader
* @brief ͨ���ⲿ�����ļ����أ�������OpenGL ��ɫ���Ĺ����ࡣ
*/
class vxGLShader
{
	// constructor/destructor
public:
	vxGLShader();
	~vxGLShader();

	// public interfaces
public:
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
	int create(const char * xpath_vshader, const char * xpath_fshader, char ** xerrlog);

	/**********************************************************/
	/**
	* @brief ������ɫ����
	*/
	void destroy();

	/**********************************************************/
	/**
	* @brief ��ɫ�������ʶ ID��
	*/
	inline unsigned int program_id() { return m_pid; }

	// data members
private:
	unsigned int   m_pid;   ///< ��ɫ������ ID
};

////////////////////////////////////////////////////////////////////////////////

#endif // __VXGLSHADER_H__
