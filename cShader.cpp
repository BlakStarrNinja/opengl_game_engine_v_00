#include "core_global.h"

cShader::cShader()
{
	_vertShaderObj = _geomShaderObj = _fragShaderObj =
		_tessControlShaderObj = _tessEvaluationShaderObj =
		_computeShaderObj = _programObj = 0L;

	_vertShaderStatus = _geomShaderStatus = _fragShaderStatus =
		_tessControlShaderStatus = _tessEvaluationShaderStatus =
		_computeShaderStatus = _progLinkStatus = 0;
}

cShader::~cShader() { _Free(); }

/*
bool
cShader::_LogOpenGLError(const char *logfilename)
{

}
*/

bool
cShader::_LogShaderError(eShaderType shaderType, const char *logfilename)
{
	unsigned long shaderObj = 0L;
	string shdtok;

	int infologLength	= 0;
	int charWritten		= 0;
	char *infoLog		= NULL;

	switch (shaderType)
	{
		case VERT_SHADER:
			shaderObj = _vertShaderObj;
			shdtok = "Vertex Shader";
		break;

		case GEOM_SHADER:
			shaderObj = _geomShaderObj;
			shdtok = "Geometry Shader";
		break;

		case FRAG_SHADER:
			shaderObj = _fragShaderObj;
			shdtok = "Fragment Shader";
		break;

		default:
			MessageBox(NULL, "Invalid Shader Type was sent.\nPlease choose a valid shader type of:\n\tVERT_SHADER\n\tGEOM_SHADER\n\tFRAG_SHADER", "ERROR: INVALID SHADER TYPE", MBOKEX);

			return false;
		break;
	}

	glGetShaderiv(shaderObj, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 1)
	{
		infoLog = ::new char[infologLength];
		if (!infoLog)
		{
			_MB("Out of memory to create shader log!", "ERROR: OUT OF MEMORY");

			return false;
		}
		glGetShaderInfoLog(shaderObj, infologLength, &charWritten, infoLog);
		ofstream logfile(logfilename, ios::ate | ios::app);
		if (!logfile)
		{
			_MB("Could NOT create/open shader log file!", "ERROR: COULD NOT OPEN FILE");
			logfile.close();

			return false;
		}
		SYSTEMTIME	lt;
		ZeroMemory(&lt, sizeof(SYSTEMTIME));
		GetLocalTime(&lt);
//		_MB(infoLog,"ERROR: SHADER");
		logfile	<<      shdtok<<" InfoLog: "<<lt.wMonth<<'/'<<lt.wDay<<'/'<<lt.wYear
				<<"  "<<lt.wHour<<':'<<lt.wMinute<<':'<<lt.wSecond<<'.'<<lt.wMilliseconds
				<<'\n'<<infoLog<<'\n'<<logfilename<<' '<<"infoLog length = "<<infologLength
				<<" bytes\n\n********************************************************************************\n";
		::delete[] infoLog;
		logfile.close();

		return false;
	}
	return true;
}

bool
cShader::_LogProgramError(const char* logfilename)
{
	int infologLength	= 0;
	int charWritten		= 0;
	char *infoLog		= NULL;

	glGetProgramiv(_programObj, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 1)
	{
		infoLog = ::new char[infologLength];
		if (!infoLog)
		{
			_MB("Out of memory to create program log!", "ERROR: OUT OF MEMORY");

			return false;
		}
		glGetProgramInfoLog(_programObj, infologLength, &charWritten, infoLog);
		ofstream logfile(logfilename, ios::ate | ios::app);
		if (!logfile)
		{
			_MB("Could NOT create/open program log file!", "ERROR: COULD NOT OPEN FILE");
			logfile.close();

			return false;
		}
		SYSTEMTIME	lt;
		ZeroMemory(&lt, sizeof(SYSTEMTIME));
		GetLocalTime(&lt);
//		_MB(infoLog,"ERROR: SHADER PROGRAM");
		logfile	<<      "Program InfoLog: "<<lt.wMonth<<'/'<<lt.wDay<<'/'<<lt.wYear
				<<"  "<<lt.wHour<<':'<<lt.wMinute<<':'<<lt.wSecond<<'.'<<lt.wMilliseconds
				<<'\n'<<infoLog<<'\n'<<logfilename<<' '<<"infoLog length = "<<infologLength
				<<" bytes\n\n********************************************************************************\n";
		::delete[] infoLog;
		logfile.close();

		return false;
	}
	return true;
}

std::string
cShader::_ReadShaderFile(const char *filename)
{
	FILE *pfile = fopen(filename, "r");

	if (!pfile)
	{
		fclose(pfile);

		string sfilename = filename;
		string errMess = "Could not open shader file: \"" + sfilename + "\".";
        _MB(errMess.c_str(), "ERROR: FILE NOT FOUND!");

		return std::string();
	}

	size_t filesize = 0;
	fseek(pfile, 0, SEEK_END);
	filesize = ftell(pfile);
	rewind(pfile);

	if (!filesize)
	{
		fclose(pfile);

		string sfilename = filename;
		string errMess	= '\"' + sfilename + "\" is an invalid file!\nPlease load a valid shader file.";
		_MB(errMess.c_str(), "ERROR: INVALID FILE!");

		return std::string();
	}

	char *buffer = ::new char[filesize];
	memset(buffer, 0, filesize);
	fread(buffer, sizeof(char), filesize, pfile);

	fclose(pfile);

	std::string returnStr = buffer;
	::delete[] buffer;

	return returnStr;
}

bool
cShader::_CreateShaderObject(const char *filename, eShaderType shaderType)
{
	unsigned long *pshaderObj = NULL, shaderObj = 0;
	int *pstatus = NULL, valstatus = 0;
	string *pshaderSrc	= NULL;
	string shaderToken;

	switch (shaderType)
	{
		case VERT_SHADER:
			pshaderObj = &_vertShaderObj;
			pstatus = &_vertShaderStatus;
			pshaderSrc = &_vertShaderSrc;
			shaderToken = "Vertex";
		break;

		case GEOM_SHADER:
			pshaderObj = &_geomShaderObj;
			pstatus = &_geomShaderStatus;
			pshaderSrc = &_geomShaderSrc;
			shaderToken = "Geometry";
		break;

		case FRAG_SHADER:
			pshaderObj = &_fragShaderObj;
			pstatus = &_fragShaderStatus;
			pshaderSrc = &_fragShaderSrc;
			shaderToken = "Fragment";
		break;

		default:
			_MB("Invalid Shader Type was sent.\nPlease choose a valid shader type of:\n\tVERT_SHADER\n\tGEOM_SHADER\n\tFRAG_SHADER", "ERROR: INVALID SHADER TYPE");

			return false;
		break;
	}

	*pshaderObj = glCreateShader(shaderType);
	shaderObj = *pshaderObj;
/*
	const char *pbuffer = _ReadShaderFile(filename);	//create pbuffer just to be sure that a NULL is not passed to string object.
	if (!pbuffer)										//if pbuffer is NULL the shader src file was invalid.
		return false;

	*pshaderSrc = pbuffer;

    glShaderSource(shaderObj, 1, &pbuffer, NULL);
*/
    *pshaderSrc = _ReadShaderFile(filename);
    if (!pshaderSrc->length())
        return false;

    const char *pbuffer = pshaderSrc->c_str();

//    glShaderSource(shaderObj, 1, (const char**)&*pshaderSrc->c_str(), NULL);//(const char**)pshaderSrc->c_str()
    glShaderSource(shaderObj, 1, &pbuffer, NULL);
	glCompileShader(*pshaderObj);
	glGetShaderiv(*pshaderObj, GL_COMPILE_STATUS, &valstatus);
	_LogShaderError(shaderType);

	if (!valstatus)
	{
		string errorMsg = "Errors in compiling " + shaderToken + " Shader.\nPlease read logs for more information.";
		_MB(errorMsg.c_str(), "ERROR: SHADER COMPILING ERROR");

		return false;
	}

	*pstatus = valstatus;

	return true;
}

bool
cShader::_CreateProgram(void)
{
	_programObj = glCreateProgram();

	if (_vertShaderStatus) glAttachShader(_programObj, _vertShaderObj);
	if (_geomShaderStatus) glAttachShader(_programObj, _geomShaderObj);
	if (_fragShaderStatus) glAttachShader(_programObj, _fragShaderObj);

	glLinkProgram(_programObj);
	glGetProgramiv(_programObj, GL_LINK_STATUS, &_progLinkStatus);
	_LogProgramError();

	if (!_progLinkStatus)
	{
		_MB("Errors in linking shader program!\nPlease read logs for explanation.","ERROR: SHADER LINKING");

		return false;
	}
	return true;
}

bool
cShader::_Bind(bool buseProgram)
{
	(buseProgram)?glUseProgram(_programObj):glUseProgram(NULL);

	return true;
}

bool
cShader::_Free(void)
{
	glUseProgram(NULL);

	if (_vertShaderStatus)
	{
	    glDetachShader(_programObj, _vertShaderObj);
	    glDeleteShader(_vertShaderObj);
	    _LogShaderError(VERT_SHADER);
	}
	if (_geomShaderStatus)
	{
	    glDetachShader(_programObj, _geomShaderObj);
	    glDeleteShader(_geomShaderObj);
	    _LogShaderError(GEOM_SHADER);
	}
	if (_fragShaderStatus)
	{
	    glDetachShader(_programObj, _fragShaderObj);
	    glDeleteShader(_fragShaderObj);
	    _LogShaderError(FRAG_SHADER);
	}

	glDeleteProgram(_programObj);
	_LogProgramError();

	return true;
}

bool
cShader::_QueryProgramObj(void)
{
	ofstream queryfile("glslquery.log");

	if (!queryfile)
	{
		queryfile.close();
		_MB("Couldn't create/open query log file","ERROR:	FILE ERROR");

		return false;
	}

	if (!_programObj)
	{
		queryfile.close();
		_MB("Shader Program Not Created.","ERROR:	INVALID SHADER");

		return false;
	}

	int	query = 0;

	int numActiveUniforms = 0,
            maxActUniNameLen = 0;

	glGetProgramiv(_programObj, GL_ACTIVE_ATTRIBUTES, &query);
	queryfile<<"GL_ACTIVE_ATTRIBUTES\t\t\t"<<query<<endl;

	glGetProgramiv(_programObj, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &query);
	queryfile<<"GL_ACTIVE_ATTRIBUTE_MAX_LENGTH\t\t"<<query<<endl;

	glGetProgramiv(_programObj, GL_ACTIVE_UNIFORMS, &query);
	queryfile<<"GL_ACTIVE_UNIFORMS\t\t\t"<<query<<endl;
	numActiveUniforms = query;

	glGetProgramiv(_programObj, GL_ACTIVE_UNIFORM_MAX_LENGTH, &query);
	queryfile<<"GL_ACTIVE_UNIFORM_MAX_LENGTH\t\t"<<query<<endl;
    maxActUniNameLen = query;

    queryfile<<endl<<"glGetIntegerv:\n";
	glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &query);
	queryfile<<"GL_MAX_VERTEX_UNIFORM_COMPONENTS\t\t"<<query<<endl;

	glGetIntegerv(GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS, &query);
	queryfile<<"GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS\t"<<query<<endl;

	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &query);
	queryfile<<"GL_MAX_VERTEX_ATTRIBS\t\t"<<query<<endl;

	int test_varying_array[query];
    queryfile<<"\nsizeof(test_varying_array)\t\t"<<sizeof(test_varying_array)<<endl<<endl;

    char szuniName[maxActUniNameLen];
    memset(szuniName,0,maxActUniNameLen);

    int length = 0, size = 0;
    GLenum type = 0;

    for (int i=0; i<numActiveUniforms; ++i)
    {
        glGetActiveUniform(_programObj, i, maxActUniNameLen, &length, &size, &type, szuniName);
        queryfile<<"UniformName : "<<szuniName<<"  size : "<<size<<"  type : "<<type<<"  length : "<<length<<endl;
        memset(szuniName,0,maxActUniNameLen);
    }

    glGetShaderiv(_vertShaderObj, GL_SHADER_SOURCE_LENGTH, &query);
    char szshader[query+1];
    memset(szshader, 0, query+1);
    glGetShaderSource(_vertShaderObj, query+1, &length, szshader);
    queryfile<<endl<<endl<<endl<<szshader<<endl<<length<<" bytes long.";

    queryfile<<"\nsizeof(szshader) after 1st call : "<<sizeof(szshader)<<" bytes.\n";

    glGetShaderiv(_fragShaderObj, GL_SHADER_SOURCE_LENGTH, &query);
    szshader[query+1];
    memset(szshader, 0, query+1);
    glGetShaderSource(_fragShaderObj, query+1, &length, szshader);
    queryfile<<endl<<endl<<endl<<szshader<<endl<<length<<" bytes long.";


    queryfile<<"\nsizeof(szshader) after 2nd call : "<<sizeof(szshader)<<" bytes.\n";

    queryfile.close();

	return true;
}

int
cShader::_CreateUniformLocationID(const char *szuniformName)
{
    return glGetUniformLocation(_programObj, szuniformName);
}

bool
cShader::_LoadUniformInt(int uniLocationID, int value)
{
	glUniform1i(uniLocationID, value);

	return true;
}

bool
cShader::_LoadUniformFloat(int uniLocationID, float value)
{
	glUniform1f(uniLocationID, value);

	return true;
}

bool
cShader::_LoadUniformVec2D(int uniLocationID, glm::vec2 value)
{
	glUniform2fv(uniLocationID, 1, glm::value_ptr(value));

	return true;
}

bool
cShader::_LoadUniformVec3D(int uniLocationID, glm::vec3 value)
{
	glUniform3fv(uniLocationID, 1, glm::value_ptr(value));

	return true;
}

bool
cShader::_LoadUniformVec4D(int uniLocationID, glm::vec4 value)
{
	glUniform4fv(uniLocationID, 1, glm::value_ptr(value));

	return true;
}

bool
cShader::_LoadUniformMat2(int uniLocationID, glm::mat2 value)
{
	glUniformMatrix2fv(uniLocationID, 1, false, glm::value_ptr(value));

	return true;
}

bool
cShader::_LoadUniformMat3(int uniLocationID, glm::mat3 value)
{
	glUniformMatrix3fv(uniLocationID, 1, false, glm::value_ptr(value));

	return true;
}

bool
cShader::_LoadUniformMat4(int uniLocationID, glm::mat4 value)
{
	glUniformMatrix4fv(uniLocationID, 1, false, glm::value_ptr(value));

	return true;
}

bool
cShader::_LoadUniformMat2v(int uniLocationID, float *pvalue)
{
	glUniformMatrix2fv(uniLocationID, 1, false, (float *)pvalue);

	return true;
}

bool
cShader::_LoadUniformMat3v(int uniLocationID, float *pvalue)
{
	glUniformMatrix3fv(uniLocationID, 1, false, (float *)pvalue);

	return true;
}

bool
cShader::_LoadUniformMat4v(int uniLocationID, float *pvalue)
{
	glUniformMatrix4fv(uniLocationID, 1, false, (float *)pvalue);

	return true;
}

bool
cShader::_LoadUniformTextureSampler(int uniLocationID, int value)
{
	glUniform1i(uniLocationID, value);

	return true;
}




/*
const char *
cShader::_ReadShaderFile(const char *filename)
{
	ifstream shaderFile(filename);

	if (!shaderFile)
	{
		string sfilename = filename;
		string errMess = "Could not open shader file: \"" + sfilename + "\".";
        _MB(errMess.c_str(), "ERROR: FILE NOT FOUND!");
		shaderFile.close();

		return false;
	}

	//get size of file to allocate buffer
	unsigned long	filesize = 0L;			//hold the number of bytes of the file stream
	shaderFile.seekg(0, ios_base::end);		//move stream's position pointer to end of stream
	filesize = shaderFile.tellg();			//get the number of bytes when moved to end of stream
	shaderFile.seekg(0, ios_base::beg);		//move read positon back to the beginning

	if (!filesize)
	{
		string sfilename = filename;
		string errMess	= '\"' + sfilename + "\" is an invalid file!\nPlease load a valid shader file.";
		_MB(errMess.c_str(), "ERROR: INVALID FILE!");
		shaderFile.close();

		return false;
	}

	string bufferString;

	char *buffer = ::new char[filesize+1];	//stream's .read() only accepts char *
	memset(buffer, 0, filesize+1);
	shaderFile.read(buffer, filesize+1);
	buffer[filesize+1] = '\0';

	bufferString = buffer;		//strings objects deallocate themselves when going out of scope.
								//so now I can delete memory for buffer now.
//    memset(buffer, 0, (filesize+1));
	::delete[] buffer;
	shaderFile.close();

	return bufferString.c_str();
}
*/
/*
std::string
cShader::_ReadShaderFile(const char *filename)
{
	ifstream shaderFile(filename);

	if (!shaderFile)
	{
		string sfilename = filename;
		string errMess = "Could not open shader file: \"" + sfilename + "\".";
        _MB(errMess.c_str(), "ERROR: FILE NOT FOUND!");
		shaderFile.close();

		return std::string();
	}

	//get size of file to allocate buffer
	unsigned long	filesize = 0L;			//hold the number of bytes of the file stream
	shaderFile.seekg(0, ios_base::end);		//move stream's position pointer to end of stream
	filesize = shaderFile.tellg();			//get the number of bytes when moved to end of stream
	shaderFile.seekg(0, ios_base::beg);		//move read positon back to the beginning

	if (!filesize)
	{
		string sfilename = filename;
		string errMess	= '\"' + sfilename + "\" is an invalid file!\nPlease load a valid shader file.";
		_MB(errMess.c_str(), "ERROR: INVALID FILE!");
		shaderFile.close();

		return std::string();
	}

	string bufferString;

	char *buffer = ::new char[filesize];	//stream's .read() only accepts char *
	memset(buffer, 0, filesize);
	shaderFile.read(buffer, filesize);
//	buffer[filesize+1] = '\0';

	bufferString = buffer;		//strings objects deallocate themselves when going out of scope.
								//so now I can delete memory for buffer now.
//    memset(buffer, 0, (filesize+1));
	::delete[] buffer;
	shaderFile.close();

	return bufferString;
}
*/

