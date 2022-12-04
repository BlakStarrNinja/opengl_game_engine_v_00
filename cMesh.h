#ifndef	_CMESH_H_
#define	_CMESH_H_

/*
typedef enum _tageVertexAttributeType
{
    VERT_POS	=	0,
    VERT_COL	=	1,
    VERT_NORM,
	VERT_TAN,
    VERT_TEXCOORD0,
    VERT_TEXCOORD1,
    VERT_TEXCOORD2,
    VERT_TEXCOORD3,
    VERT_TEXCOORD4
} eVertAttType, eVAT;

typedef enum _tageVertexBufferType
{
	NONE				=	0,
    BUFF_VERT_POS		= 100,
    BUFF_VERT_COL		= 101,
    BUFF_VERT_NORM		= 102,
	BUFF_VERT_TAN		= 103,
    BUFF_VERT_TEXCOORD0	= 104,
    BUFF_VERT_TEXCOORD1	= 105,
    BUFF_VERT_TEXCOORD2	= 106,
    BUFF_VERT_TEXCOORD3	= 107,
    BUFF_VERT_TEXCOORD4	= 108,

	BUFF_VERT_INDEX		=  32
}eVertBuffType, eVBT;

typedef enum _tageVectorDimension
{
	ZERO_D	=	0,
	ZEROD   =   0,
    ONED	=	1,
    ONE_D   =   1,
    TWOD    =   2,
    TWO_D   =   2,
    THREED  =   3,
    THREE_D =   3,
    FOURD   =   4,
    FOUR_D  =   4
} eVecDim, eVD;
*/

class cMesh
{
	protected:
		struct sMeshTexture
		{
			bool	_bglobalTextureLoaded;
			eVAT	_evat;
			eVBT	_evbt;
			eVecDim	_edimension;
			GLuint	_texCoordBufferID;
			cTexture *_ptexture;

			sMeshTexture()
			{
				_bglobalTextureLoaded = false;

				_evat	=	VERT_TEXCOORD0;
				_evbt	=	BUFF_VERT_TEXCOORD0;
				_edimension	=	TWO_D;
				_texCoordBufferID = 0;
				_ptexture = NULL;
			}

			~sMeshTexture()
			{
				if (_texCoordBufferID)
				{
					glDeleteBuffers(1, &_texCoordBufferID);
					_texCoordBufferID = 0;
				}

				if (_ptexture)
					if (!_bglobalTextureLoaded)
					{
						_ptexture->_Free();
						_ptexture = NULL;
					}
			}
		};

	protected:
//		GLuint _numMats;
		GLuint _numTexs;
		GLuint _numVerts;
		GLuint _numTris;

/*
		sColor		*_pcolors;
		sTangent	*_ptangents;
		sNormal		*_pnormals;
		sTexCoord	**_pptexCoords;
		sVertex		*_pvertices;
*/
		eVecDim	_colorDim,
				_tangentDim,
				_normalDim,
				_vertexDim,
				_triElemDim;

		GLuint _colorBufferID;
		GLuint _tangentBufferID;
		GLuint _normalBufferID;
		GLuint _vertexBufferID;

		GLuint _triElemBufferID;

		sMeshTexture	*_pmeshTexs;

		GLuint _vertexAttribArrayID;

	public:
		cMesh();
		~cMesh();

		inline GLuint	_GetNumTextures(void) const { return _numTexs; }
		inline GLuint	_GetNumVertices(void) const { return _numVerts; }
		inline GLuint	_GetNumTriangles(void) const { return _numTris; }
		cTexture		*_GetTexture(eVBT ebufferIDType);

		bool	_SetNumTextures(GLuint numTextures);
		bool	_SetNumVertices(GLuint numVertices);
		bool	_SetNumTriangles(GLuint numTriangles);
		bool	_SetTexture(const char *filename, eVBT ebufferIDType = BUFF_VERT_TEXCOORD0);
		bool	_SetTexture(cTexture *pglobalTex, eVBT ebufferIDType = BUFF_VERT_TEXCOORD0);


		GLuint	_CreateBuffer(	eVBT ebuffIDType, eVD edimension, GLenum buffType,
								GLenum buffPurpose, GLuint buffSize,
								void *data);

		GLuint	_UpdateBufferData(	eVBT ebuffIDType, GLenum buffType, GLuint buffSize,
								void *data);
		bool	_CreateVertexAttribs(void);
		bool	_Free(void);
		bool	_BindBuffer(eVBT ebufferIDType, GLenum buffType = GL_ARRAY_BUFFER);
		bool	_BindVertexAttribs(bool bbindVAO);
/*
		float *_pcolors;
		float *_ptangents;
		float *_pnormals;
		float **_pptexCoords;
		float *_pvertices;
*/
};

/*
struct sColor
{
	float _pcolor[4];
};

struct sTangent
{
	float _ptangent[4];
};

struct sNormal
{
	float _pnormal[3];
};

struct sTexCoord
{
	float _texCoord[2];
}

struct sVertex
{
	float _pvertex[3];
};

struct sTriangle
{
	unsigned long _triangle[3];
};
*/

#endif	//_CMESH_H_

/*
struct sMeshTexture
{
	bool	_bglobalTextureLoaded;
	eVAT	evat;
	eVBT	evbt;
	eVecDim	_edimension;
	GLuint	_texCoordBufferID;
	cTexture *_ptexure;

	sMeshTexture()
	{
		_bglobalTextureLoaded = false;

		evat	=	VERT_TEXCOORD0;
		evbt	=	BUFF_VERT_TEXCOORD0;
		edimension	=	TWO_D;
		_texCoordBufferID = 0;
		_ptexture = NULL;
	}

	~sMeshTexture()
	{
		if (_ptexture)
			if (!_bglobalTextureLoaded)
				_ptexture->_Free();

		_ptexture = NULL;
	}
}

sMeshTexture *_pmeshTexs = NULL;
_pmeshTexs = new sMeshTexture[_numTexs];
_pmeshTexs[0]

settexture(char filename, evbt ebufferIDType)
{
switch (ebufferIDType)
{
	//.................
	case BUFF_VERT_TEXCOORD0:
	case BUFF_VERT_TEXCOORD1:
	case BUFF_VERT_TEXCOORD2:
	case BUFF_VERT_TEXCOORD3:
	case BUFF_VERT_TEXCOORD4:
		if ((int temp = ebufferIDType - BUFF_VERT_TEXCOORD0) >= _numTexs)
			break;
	break;

	default:	//
}

	if (!_pmeshTexs[temp].ptexture)
			return false;

	return _pmeshTexs[temp].ptexture->_Load(filename);
}

settexture(cTexture *pglobalTex, evbt ebufferIDType)
{
switch (ebufferIDType)
{
	//.................
	case BUFF_VERT_TEXCOORD0:
	case BUFF_VERT_TEXCOORD1:
	case BUFF_VERT_TEXCOORD2:
	case BUFF_VERT_TEXCOORD3:
	case BUFF_VERT_TEXCOORD4:
		if ((int temp = ebufferIDType - BUFF_VERT_TEXCOORD0) >= _numTexs)
			break;
	break;

	default:	//
}

	if (_pmeshTexs[temp].ptexture->isloaded())
			return false;

	_pmeshTexs[temp]._bbglobalTextureLoaded = true;
	_pmeshTexs[temp].ptexture = pglobalTex;

	return true;
}
*/
