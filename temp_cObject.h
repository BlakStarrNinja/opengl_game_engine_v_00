#ifndef _TEMP_COBJECT_H_
#define _TEMP_COBJECT_H_

#include <memory.h>
#include "cShader.h"

#define MAX_VERTS	3
/*
typedef enum _tageVertexAttributeType
{
    VERT_POS	=	0,
    VERT_COL	=	1,
    VERT_NORM,
	VERT_TANG,
    VERT_TEXCOORD0,
    VERT_TEXCOORD1,
    VERT_TEXCOORD2,
    VERT_TEXCOORD3,
    VERT_TEXCOORD4
} eVertAttType, eVAT;

typedef enum _tageVectorDimension
{
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
class cObject
{
	protected:
		unsigned long	_vboID[2];

		float	_vertArray[MAX_VERTS][THREE_D],
				_colArray[MAX_VERTS][THREE_D];

		cShader	_shader;

	public:
        cObject();

		cShader *	_GetShader(void);

		virtual void _Init(void);
		virtual void _Shutdown(void);
		virtual void _Render(void);
};

class cObj : public cObject
{
    public:
        void _Init();
        void _Shutdown();
        void _Render();
};

#endif //_TEMP_COBJECT_H_
