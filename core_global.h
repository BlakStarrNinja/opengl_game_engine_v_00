#ifndef _CORE_GLOBALS_H_
#define _CORE_GLOBALS_H_

#define null NULL

#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <unordered_map>


#define GLEW_STATIC
#include <gl/glew.h>
#include <gl/wglew.h>

#define _UNICODE
#include <il/il.h>
#include <il/ilu.h>
#include <il/ilut.h>
#undef _UNICODE

#define GLM_FORCE_MESSAGES
#define GLM_FORCE_SWIZZLE
#define GLM_FORCE_SSE2
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtx/simd_vec4.hpp>
//#include <glm/gtx/simd_mat4.hpp>


#include "core_system.h"
#include "core_graphics.h"

#include "cTimer.h"
#include "cShader.h"
#include "cTexture.h"


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
//    NONE    =   0,
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


#include "cMesh.h"
#include "cObject.h"

#include "cFont.h"

//#include "temp_cObject.h"

const int MBOKEX = MB_OK | MB_ICONWARNING;
#define _MB(mess, capt) ::MessageBox(NULL, mess, capt, MBOKEX);

const double PI	= 3.1415926535897932384626433832795;
#define DEGTORAD(degangle) (degangle * 0.017453292519943295769236907684886)
#define RADTODEG(radangle) (radangle * 57.295779513082320876798154814105)

inline
const wchar_t *
gILConvertMBtoWC(const char *operand)
{
    static wchar_t szbuffer[2048];
    mbstowcs(szbuffer, operand, sizeof(szbuffer));

    return szbuffer;
}

inline
const char *
gILConvertWCtoMB(const wchar_t *operand)
{
    static char szbuffer[2048];
    wcstombs(szbuffer, operand, sizeof(szbuffer));

    return szbuffer;
}

using namespace std;

#endif //_CORE_GLOBALS_H_
