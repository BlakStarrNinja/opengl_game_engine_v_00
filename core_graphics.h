#ifndef _CORE_GRAPHICS_H_
#define _CORE_GRAPHICS_H_

/*
#include <windows.h>

#include <gl/glew.h>
#include <gl/wglew.h>
*/

class cGraphics;
/*
class cShader;
class cVertexArrayObject;

class cTexture;
class cMaterial;
class cLight;
class cFont;
class cVertexBuffer;

class cWorldPosition;
class cCamera;
class cMesh;
class cobject;
class cAnimation;

struct sMesh;
struct sFrame;
struct sAnimation;
struct sAnimationSet;
*/

class cGraphics
{
	protected:      //  app/system variables
//		HWND			_hWnd;
		HDC				_hDC;
		HGLRC			_hRC;

//		DEVMODE			_dm;

		bool			_bfullscreen;
		bool			_buseDepth;

		long			_width;
		long			_height;
		unsigned char	_bPP;

    protected:      //  graphics state variables
		glm::mat4		_projection;    //a.k.a.: the Perspective Projection Matrix
		glm::mat4		_orthogonal;
		glm::mat4		_view;

//		glm::simdMat4	_projectionSIMD;
//		glm::simdMat4	_viewSIMD;

		float _clearColor[4];

		float			_ambientRed;
		float			_ambientGreen;
		float			_ambientBlue;
		float			_ambientAlpha;

//    public:
	protected:
		bool			_CreateRenderContext(HDC hdc);
		bool			_DeleteRenderContext(void);

	public:
		cGraphics();
		~cGraphics();

		bool	_Init(void);
		bool	_Shutdown(void);

		bool	_SetMode(HDC hdc, bool bfullscreenflag = false, bool buseDepthflag = true, long width = 800, long height = 600, unsigned char bpp = 32);
		virtual bool	_InitialStates(void);
//		long	_GetNumDisplayModes(/* parameters */);
//		bool	_GetDisplayModeInfo(long number, DEVMODE *pdm, PIXELFORMAT pdf);

//		char	_GetFormatBPP(PIXELFORMAT pdf);
//		bool	_CheckFormat(PIXELFORMAT pdf, bool bfullscreen);
/*
		bool	_Display(void);

		bool	_BeginScene(void);
		bool	_EndScene(void);

		bool	_Begin2D(void);
		bool	_End2D(void);
*/
//		bool	_Clear(float *pclearColor, float depthBuf = 1.0f);
//		bool	_ClearDisplay(float *pclearColor);
//		bool	_ClearDepthBuffer(float depthBuf = 1.0f);

		inline long			_GetWidth(void) const { return _width; }
		inline long			_GetHeight(void) const { return _height; }
		inline unsigned char	_GetBPP(void) const { return _bPP; }
        bool			        _IsDepthBuffer(void);

//		bool	_SetPerspective(float FOV = PI / 4.0f, float aspectRatio = 1.3333333333f, float near = 1.0f, float far = 10000.0f);

		inline glm::mat4	_GetProjectionMatrix(void) const { return _projection; }
		inline glm::mat4	_GetOrthogonalMatrix(void) const { return _orthogonal; }
		inline glm::mat4	_GetViewMatrix(void) const { return _view; }
		inline glm::mat4	_GetViewProjectionMatrix(void) const { return (_projection * _view); }

//		inline glm::simdMat4	_GetViewProjectionMatrixSIMD(void) const { glm::simdMat4 P = _projectionSIMD; return  P *= _viewSIMD; }

        bool    _SetWidth(long width);
        bool    _SetHeight(long width);

		bool	_SetPerspective(float degFOV, float aspect, float znear, float zfar);
		bool	_SetOrthogonal(float left, float right, float bottom, float top, float znear, float zfar);
		bool	_Clear(void);
		bool	_ClearDisplay(void);
		bool	_ClearDepthBuffer(void);
		bool	_SetClearColor(float color[4]);
		bool	_SetClearColor(float red, float blue, float green, float alpha);

//		bool	_SetPerspectiveSIMD(float degFOV, float aspect, float znear, float zfar);


//		bool	_SetWorldPostion(cWorldPosition *pworldPos);
//		bool	_SetCamera(cCamera *pcamera);
//		bool	_SetLight(long number, cLight *plight);
//		bool	_SetMaterial(cMaterial *pmaterial);
//		bool	_SetTexture(short number, cTexture *ptexture);

//		bool	_GetAmbientLight(char *pred, char *pgreen, char *pblue);
//		bool	_SetAmbientLight(char red, char green, char blue);

//		bool	_EnableLight(long lightID, bool benable = true);
//		bool	_EnableLighting(bool benable = true);
//		bool	_EnableDepthBuffer(bool benable = true);
//		bool	_EnableAlphaBlending(bool benable = true, long src, long dest);	//****	TODO:	set src and dest	****//
//		bool	_EnableAlphaTesting(bool benable = true);
};
/*
class cTexture
{

};
*/
#endif // _CORE_GRAPHICS_H_
