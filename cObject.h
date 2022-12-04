#ifndef _COBJECT_H_
#define _COBJECT_H_

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
	NONE	=	0,
    ONED	=	1,
    ONE_D   =   1,
    TWOD    =   2,
    TWO_D   =   2,
    THREED  =   3,
    THREE_D =   3,
    FOURD   =   4,
    FOUR_D  =   4
} eVecDim, eVD;

/*
typedef union _tagVertex
{
	float _vert2D[2];
	float _vert3D[3];
	float _vert4D[4];
}vert, Vertex;

typedef union _tagTexureCoordinate
{
	float _texCoord2D[2];
	float _texCoord3D[3];
	float _texCoord4D[4];
}texCoord, TexCoord;

typedef union _tagColor
{
	float _color2D[2];
	float _color3D[3];
	float _color4D[4];
}color, Color;


typedef struct _tagTriangle
{
	unsigned long _vertexIndices[3];
}tri, Triangle;
*//*
class cMesh
{
	public:
		unsigned long	_numVertices;
		unsigned long	_colorBufferID;
//		Color			*_pcolors;
		unsigned long	_texCoordBufferID;
//		TexCoord		**_ptexCoords;
		unsigned long	_vertexBufferID;
//		Vertex			*_pvertices;

		unsigned long	_numTriangles;
		unsigned long	_triElemBufferID;
		Triangle		*_ptriangles;

		unsigned long	_numTextures;
		cTextures		*_ptextures;

	public:
		bool	_CreateBuffer(unsigned int *bufferID, unsigned long bufferType,
								unsigned long bufferPurpose, unsigned long bufferSize,
								void *data);
		bool	_BindBuffer(unsigned long bufferID);
		bool	_DeleteBuffer(unsigned long bufferID);
		bool	_Free(void);
};
*/

class cObject
{
	protected:
//		unsigned long	_numMeshes;
		cGraphics	*_pgraphics;
		cMesh		*_pmeshes;

		glm::mat4		_model;
//		glm::simdMat4	_modelSIMD;

		glm::vec3	_position;
		glm::vec3	_rotation;
		glm::vec3	_scale;

//	public:
		cShader		_shader;

        long        _numTexSamp;
        GLint       *_puniTextureSampler;

		GLint       _uniMVP;

	public:
		cObject();
		~cObject();

		bool	_Create(cGraphics *pgraphics, cMesh *pmesh);
		bool	_Free(void);

		bool _SetMesh(cMesh *pmesh);
		bool _SetUniformMVPLocation(void);
		bool _SetUniformTextureSamplers(void);


		inline cShader *	_GetShader(void) { return &_shader; }

		inline glm::mat4	_GetMatrix(void) const { return _model; }
		inline glm::vec3	_GetPosition(void) const { return _position; }
		inline glm::vec3	_GetRotation(void) const { return _rotation; }
		inline glm::vec3	_GetScale(void) const { return _scale; }

//		glm::mat4   _GetMatrixCon(void) const { return glm::mat4_cast(_modelSIMD); }

		bool    _Reset(void);

		bool	_Move(glm::vec3 position);
		bool	_MoveRel(glm::vec3 addPosition);
		bool	_Rotate(glm::vec3 rotation);
		bool	_RotateRel(glm::vec3 addRotation);
		bool	_Scale(glm::vec3 scale);
		bool	_ScaleRel(glm::vec3 addScale);
/*
//		bool	_RotateSIMD(float degAngle, glm::bvec3 axis);
		bool	_RotateXSIMD(float degAngle);
		bool	_RotateYSIMD(float degAngle);
		bool	_RotateZSIMD(float degAngle);
		bool	_RotateSIMD(glm::vec3 degAngles);

		bool	_MoveSIMD(glm::vec3 position);
		bool	_ScaleSIMD(glm::vec3 scale);
*/
		bool    _BindVertexAttribs(bool bbindVAO);	    //maybe move to protect:
		bool    _BindTextures(bool bbindtextures);     //maybe move to protect:

		virtual bool	_Update(void);
		virtual bool	_Render(void);

//		virtual bool	_UpdateSIMD(void);
};

#endif	//_COBJECT_H_
