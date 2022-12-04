#ifndef _CSHADER_H_
#define _CSHADER_H_

/*
#include <iostream>
#include <fstream>
#include <strings>
*/

#include "core_global.h"
//#include <gl/glew.h>
//#include <strings.h>

using namespace std;

typedef enum _tageShaderType
{
	VERT_SHADER     =	GL_VERTEX_SHADER,
	GEOM_SHADER     =	GL_GEOMETRY_SHADER,
	TESS_CONT_SHADER =  GL_TESS_CONTROL_SHADER,
	TESS_EVAL_SHADER =	GL_TESS_EVALUATION_SHADER,
	FRAG_SHADER     =	GL_FRAGMENT_SHADER,
	COMP_SHADER     =	GL_COMPUTE_SHADER
} eShaderType, eST;

class cShader
{
	protected:
		unsigned long	_vertShaderObj;
		unsigned long	_geomShaderObj;
		unsigned long	_tessControlShaderObj;
		unsigned long	_tessEvaluationShaderObj;
		unsigned long	_fragShaderObj;
		unsigned long	_computeShaderObj;
		unsigned long	_programObj;

		int	_vertShaderStatus;
		int	_geomShaderStatus;
		int	_tessControlShaderStatus;
		int	_tessEvaluationShaderStatus;
		int	_fragShaderStatus;
		int	_computeShaderStatus;
		int	_progLinkStatus;

		string	_vertShaderSrc;
		string	_geomShaderSrc;
		string	_tessControlShaderSrc;
		string	_tessEvaluationShaderSrc;
		string	_fragShaderSrc;
		string	_computeShaderSrc;

	protected:
//		bool	_ReadShaderFile(const char *filename);//, eShaderType shaderType);
//		const char*   _ReadShaderFile(const char *filename);
        string  _ReadShaderFile(const char *filename);
//		bool    _LogOpenGLError(const char *logfilename = "glerror.log");		//*******TODO:	define _logglerror()
		bool    _LogShaderError(eShaderType shaderType, const char *logfilename = "glslerror.log");
		bool    _LogProgramError(const char *logfilname = "glslerror.log");

	public:
		cShader();
		~cShader();

		bool	_CreateShaderObject(const char *filename, eShaderType shaderType);
		bool	_CreateProgram(void);
		bool	_Bind(bool buseProgram);
		bool	_Free(void);
		bool	_QueryProgramObj(void);

		inline unsigned long _GetProgramObject(void) const { return _programObj; }

		// Get the Uniform Location ID from the shader by name.
        int _CreateUniformLocationID(const char *szuniformName);

        // Send the value of a uniform to the shader.
        bool _LoadUniformInt(int uniLocationID, int value);
        bool _LoadUniformFloat(int uniLocationID, float value);
        bool _LoadUniformVec2D(int uniLocationID, glm::vec2 value);
        bool _LoadUniformVec3D(int uniLocationID, glm::vec3 value);
        bool _LoadUniformVec4D(int uniLocationID, glm::vec4 value);
        bool _LoadUniformMat2(int uniLocationID, glm::mat2 value);
        bool _LoadUniformMat3(int uniLocationID, glm::mat3 value);
        bool _LoadUniformMat4(int uniLocationID, glm::mat4 value);
        bool _LoadUniformMat2v(int uniLocationID, float *pvalue);
        bool _LoadUniformMat3v(int uniLocationID, float *pvalue);
        bool _LoadUniformMat4v(int uniLocationID, float *pvalue);
        bool _LoadUniformTextureSampler(int uniLocationID, int value);

};
#endif // _CSHADER_H_
