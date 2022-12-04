#include "core_global.h"
//#include "core_graphics.h"

cGraphics::cGraphics()
{
//	_hWnd = NULL;
    _hDC = NULL;
	_hRC = NULL;

	_bfullscreen = false;
	_buseDepth = true;

	_width = 0;
	_height = 0;
	_bPP = 0;

	_projection = _view = glm::mat4(1.0f);
//	_projectionSIMD = _viewSIMD = glm::simdMat4(1.f);

	_clearColor[0] = _clearColor[1] = _clearColor[2]  = 0.f;
    _clearColor[3] = 1.0f;

	_ambientRed		= _ambientBlue  =
	_ambientGreen	= _ambientAlpha = 1.0f;
}

cGraphics::~cGraphics() { _Shutdown(); }

bool
cGraphics::_Init(void)
{
	_Shutdown();

	ilInit();
	iluInit();
	ilutRenderer(ILUT_OPENGL);

	/** Added Dec. 23 2018 at 4:32PM after learning that simply enabling this
         flag makes texture NOT be flipped depending on the file format.

        This flag seems to notify DevIL / OpenIL that the origin of the file
         actually MATTERS.
    ***/
    ilEnable(IL_ORIGIN_SET);

	return true;
}

bool cGraphics::_Shutdown(void)	{ return _DeleteRenderContext(); }

bool
cGraphics::_SetMode(HDC hdc, bool bfullscreenflag, bool buseDepthflag, long width, long height, unsigned char bpp)
{
//	_hWnd = hWnd;
    _hDC = hdc;
	_bfullscreen = bfullscreenflag;
	_buseDepth = buseDepthflag;
	_width = width;
	_height = height;
	_bPP = bpp;

    _CreateRenderContext(_hDC);

	return true;
}

bool
cGraphics::_CreateRenderContext(HDC hdc)//, HGLRC hrc)
{
	if (!hdc)
		return false;

	//create fake context to allow creation of real context
	HGLRC 					temphrc;
	PIXELFORMATDESCRIPTOR	pfd;
	long					formatID;

	ZeroMemory(&pfd, sizeof(pfd));

	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags =	PFD_DRAW_TO_WINDOW |
					PFD_SUPPORT_OPENGL |
					PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = _bPP;
	pfd.cDepthBits = 24;
	pfd.iLayerType = PFD_MAIN_PLANE;

	formatID	=	ChoosePixelFormat(hdc, &pfd);

	SetPixelFormat(hdc, formatID, &pfd);

	temphrc	=	wglCreateContext(hdc);

	//created fake context, now begin creating core profile rendering context
	wglMakeCurrent(hdc, temphrc);

	glewInit();

	int attribs[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 3,
		WGL_CONTEXT_FLAGS_ARB,
		WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
		WGL_CONTEXT_PROFILE_MASK_ARB,
		WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0
	};

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(temphrc);

	_hRC = wglCreateContextAttribsARB(hdc, NULL, attribs);

	wglMakeCurrent(hdc, _hRC);

	return true;
}

bool
cGraphics::_DeleteRenderContext(void)
{
	if (_hRC)
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(_hRC);
		_hRC = NULL;
	}

	return true;
}

bool
cGraphics::_InitialStates(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	return true;
}

bool
cGraphics::_SetWidth(long width) { _width = width; }

bool
cGraphics::_SetHeight(long height) { _height = height; }

bool
cGraphics::_SetPerspective(float degFOV, float aspect, float znear, float zfar)
{
	_projection = glm::perspective  (   degFOV,
                                        aspect,
                                        znear,
                                        zfar
                                    );

	return true;
}

bool
cGraphics::_SetOrthogonal(float left, float right, float bottom, float top, float znear = -1, float zfar = 1)
{
	_orthogonal = glm::ortho(left, right, bottom, top, znear, zfar);

	return true;
}

/*
bool
cGraphics::_SetPerspectiveSIMD(float degFOV, float aspect, float znear, float zfar)
{
 //   glm::mat4 projection(1.f);

 //   projection = glm::perspective((float)degFOV, (float)aspect, (float)znear, (float)zfar);

//	_projectionSIMD = glm::simdMat4(glm::perspective(degFOV, aspect, znear, zfar));

	return true;
}
*/

bool
cGraphics::_Clear(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return true;
}

bool
cGraphics::_ClearDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	return true;
}

bool
cGraphics::_ClearDepthBuffer(void)
{
	glClear(GL_DEPTH_BUFFER_BIT);

	return true;
}

bool
cGraphics::_SetClearColor(float color[4])
{
	memcpy(_clearColor, color, sizeof(float)*4);

	glClearColor(_clearColor[0], _clearColor[1], _clearColor[2], _clearColor[3]);

	return true;
}

bool
cGraphics::_SetClearColor(float red, float blue, float green, float alpha)
{
	_clearColor[0] = red;
	_clearColor[1] = blue;
	_clearColor[2] = green;
	_clearColor[3] = alpha;

	glClearColor(_clearColor[0], _clearColor[1], _clearColor[2], _clearColor[3]);
}

