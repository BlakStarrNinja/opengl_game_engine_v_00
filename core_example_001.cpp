#include "core_global.h"

class cApp : public cApplication
{
        cGraphics _Graphics;

        bool _Render(void);

    public:
        cApp();

        bool _Init(void);
        bool _Shutdown(void);
        bool _Frame(void);
};

cApp::cApp()
{
    _appWndWidth = 1280;
    _appWndHeight = 720;

    _appFullWidth = 1280;
    _appFullHeight = 720;
}

bool cApp::_Init(void)
{
    _Graphics._Init();
    _Graphics._SetMode(_GethWnd(), _IsFullscreen(), true, _GetWidth(), _GetHeight(), _GetBPP());
    _Graphics._CreateRenderContext(_GethDC());
    _Graphics._InitialStates();

    return true;
}

bool cApp::_Shutdown(void)
{
    _Graphics._Shutdown();

    return true;
}

bool cApp::_Render(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
