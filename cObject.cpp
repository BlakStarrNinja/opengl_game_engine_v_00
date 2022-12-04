#include "core_global.h"

cObject::cObject()
{
	_pgraphics = NULL;
	_pmeshes = NULL;


    _numTexSamp	= 0;
    _puniTextureSampler = NULL;
    _uniMVP = 0;

//	_modelSIMD = glm::simdMat4(glm::mat4(1.f));
//	_pshader = NULL;
}

cObject::~cObject() { _Free(); }

bool
cObject::_Create(cGraphics *pgraphics, cMesh *pmesh)
{
	if (!(_pgraphics = pgraphics))
		return false;

	_pmeshes = pmesh;

	_position = _rotation = glm::vec3(0.0f);
	_scale = glm::vec3(1.0f);

/*
    _modelSIMD = glm::simdMat4(1.f);

	_MoveSIMD(_position);
	_RotateSIMD(_rotation);
	_ScaleSIMD(_scale);
	_UpdateSIMD();
*/

	_model = glm::mat4(1.0f);
	_Move(_position);
	_Rotate(_rotation);
	_Scale(_scale);
	_Update();

	return true;
}

bool
cObject::_Free(void)
{
	_pgraphics = NULL;
	_pmeshes = NULL;
//	_pshader = NULL;

    if (_puniTextureSampler) delete [] _puniTextureSampler;
    _puniTextureSampler = NULL;
    _numTexSamp = 0;

	_shader._Free();
}

bool
cObject::_SetMesh(cMesh *pmesh)
{
	_pmeshes = pmesh;

	return true;
}

bool
cObject::_SetUniformMVPLocation(void)
{
	_shader._Bind(true);
		_uniMVP = glGetUniformLocation(_shader._GetProgramObject(), "MVP");
	_shader._Bind(false);
	return true;
}

bool
cObject::_SetUniformTextureSamplers(void)
{
	if (!_pmeshes)	return false;
	if (!(_numTexSamp = _pmeshes->_GetNumTextures()))	return false;

	_puniTextureSampler = ::new int[_numTexSamp];

    char    buffer[4] = { 0 };
    unsigned buffsize;
    string str;

	_shader._Bind(true);
		for (int i = 0; i < _numTexSamp; ++i)
		{
			buffsize = sprintf(buffer, "%d", i);
			str = buffer;

			switch (buffsize)
			{
				case 1:
					str = "tex00" + str;
				break;

				case 2:
					str = "tex0" + str;
				break;

				case 3:
					str = "tex" + str;
				break;

				default:
					continue;
			}

			_puniTextureSampler[i] =
				glGetUniformLocation(_shader._GetProgramObject(), str.c_str());
			glActiveTexture(GL_TEXTURE0 + i);
			_pmeshes->_GetTexture((eVBT)((int)BUFF_VERT_TEXCOORD0 + i))->_Bind(true);
			glUniform1i(_puniTextureSampler[i], i);
		}
	_shader._Bind(false);

	return true;
}


bool
cObject::_BindVertexAttribs(bool bbindVAO)
{
	if (!_pmeshes)
		return false;

	return _pmeshes->_BindVertexAttribs(bbindVAO);
}

bool
cObject::_BindTextures(bool bbindtextures)
{
	if (!_numTexSamp)	return false;
	if (bbindtextures)
	{
		for (int i = 0; i < _numTexSamp; ++i)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			_pmeshes->_GetTexture((eVBT)((int)BUFF_VERT_TEXCOORD0 + i))->_Bind(true);
//			glUniform1i(_puniTextureSampler[i], i);
		}
	}
	else
	{
		for (int i = _numTexSamp-1; i >= 0; --i)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			_pmeshes->_GetTexture((eVBT)((int)BUFF_VERT_TEXCOORD0 + i))->_Bind(false);
//			glUniform1i(_puniTextureSampler[i], i);
		}
	}

	return true;
}

bool
cObject::_Reset(void)
{
	_model = glm::mat4(1.f);
//	_modelSIMD = glm::simdMat4(1.f);

    return true;
}

bool
cObject::_Move(glm::vec3 position)
{
	_position = position;

	_model = glm::translate(_model, _position);

	return true;
}

bool
cObject::_MoveRel(glm::vec3 addPosition)
{
	_position += addPosition;

	return _Move(_position);//_model = glm::translate(_model, _position);
}

bool
cObject::_Rotate(glm::vec3 degAngles)
{
	_rotation = degAngles;

	if (degAngles.z)
        _model = glm::rotate(_model, _rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    if (degAngles.x)
        _model = glm::rotate(_model, _rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    if (degAngles.y)
        _model = glm::rotate(_model, _rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));

	return true;
}

bool
cObject::_RotateRel(glm::vec3 addRotation)
{
	_rotation += addRotation;

	return _Rotate(_rotation);
}

/*
bool
cObject::_RotateSIMD(float degAngle, glm::bvec3 axis)
{
	glm::mat4	rotation = glm::mat4(1.f);

	if (axis.z)
	{
		_rotation.z = degAngle;
		glm::rotate(rotation, degAngle, glm::vec3(0.f, 0.f, 1.f));
	}
	if (axis.x)
	{
		_rotation.x = degAngle;
		glm::rotate(rotation, degAngle, glm::vec3(1.f, 0.f, 0.f));
	}
	if (axis.y)
	{
		_rotation.y = degAngle;
		glm::rotate(rotation, degAngle, glm::vec3(0.f, 1.f, 0.f));
	}

	_modelSIMD *= glm::simdMat4(rotation);

	return true;
}
*/
/*  taken out 10/18/2012
bool
cObject::_RotateXSIMD(float degAngle)
{
	_rotation.x = degAngle;

	glm::mat4	rotation = glm::mat4(1.f);

	rotation = glm::rotate(rotation, degAngle, glm::vec3(1.f, 0.f, 0.f));

	_modelSIMD *= glm::simdMat4(rotation);

	return true;
}

bool
cObject::_RotateYSIMD(float degAngle)
{
	_rotation.y = degAngle;

	glm::mat4	rotation = glm::mat4(1.f);

	rotation = glm::rotate(rotation, degAngle, glm::vec3(0.f, 1.f, 0.f));

	_modelSIMD *= glm::simdMat4(rotation);

	return true;
}

bool
cObject::_RotateZSIMD(float degAngle)
{
	_rotation.z = degAngle;

	glm::mat4	rotation = glm::mat4(1.f);

	rotation = glm::rotate(rotation, degAngle, glm::vec3(0.f, 0.f, 1.f));

	_modelSIMD *= glm::simdMat4(rotation);

	return true;
}

bool
cObject::_RotateSIMD(glm::vec3 degAngles)
{
	_rotation = degAngles;

	glm::mat4	rotation = _model;

	if (degAngles.z)
        glm::rotate_slow(rotation, _rotation.z, glm::vec3(0.f,0.f,1.f));
    if (degAngles.x)
        rotation = glm::rotate(rotation, degAngles.x, glm::vec3(1.f,0.f,0.f));
	if (degAngles.y)
        rotation = glm::rotate(rotation, degAngles.y, glm::vec3(0.f,1.f,0.f));


/*
	_model = glm::mat4_cast(_modelSIMD *= glm::simdMat4(rotation));

	return true;
}
*/

bool
cObject::_Scale(glm::vec3 scale)
{
	_scale = scale;

	_model = glm::scale(_model, _scale);

	return true;
}

bool
cObject::_ScaleRel(glm::vec3 addScale)
{
	_scale += addScale;

	return _Scale(_scale);
}

/*  taken out 10/18/2012
bool
cObject::_MoveSIMD(glm::vec3 position)
{
	_position = position;

	glm::mat4 translate = _model;
//	glm::mat4 translate = glm::mat4_cast(_modelSIMD);

	_model = glm::mat4_cast(_modelSIMD *= glm::simdMat4(glm::translate(translate, _position)));

	return true;
}

bool
cObject::_ScaleSIMD(glm::vec3 scale)
{
	_scale = scale;

	glm::mat4 scaleMat = glm::mat4(1.f);

	_modelSIMD *= glm::simdMat4(glm::scale(scaleMat, _scale));

	return true;
}
*/
bool
cObject::_Update(void)
{
//	glm::mat4 MVP = GetProjectionMatrix() * GetViewMatrix() * _GetMatrix();
	glm::mat4 MVP = _pgraphics->_GetViewProjectionMatrix() * _model;

	_shader._Bind(true);
		glUniformMatrix4fv(_uniMVP, 1, false, glm::value_ptr(MVP));
	_shader._Bind(false);

	return true;
}
/*  taken out 10/18/2012
bool
cObject::_UpdateSIMD(void)
{
    glm::simdMat4 VP = _pgraphics->_GetViewProjectionMatrixSIMD();

	glm::mat4 MVP = glm::mat4_cast(VP *= _modelSIMD);

	_shader._Bind(true);
		glUniformMatrix4fv(_uniMVP, 1, false, glm::value_ptr(MVP));
	_shader._Bind(false);

	return true;
}
*/
bool/**/
cObject::_Render(void)
{
	_shader._Bind(true);
		_BindVertexAttribs(true);
		if (_numTexSamp) _BindTextures(true);
			glDrawElements(GL_TRIANGLES,
							_pmeshes->_GetNumTriangles() * THREE_D,
							GL_UNSIGNED_INT, 0);
        if (_numTexSamp) _BindTextures(false);
		_BindVertexAttribs(false);
	_shader._Bind(false);

	return true;
}

/* //PRE-Textures Render().  removed 10/22/2012 11:33 pm during a stromy night at Papa's house with the window open and getting a fresh breeze.
//had a interview to day with Kendall vacuums.  went decently.  enjoyed Sam's group meeting.  AJ told me he wanted to kill Mark White today.
bool
cObject::_Render(void)
{
	_shader._Bind(true);
		_BindVertexAttribs(true);
			glDrawElements(GL_TRIANGLES,
							_pmeshes->_GetNumTriangles() * THREE_D,
							GL_UNSIGNED_INT, 0);
		_BindVertexAttribs(false);
	_shader._Bind(false);

	return true;
}
*/



/*
inline glm::mat4
_GetMatrix(void) const { return _model; }
*/
/*
unsigned int
cObject::_CreateVertexAttrib(void)
{
	unsigned int vertArrayID=0;
	unsigned int numTexs = _numTexs;

	unsigned
	int		buseColor		= _colorBufferID,
			buseTangent 	= _tangentBufferID,
			buseNormal	 	= _normalBufferID,
			buseVertex		= _vertexBufferID,

			buseTriangle	= _triangleBufferID;

	unsigned int
			*buseTexCoord	= new unsigned int[numTexs];

			memcpy(buseTexCoord, _ptexCoordBufferID,
					sizeof(unsigned int)*numTexs);

	glGenVertexArrays(1, &vertArrayID);
	glBindVertexArray(vertArrayID);
		if (buseColor)
		{
//			glBindBuffer(GL_ARRAY_BUFFER, buseColor)
			_BindBuffer(GL_ARRAY_BUFFER, busecolor);
			glVertexAttribPointer(BUFF_VERT_COL, _colorDim, GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(BUFF_VERT_COL);
		}
		if (buseTangent)
		{
//			glBindBuffer(GL_ARRAY_BUFFER, buseTangent)
			_BindBuffer(GL_ARRAY_BUFFER, buseTangent);
			glVertexAttribPointer(BUFF_VERT_TAN, _tangentDim, GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(BUFF_VERT_TAN);
		}
		if (buseNormal)
		{
//			glBindBuffer(GL_ARRAY_BUFFER, buseNormal)
			_BindBuffer(GL_ARRAY_BUFFER, buseNormal);
			glVertexAttribPointer(BUFF_VERT_NORM, _normalDim, GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(BUFF_VERT_NORM);
		}
		if (buseVertex)
		{
//			glBindBuffer(GL_ARRAY_BUFFER, buseVertex)
			_BindBuffer(GL_ARRAY_BUFFER, buseVertex);
			glVertexAttribPointer(BUFF_VERT_POS, _vertexDim, GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(BUFF_VERT_POS);
		}
		if (buseTexCoord[0])
		{
			for (int i=0; i<numTexs; ++i)
			{
//				glBindBuffer(GL_ARRAY_BUFFER, buseTexCoord[i])
				_BindBuffer(GL_ARRAY_BUFFER, buseTexCoord[i]);
				glVertexAttribPointer(BUFF_VERT_TEXCOORD0+i, _texCoordDim, GL_FLOAT, GL_FALSE, 0, NULL);
				glEnableVertexAttribArray(BUFF_VERT_TEXCOORD0+i);
			}
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);

//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buseTriangle);
		_BindBuffer(GL_ELEMENT_ARRAY_BUFFER, buseTriangle);
	glBindVertexArray(0);

	delete[] buseTexCoord;

	return vertArrayID;
}


bool
cMesh::_CreateVertexAttrib(void)
{
	glGenVertexArrays(1, &_vertexAttribArrayID);
	glBindVertexArray(_vertexAttribArrayID);
		if (_colorBufferID)
		{
			_BindBuffer(GL_ARRAY_BUFFER, _colorBufferID);
			glVertexAttribPointer(BUFF_VERT_COL, _colorDim, GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(BUFF_VERT_COL);
		}
		if (_tangentBufferID)
		{
			_BindBuffer(GL_ARRAY_BUFFER, _tangentBufferID);
			glVertexAttribPointer(BUFF_VERT_TAN, _tangentDim, GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(BUFF_VERT_TAN);
		}
		if (_normalBufferID)
		{
			_BindBuffer(GL_ARRAY_BUFFER, _normalBufferID);
			glVertexAttribPointer(BUFF_VERT_NORM, _normalDim, GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(BUFF_VERT_NORM);
		}
		if (_vertexBufferID)
		{
			_BindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
			glVertexAttribPointer(BUFF_VERT_POS, _vertexDim, GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(BUFF_VERT_POS);
		}
		if (_ptexCoordBufferID[0])
		{
			for (int i=0; i<_numTexs; ++i)
			{
				_BindBuffer(GL_ARRAY_BUFFER, _ptexCoordBufferID[i]);
				glVertexAttribPointer(BUFF_VERT_TEXCOORD0+i, _texCoordDim, GL_FLOAT, GL_FALSE, 0, NULL);
				glEnableVertexAttribArray(BUFF_VERT_TEXCOORD0+i);
			}
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		if (_triElemBufferID)
			_BindBuffer(GL_ELEMENT_ARRAY_BUFFER, _triElemBufferID);
	glBindVertexArray(0);

	return true;
}

inline bool
cMesh::_BindVertexAttribs(bool bbindVAO)
{
	bbindVAO : (glBindVertexArray(_vertexAttribArrayID) ? (glBindVertexArray(0);

	return true;
}
*/
