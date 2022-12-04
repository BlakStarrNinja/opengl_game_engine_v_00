#ifndef _CORE_SYSTEM_H_
#define _CORE_SYSTEM_H_

//#include <windows.h>

class cApplication
{
	private:
		HINSTANCE	_hInst;
		HWND	    _hWnd;
		HDC		    _hDC;
//		HGLRC	    _hRC;

	protected:
		char	_szwndClassName[MAX_PATH];
		char	_szwndCaption[MAX_PATH];

		WNDCLASSEX	_wcex;

		long	_appWndPosX,
				_appWndPosY,
				_appWndWidth,
				_appWndHeight,
				_appFullWidth,
				_appFullHeight;

		unsigned char	_bPP;

		bool	_bfullscreen,
				_bprogLooping;

   		static const UINT WM_TOGGLEFULLSCREEN = (WM_USER + 1);

	protected:
		bool	_CreateWindow(void);
		bool	_CreateWindow(const char *wndTitle, bool fullscreenflag, long width, long height, unsigned char bpp);
		bool	_DestroyWindow(void);
		bool	_ChangeScreenResolution(void);
		bool	_ToggleFullscreen(void);
		bool	_TerminateApplication(void);
		bool    _SwapBuffers(void);

//		virtual bool _Update(void);
//		virtual bool _Update(double timeElapsed);


	public:
		cApplication();

		HINSTANCE	_GethInst(void);
		HWND		_GethWnd(void);
		HDC			_GethDC(void);
//		HGLRC		_GethRC(void);
		bool		_SethDC(HDC hDC);
//		bool		_SethRC(HGLRC hRC);

		long		_GetPosX(void);
		long		_GetPosY(void);
		long		_GetWidth(void);
		long		_GetHeight(void);
//		long		_GetWndWidth(void);
//		long		_GetWndHeight(void);
//		long		_GetFullWidth(void);
//		long		_GetFullHeight(void);

		bool		_SetPosX(long x);
		bool		_SetPosY(long y);
		bool		_SetWidth(long width);
		bool		_SetHeight(long height);
//		bool		_SetAppWidth(long width);
//		bool		_SetAppHeight(long height);
//		bool		_SetFullWidth(long width);
//		bool		_SetFullHeight(long height);

		unsigned char _GetBPP(void);
		bool		_SetBPP(unsigned char bpp);
        bool       _IsFullscreen(void);

		bool		_ShowMouse(bool bshow = true);
		bool		_Error(bool bfatal, char *sztext, ...); //most likely will make this a virtual function that will operate with strings

		virtual bool _Move(long xpos, long ypos);
		virtual bool _Resize(long width, long height);
		virtual bool _Run(void);

		virtual long FAR PASCAL _MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual bool _Init(void)		{	return true;	}
		virtual bool _Shutdown(void)	{	return true;	}
		virtual bool _Frame(void)		{	return true;	}
};

static cApplication * g_pApplication = NULL;
static long FAR PASCAL AppWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

enum Purposes
{
	NOPURPOSE = 0,
	INITPURPOSE,
	SHUTDOWNPURPOSE,
	FRAMEPURPOSE
};

class cStateManager
{
	typedef struct sState
	{
		void (*_pFunction)(void *ptr, long purpose);
		sState *_pnext;

		sState()
		{
			_pFunction = NULL;
			_pnext	= NULL;
		}

		~sState()
		{
			delete _pnext;
			_pnext = NULL;
		}
	} sState;

	protected:
		sState *_pstateParent;

	public:
		cStateManager();
		~cStateManager();

		void	_Push(void (*pFunction)(void *ptr, long purpose), void *pdata = NULL);
		bool	_Pop(void *pdata = NULL);
		void	_PopAll(void *pdata = NULL);
		bool	_Process(void *pdata = NULL);
};

class cProcessManager
{
	typedef struct sProcess
	{
		void (*_pFunction)(void *ptr, long purpose);
		sProcess *_pnext;

		sProcess()
		{
			_pFunction = NULL;
			_pnext = NULL;
		}

		~sProcess()
		{
			delete _pnext;
			_pnext = NULL;
		}
	}	sProcess;

	protected:
		sProcess *_pprocessParent;

	public:
		cProcessManager();
		~cProcessManager();

		void _Push(void (*pProcess)(void *ptr, long purpose), void *pdata = NULL);
		bool _Pop(void *pdata = NULL);
		void _PopAll(void *pdata = NULL);
		void _Process(void *pdata = NULL);
};

class cDataPackage
{
	protected:
		void	*_pbuf;
		unsigned long _ulsize;

	public:
		cDataPackage();
		~cDataPackage();

		void *_Create(unsigned long size);
		void _Free(void);

		bool _Save(char *pfile);
		void *_Load(char *pfile, unsigned long size);

		void		*_GetPtr(void);
		unsigned long _GetSize(void);
};

#endif //_CORE_SYSTEM_H_
