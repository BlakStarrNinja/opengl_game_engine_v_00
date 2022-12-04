//*******	TODO:	Define Get()'s and Set()'s, new variables, and newfunctions
//*******	DONE:	3/8/2011

//#include "core_system.h"
#include "core_global.h"

cApplication::cApplication()
{
	g_pApplication = this;

	_hInst = GetModuleHandle(NULL);

	strcpy(_szwndClassName, "AppClass");
	strcpy(_szwndCaption, "Rani's App Window");

	_wcex.cbSize		= sizeof(WNDCLASSEX);
	_wcex.style			= CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	_wcex.lpfnWndProc	= (WNDPROC)AppWndProc;
	_wcex.cbClsExtra	= 0;
	_wcex.cbWndExtra	= 0;
	_wcex.hInstance		= _hInst;
	_wcex.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	_wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	_wcex.hbrBackground	= (HBRUSH)(COLOR_APPWORKSPACE);
	_wcex.lpszMenuName	= NULL;
	_wcex.lpszClassName	= _szwndClassName;
	_wcex.hIconSm		= LoadIcon(NULL, IDI_APPLICATION);

	_appWndPosX = _appWndPosY = 0;
	_appWndWidth	= 800;
	_appWndHeight	= 600;
	_appFullWidth	= 1920;
	_appFullHeight	= 1080;
/*
	_appFullWidth	= 1440;
	_appFullHeight	= 900;
*/
	_bPP			= 32;

	_bfullscreen	= false;
	_bprogLooping	= false;
}

bool
cApplication::_CreateWindow(void)
{
	DWORD 	wndStyle = WS_OVERLAPPEDWINDOW,
			wndExStyle = WS_EX_APPWINDOW;

	if (_bfullscreen)
	{
		if (_ChangeScreenResolution())
		{
			_ShowMouse(false);
			wndStyle = WS_POPUP;
			wndExStyle |= WS_EX_TOPMOST;
		}
		else
		{
			MessageBox(NULL, "Cannot Run In Fullscreen.\nSwitching to Window Setting.", "ERROR: No Fullscreen", MB_OK | MB_ICONEXCLAMATION);
			_bfullscreen = false;
		}
	}
	else
        _ShowMouse(true);

//	RECT wndRect = {_GetPosX(), _GetPosY(), _GetPosX() + _GetWidth(), _GetPosY + _GetHeight()};
    RECT wndRect;

    wndRect.top = _GetPosY();
    wndRect.left= _GetPosX();
    wndRect.bottom  = _GetPosY() + _GetHeight();
    wndRect.right   = _GetPosX() + _GetWidth();

	if (!_bfullscreen)
	{
		AdjustWindowRectEx(&wndRect, wndStyle, 0, wndExStyle);

		if (wndRect.left < 0)
		{
			wndRect.right -= wndRect.left;
			wndRect.left = 0;
		}

		if (wndRect.top < 0)
		{
			wndRect.bottom -= wndRect.top;
			wndRect.top = 0;
		}
	}

	_hWnd = CreateWindowEx(	wndExStyle,
							_szwndClassName,
							_szwndCaption,
							wndStyle,
							wndRect.left,
							wndRect.top,
							wndRect.right - wndRect.left,
							wndRect.bottom - wndRect.top,
							NULL,
							0,
							_hInst,
							NULL	);

	if (_hWnd)
	{
		_hDC = GetDC(_hWnd);
//		ShowWindow(_hWnd, SW_NORMAL);
		return true;
	}
	else
	{
		_DestroyWindow();
		return false;
	}
}

bool
cApplication::_CreateWindow(const char *wndTitle, bool fullscreenflag, long width, long height, unsigned char bpp)
{
	_bfullscreen = fullscreenflag;
	strcpy(_szwndCaption, wndTitle);

	_SetWidth(width);
	_SetHeight(height);
	_SetBPP(bpp);

	return _CreateWindow();
}

bool
cApplication::_DestroyWindow(void)
{
	if (_hWnd)
	{
		if (_hDC)
		{
/*			wglMakeCurrent(_hDC, NULL);
			if (_hRC)
			{
				wglDeleteContext(_hRC);
				_hRC = 0;
			}
*/
			ReleaseDC(_hWnd, _hDC);
			_hDC = 0;
		}
		DestroyWindow(_hWnd);
		_hWnd = 0;
	}

//	if (_bfullscreen)
//	{
		ChangeDisplaySettings(NULL, 0);
//		_ShowMouse(true);
//	}

	return true;
}

bool
cApplication::_ChangeScreenResolution(void)
{
	DEVMODE	dm;
	ZeroMemory(&dm, sizeof(DEVMODE));
	dm.dmSize		= sizeof(DEVMODE);
	dm.dmPelsWidth	= _GetWidth();
	dm.dmPelsHeight	= _GetHeight();
	dm.dmBitsPerPel	= _bPP;
	dm.dmFields		= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

	if (ChangeDisplaySettings(&dm, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		return false;

	return true;
}

bool
cApplication::_ToggleFullscreen(void)
{
	PostMessage(_hWnd, WM_TOGGLEFULLSCREEN, 0, 0);
	return true;
}

bool
cApplication::_TerminateApplication(void)
{
	_bprogLooping = false;
	PostMessage(_hWnd, WM_QUIT, 0, 0);
	return true;
}

HWND		cApplication::_GethWnd(void)	{	return _hWnd;	}

HINSTANCE	cApplication::_GethInst(void)	{	return _hInst;	}

HDC			cApplication::_GethDC(void)		{	return _hDC;	}

//HGLRC		cApplication::_GethRC(void)		{	return _hRC;	}

bool
cApplication::_SethDC(HDC hDC)
{
	_hDC = hDC;
	return true;
}

/*
bool		cApplication::_SethRC(HGLRC hRC)
{
	_hRC = hRC;
	return true;
}
*/

long
cApplication::_GetPosX(void)
{
	if (_bfullscreen)
		return 0;
	else
		return _appWndPosX;
}

long
cApplication::_GetPosY(void)
{
	if (_bfullscreen)
		return 0;
	else
		return _appWndPosY;
}

long
cApplication::_GetWidth(void)
{
	if (_bfullscreen)
		return _appFullWidth;
	else
		return _appWndWidth;
}

long
cApplication::_GetHeight(void)
{
	if (_bfullscreen)
		return _appFullHeight;
	else
		return _appWndHeight;
}

bool
cApplication::_SetPosX(long xpos)
{
	if (!_bfullscreen)
		_appWndPosX = xpos;
	return true;
}

bool
cApplication::_SetPosY(long ypos)
{
	if (!_bfullscreen)
		_appWndPosY = ypos;
	return true;
}

bool
cApplication::_SetWidth(long width)
{
	if (_bfullscreen)
		_appFullWidth = width;
	else
		_appWndWidth = width;
	return true;
}

bool
cApplication::_SetHeight(long height)
{
	if (_bfullscreen)
		_appFullHeight = height;
	else
		_appWndHeight = height;
	return true;
}

unsigned char
cApplication::_GetBPP(void)	{	return _bPP;	}

bool
cApplication::_SetBPP(unsigned char bpp)
{
	_bPP = bpp;
	return true;
}

bool
cApplication::_IsFullscreen(void) { return _bfullscreen; }

bool
cApplication::_SwapBuffers(void)
{
    SwapBuffers(_hDC);

    return true;
}

//******	TODO:	Complete _Error()
bool
cApplication::_Error(bool bfatal, char *sztext, ...)
{
	return true;
}

bool
cApplication::_ShowMouse(bool bshow)
{
	ShowCursor(bshow);
	return true;
}

//*****	TODO:	Complete _Move()
bool
cApplication::_Move(long xpos, long ypos)
{
	return true;
}

//*****	TODO:	Complete _Resize()
bool
cApplication::_Resize(long width, long height)
{
	return true;
}

bool
cApplication::_Run(void)
{
/*   	int result = MessageBox(NULL, "Would you like to start in fullscreen?\n(Press F11 to toggle fullscreen mode.)", "Fullscreen?", MB_ICONQUESTION | MB_YESNOCANCEL);
    if (result == IDYES)
		_bfullscreen = true;
	else
		_bfullscreen = false;
*/
/*
	if (result == IDCANCEL)
        _bprogLooping = false;
    else
        _bprogLooping = true;
*/
   if ((MessageBox(NULL, "Would you like to start in fullscreen?\n(Press F11 to toggle fullscreen mode.)", "Fullscreen?", MB_ICONQUESTION | MB_YESNO)) == IDYES)
		_bfullscreen = true;

	if (!RegisterClassEx(&_wcex))
	{
		MessageBox(NULL, "", "", MB_OK | MB_ICONEXCLAMATION);   //****  TODO:   don't really expect class not to be registered.
		return false;
	}

	CoInitialize(NULL);

    _bprogLooping = true;

	while (_bprogLooping)
	{
		if (_CreateWindow())
		{
			if (_Init())
			{
			    ShowWindow(_hWnd, SW_NORMAL);
				MSG	msg;
				ZeroMemory(&msg,sizeof(MSG));
				bool bmsgPumpAct	=	true;

				while (bmsgPumpAct)
				{
					if (PeekMessage(&msg, _hWnd, 0, 0, PM_REMOVE))
					{
						if (msg.message != WM_QUIT)
						{
							TranslateMessage(&msg);
							DispatchMessage(&msg);
						}
						else
							bmsgPumpAct = false;
					}
					else
					{
						_Frame();
					}
				}
			}
			else
				_TerminateApplication();

			_Shutdown();
			_DestroyWindow();
		}
		else
		{
			MessageBox(NULL,"Error Creating Window!", "ERROR!", MB_OK | MB_ICONEXCLAMATION);
			_bprogLooping = false;
		}
	}

	CoUninitialize();

	UnregisterClass(_szwndClassName, _hInst);
	return true;
}

long FAR PASCAL
cApplication::_MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_TOGGLEFULLSCREEN:
			_bfullscreen = !_bfullscreen;
			PostMessage(_hWnd, WM_QUIT, 0, 0);
		break;

		case WM_CLOSE:
			_TerminateApplication();
			return 0;
		break;

		case WM_MOVE:
				_SetPosX(LOWORD(lParam));
				_SetPosY(HIWORD(lParam));
			return 0;
		break;

		case WM_SIZE:
			_SetWidth(LOWORD(lParam));
			_SetHeight(HIWORD(lParam));
			return 0;
		break;

		case WM_KEYUP:
		{
			switch(wParam)
			{
				case VK_ESCAPE:
					_TerminateApplication();
					return 0;
				break;

				case VK_F11:
					_ToggleFullscreen();
					return 0;
				break;
			}
			return 0;
		}
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

static
long FAR PASCAL
AppWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return g_pApplication->_MsgProc(hWnd, uMsg, wParam, lParam);
}
