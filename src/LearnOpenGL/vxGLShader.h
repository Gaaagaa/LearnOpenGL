/**
* @file    vxGLShader.h
* <pre>
* Copyright (c) 2010-2018, Gaaagaa All rights reserved.
* 
* 文件名称：vxGLShader.h
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

#ifndef __VXGLSHADER_H__
#define __VXGLSHADER_H__

////////////////////////////////////////////////////////////////////////////////
// vxGLShader

/**
* @class vxGLShader
* @brief 通过外部配置文件加载（创建）OpenGL 着色器的工具类。
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
	int create(const char * xpath_vshader, const char * xpath_fshader, char ** xerrlog);

	/**********************************************************/
	/**
	* @brief 销毁着色器。
	*/
	void destroy();

	/**********************************************************/
	/**
	* @brief 着色器程序标识 ID。
	*/
	inline unsigned int program_id() { return m_pid; }

	// data members
private:
	unsigned int   m_pid;   ///< 着色器程序 ID
};

////////////////////////////////////////////////////////////////////////////////

#endif // __VXGLSHADER_H__
