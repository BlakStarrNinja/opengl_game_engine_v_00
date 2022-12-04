#include "core_global.h"

class cApp : public cApplication
{
        cGraphics   _Graphics;
        cObj        _ObjTri;

        bool _Render(void);

    public:
        cApp();

        bool _Init(void);
        bool _Shutdown(void);
        bool _Frame(void);
};

cApp::cApp()
{
    _appWndWidth = 256;
    _appWndHeight = 256;

//   _appFullWidth = 640;
//    _appFullHeight = 480;
}

bool cApp::_Init(void)
{
    _Graphics._Init();
    _Graphics._SetMode(_GethDC(), _IsFullscreen(), true, _GetWidth(), _GetHeight(), _GetBPP());
//    _Graphics._CreateRenderContext(_GethDC());
    _Graphics._InitialStates();

    _ObjTri._Init();
    _ObjTri._GetShader()->_CreateShaderObject("minimal.vert.glsl", VERT_SHADER);
    _ObjTri._GetShader()->_CreateShaderObject("minimal.frag.glsl", FRAG_SHADER);
    _ObjTri._GetShader()->_CreateProgram();

    return true;
}

bool cApp::_Shutdown(void)
{
    _ObjTri._Shutdown();

    _Graphics._Shutdown();

    return true;
}

bool cApp::_Render(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _ObjTri._Render();

    return true;
}

bool cApp::_Frame(void)
{
    _Render();
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _SwapBuffers();

    return true;
}

int PASCAL WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR szCmdLine, int nCmdShow)
{
	cApp App;
	return App._Run();
}
