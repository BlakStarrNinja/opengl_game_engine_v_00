#include "core_global.h"

cMesh::cMesh()
{
	_numTexs = 0;
	_numVerts = 0;
	_numTris = 0;

	_colorDim = _tangentDim = _normalDim = _vertexDim = ZERO_D;

	_triElemDim = THREE_D;

	_colorBufferID = _tangentBufferID = _normalBufferID =
	_vertexBufferID = _triElemBufferID = 0;

//	_ptexCoordBufferID = NULL;

//	_ptextures = NULL;
	_pmeshTexs = NULL;

	_vertexAttribArrayID = 0;
}

cMesh::~cMesh()
{
	_Free();

	_numTexs = 0;
	_numVerts = 0;
	_numTris = 0;

	_colorDim = _tangentDim = _normalDim = _vertexDim = ZERO_D;

	_triElemDim = THREE_D;

	_colorBufferID = _tangentBufferID = _normalBufferID =
	_vertexBufferID = _triElemBufferID = 0;

//	_ptexCoordBufferID = NULL;

//	_ptextures = NULL;
	_pmeshTexs = NULL;

	_vertexAttribArrayID = 0;
}

cTexture *
cMesh::_GetTexture(eVBT ebufferIDType)
{
	int index = -1;

	switch (ebufferIDType)
	{
		case BUFF_VERT_TEXCOORD0:
		case BUFF_VERT_TEXCOORD1:
		case BUFF_VERT_TEXCOORD2:
		case BUFF_VERT_TEXCOORD3:
		case BUFF_VERT_TEXCOORD4:
			index = (ebufferIDType-BUFF_VERT_TEXCOORD0);
		break;
	}

	if ((index < 0) || (index >= _numTexs))
		return NULL;

	return _pmeshTexs[index]._ptexture;
}

bool
cMesh::_SetNumTextures(GLuint numTextures)
{
	if (_pmeshTexs)
		delete [] _pmeshTexs;

	_numTexs = numTextures;
	_pmeshTexs = new sMeshTexture[numTextures];

	for (int i=0; i<_numTexs; ++i)
	{
		_pmeshTexs[i]._evat = (eVAT)((int)_pmeshTexs[i]._evat + i);
		_pmeshTexs[i]._evbt = (eVBT)((int)_pmeshTexs[i]._evbt + i);
	}

	return true;
}

bool
cMesh::_SetNumVertices(GLuint numVertices)
{
	_numVerts = numVertices;

	return true;
}

bool
cMesh::_SetNumTriangles(GLuint numTriangles)
{
	_numTris = numTriangles;

	return true;
}

bool
cMesh::_SetTexture(const char *filename, eVBT ebufferIDType)
{
	int index = -1;

	switch (ebufferIDType)
	{
		case BUFF_VERT_TEXCOORD0:
		case BUFF_VERT_TEXCOORD1:
		case BUFF_VERT_TEXCOORD2:
		case BUFF_VERT_TEXCOORD3:
		case BUFF_VERT_TEXCOORD4:
			index = (ebufferIDType-BUFF_VERT_TEXCOORD0);
		break;
	}

	if ((index < 0) || (index >= _numTexs))
		return false;

	_pmeshTexs[index]._ptexture->_Load(filename);

	return _pmeshTexs[index]._ptexture->_IsLoaded();
}

bool
cMesh::_SetTexture(cTexture *pglobalTex, eVBT ebufferIDType)
{
	int index = -1;

	switch (ebufferIDType)
	{
		case BUFF_VERT_TEXCOORD0:
		case BUFF_VERT_TEXCOORD1:
		case BUFF_VERT_TEXCOORD2:
		case BUFF_VERT_TEXCOORD3:
		case BUFF_VERT_TEXCOORD4:
			index = (ebufferIDType-BUFF_VERT_TEXCOORD0);
		break;
	}

	if ((index < 0) || (index >= _numTexs))
		return false;

	_pmeshTexs[index]._bglobalTextureLoaded = true;
	_pmeshTexs[index]._ptexture = pglobalTex;

	return _pmeshTexs[index]._ptexture->_IsLoaded();
}

GLuint
cMesh::_CreateBuffer(	eVBT ebufferIDType, eVD edimension, GLenum buffType,
						GLenum buffPurpose, GLuint buffSize,
						void *data)
{
	unsigned int *pbufferID = NULL;
	int index = -1;

	switch (ebufferIDType)
	{
		case BUFF_VERT_POS:
			pbufferID = &_vertexBufferID;
			_vertexDim = edimension;
		break;

		case BUFF_VERT_COL:
			pbufferID = &_colorBufferID;
			_colorDim = edimension;
		break;

		case BUFF_VERT_NORM:
			pbufferID = &_normalBufferID;
			_normalDim = edimension;
		break;

		case BUFF_VERT_TAN:
			pbufferID = &_tangentBufferID;
			_tangentDim = edimension;
		break;

		case BUFF_VERT_TEXCOORD0:
		case BUFF_VERT_TEXCOORD1:
		case BUFF_VERT_TEXCOORD2:
		case BUFF_VERT_TEXCOORD3:
		case BUFF_VERT_TEXCOORD4:
			index = (ebufferIDType-BUFF_VERT_TEXCOORD0);

			if ((index < 0) || (index >= _numTexs))
				return false;

			pbufferID = &_pmeshTexs[index]._texCoordBufferID;
			_pmeshTexs[index]._edimension = edimension;
		break;

//*****	TODO:	check for or throw exception for non-THREE_D edimension
		case BUFF_VERT_INDEX:
			pbufferID = &_triElemBufferID;
			_triElemDim = THREE_D;//edimension;
		break;
	}

	if (!pbufferID)
	{
		unsigned int temp = 0;
		pbufferID = &temp;
	}
/*****	TODO:	take care of a NULL of 0 buffsize
	switch (ebufferIDType)
	{
		case BUFF_VERT_POS:
		case BUFF_VERT_COL:
		case .....
			if (buffsize == (or !=) _numVerts * edimension * sizeof(GLfloat))
				.......
//	if (buffSize == ()
*/
	glGenBuffers(1, pbufferID);

	glBindBuffer(buffType, *pbufferID);
	glBufferData(buffType, buffSize, data, buffPurpose);
	glBindBuffer(buffType, 0);

	return (*pbufferID);
}



GLuint
cMesh::_UpdateBufferData(	eVBT ebufferIDType, GLenum buffType, GLuint buffSize,
                        void *data)
{
	unsigned int bufferID = 0;
	eVD edimension;
	int index = -1;

	switch (ebufferIDType)
	{
		case BUFF_VERT_POS:
			bufferID = _vertexBufferID;
			edimension = _vertexDim;
		break;

		case BUFF_VERT_COL:
			bufferID = _colorBufferID;
			edimension = _colorDim;
		break;

		case BUFF_VERT_NORM:
			bufferID = _normalBufferID;
			edimension = _normalDim;
		break;

		case BUFF_VERT_TAN:
			bufferID = _tangentBufferID;
			edimension = _tangentDim;
		break;

		case BUFF_VERT_TEXCOORD0:
		case BUFF_VERT_TEXCOORD1:
		case BUFF_VERT_TEXCOORD2:
		case BUFF_VERT_TEXCOORD3:
		case BUFF_VERT_TEXCOORD4:
			index = (ebufferIDType-BUFF_VERT_TEXCOORD0);

			if ((index < 0) || (index >= _numTexs))
				return false;

			bufferID = _pmeshTexs[index]._texCoordBufferID;
			edimension = _pmeshTexs[index]._edimension;
		break;

//*****	TODO:	check for or throw exception for non-THREE_D edimension
		case BUFF_VERT_INDEX:
			bufferID = _triElemBufferID;
            edimension = _triElemDim;//edimension;
		break;
	}

	if (!bufferID)
        return 0;
/*****	TODO:	take care of a NULL of 0 buffsize
	switch (ebufferIDType)
	{
		case BUFF_VERT_POS:
		case BUFF_VERT_COL:
		case .....
			if (buffsize == (or !=) _numVerts * edimension * sizeof(GLfloat))
				.......
//	if (buffSize == ()
*/
	glBindBuffer(buffType, bufferID);
	glBufferSubData(buffType, 0, buffSize, data);
	glBindBuffer(buffType, 0);

	return bufferID;
}

bool
cMesh::_CreateVertexAttribs(void)
{
	glGenVertexArrays(1, &_vertexAttribArrayID);
	glBindVertexArray(_vertexAttribArrayID);
		if (_colorBufferID)
		{
			glBindBuffer(GL_ARRAY_BUFFER, _colorBufferID);
			glVertexAttribPointer(VERT_COL, _colorDim, GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(VERT_COL);
		}
		if (_tangentBufferID)
		{
			glBindBuffer(GL_ARRAY_BUFFER, _tangentBufferID);
			glVertexAttribPointer(VERT_TAN, _tangentDim, GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(VERT_TAN);
		}
		if (_normalBufferID)
		{
			glBindBuffer(GL_ARRAY_BUFFER, _normalBufferID);
			glVertexAttribPointer(VERT_NORM, _normalDim, GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(VERT_NORM);
		}
		if (_vertexBufferID)
		{
			glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
			glVertexAttribPointer(VERT_POS, _vertexDim, GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(VERT_POS);
		}
		if (_pmeshTexs/*[0]._texCoordBufferID*/)
		{
			for (GLuint i=0; i<_numTexs; ++i)
			{
				glBindBuffer(GL_ARRAY_BUFFER, _pmeshTexs[i]._texCoordBufferID);
				glVertexAttribPointer(_pmeshTexs[i]._evat, _pmeshTexs[i]._edimension, GL_FLOAT, GL_FALSE, 0, NULL);
				glEnableVertexAttribArray(_pmeshTexs[i]._evat);
			}
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		if (_triElemBufferID)
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _triElemBufferID);
	glBindVertexArray(0);

	return true;
}

bool
cMesh::_Free(void)
{
	if (_colorBufferID)
		glDeleteBuffers(1, &_colorBufferID);

	if (_tangentBufferID)
		glDeleteBuffers(1, &_tangentBufferID);

	if (_normalBufferID)
		glDeleteBuffers(1, &_normalBufferID);

	if (_vertexBufferID)
		glDeleteBuffers(1, &_vertexBufferID);

	if (_triElemBufferID)
		glDeleteBuffers(1, &_triElemBufferID);

	if (_pmeshTexs)
	{
		for (int i=0; i<_numTexs; ++i)
		{
			glDeleteBuffers(1, &_pmeshTexs[i]._texCoordBufferID);
			_pmeshTexs[i]._texCoordBufferID = 0;
			if (!_pmeshTexs[i]._bglobalTextureLoaded)
			{
				_pmeshTexs[i]._ptexture->_Free();
				_pmeshTexs[i]._ptexture = NULL;
			}
		}
	}
/*
	if (_ptexCoordBufferID)
	{
		for (int i=0; i<_numTexs; ++i)
		{
			glDeleteBuffers(1, &_ptexCoordBufferID[i]);
		}
	}
*/
	if (_vertexAttribArrayID)
		glDeleteVertexArrays(1, &_vertexAttribArrayID);

	return true;
}

bool
cMesh::_BindBuffer(eVBT ebufferIDType, GLenum buffType)
{
	GLuint	buffID = 0;
	int index = -1;

	switch (ebufferIDType)
	{
		case	NONE:
			buffID = 0;
		break;

		case	BUFF_VERT_POS:
			buffID = _vertexBufferID;
		break;

		case	BUFF_VERT_COL:
			buffID = _colorBufferID;
		break;

		case	BUFF_VERT_NORM:
			buffID = _normalBufferID;
		break;

		case	BUFF_VERT_TAN:
			buffID = _tangentBufferID;
		break;

		case BUFF_VERT_TEXCOORD0:
		case BUFF_VERT_TEXCOORD1:
		case BUFF_VERT_TEXCOORD2:
		case BUFF_VERT_TEXCOORD3:
		case BUFF_VERT_TEXCOORD4:
			index = (ebufferIDType-BUFF_VERT_TEXCOORD0);

			if ((index < 0) || (index >= _numTexs))
				return false;

			buffID = _pmeshTexs[index]._texCoordBufferID;
		break;

		case	BUFF_VERT_INDEX:
			buffID = _triElemBufferID;
			buffType = GL_ELEMENT_ARRAY_BUFFER;
		break;
	}

	glBindBuffer(buffType, buffID);

	return true;
}

bool
cMesh::_BindVertexAttribs(bool bbindVAO)
{
	bbindVAO ? glBindVertexArray(_vertexAttribArrayID) : glBindVertexArray(0);

	return true;
}

