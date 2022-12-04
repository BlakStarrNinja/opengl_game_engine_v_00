#include "core_global.h"

class cApp : public cApplication
{
        cTimer _Timer;
        double _elapsedTime;

        cTexture _texture;

        float _degTheta;
        float _degFOV;
        float _near;
        float _far;

        glm::mat4 _projection;
        glm::mat4 _modelview;

        GLint   _uniformMVP, _uniUnsed44;
        GLuint  _programID;

        cGraphics   _Graphics;
        cObj        _ObjTri;

        bool _Render(void);

    public:
        cApp();

        bool _Resize(long width, long height);

        long FAR PASCAL _MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        bool _Init(void);
        bool _Shutdown(void);
        bool _Frame(void);
};

cApp::cApp()
{
    _appWndWidth = 256;
    _appWndHeight = 256;

//   _appFullWidth = 640;
//   _appFullHeight = 480;

    _degFOV = 45.0f;
    _degTheta = 0.0f;
    _near = 0.1f;
    _far = 1000.0f;

    _projection = _modelview = glm::mat4(1.0f);

    _uniformMVP = _programID = 0;
}

bool cApp::_Init(void)
{
    _Graphics._Init();
    _Graphics._SetMode(_GethDC(), _IsFullscreen(), true, _GetWidth(), _GetHeight(), _GetBPP());
    _Graphics._InitialStates();

    _ObjTri._Init();
    _ObjTri._GetShader()->_CreateShaderObject("minimal_02.frag.glsl", FRAG_SHADER);
    _ObjTri._GetShader()->_CreateShaderObject("minimal_02.vert.glsl", VERT_SHADER);
    _ObjTri._GetShader()->_CreateProgram();

    _ObjTri._GetShader()->_QueryProgramObj();

    _ObjTri._GetShader()->_Bind(true);
        _programID = _ObjTri._GetShader()->_GetProgramObject();
        _uniformMVP = glGetUniformLocation(_programID, "MVP");
        _uniUnsed44 = glGetUniformLocation(_programID, "unused44");
    _ObjTri._GetShader()->_Bind(false);

    _texture._Load("data/kueken256-dxt1.dds");

    return true;
}

bool cApp::_Shutdown(void)
{
    _ObjTri._Shutdown();

    _Graphics._Shutdown();

    _texture._Free();

    return true;
}

bool cApp::_Render(void)
{
	glm::mat4 modelview2;
	glm::mat4 MVP;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_modelview = glm::mat4(1.0f);	//loadidentity();

	modelview2 =  _modelview;		//pushmatrix();
///*
		modelview2 = glm::translate(modelview2, glm::vec3(-0.75f, 0.0f, -4.0f));			//translate(-5.0f, 0.0f, -10.0f);
		modelview2 = glm::rotate(modelview2, _degTheta, glm::vec3(0.0f, 0.0f, 1.0f));	//rotatef(theta, 0.0f, 0.0f, 1.0f);

		MVP = _projection * modelview2;
		_ObjTri._GetShader()->_Bind(true);
			glUniformMatrix4fv(_uniformMVP, 1, false, glm::value_ptr(MVP));

			glm::mat4 unused44 = glm::mat4(5.0f);
			glUniformMatrix4fv(_uniUnsed44, 1, false, glm::value_ptr(unused44));
		_ObjTri._GetShader()->_Bind(false);

		_ObjTri._Render();
	modelview2 = _modelview;		//popmatrix();
//*/
///*
//	modelview2 = _modelview;		//pushmatrix();
		modelview2 = glm::translate(modelview2, glm::vec3(0.75f, 0.0f, -4.0f));
		modelview2 = glm::rotate(modelview2, -_degTheta, glm::vec3(1.0f, 0.0f, 0.0f));

//        MVP = modelview2;
		MVP = _projection * modelview2;
		_ObjTri._GetShader()->_Bind(true);
			glUniformMatrix4fv(_uniformMVP, 1, false, glm::value_ptr(MVP));
		_ObjTri._GetShader()->_Bind(false);

		_ObjTri._Render();
//	modelview2 = modelview;


	if ((_degTheta += 0.03f) > 360.0f)
		_degTheta = 0.0f;
//*/
	return true;

/*
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _ObjTri._Render();

    return true;
*/
}

bool cApp::_Frame(void)
{
    _Render();

    _SwapBuffers();

    return true;
}

bool
cApp::_Resize(long width, long height)
{
    if (height == 0) height = 1;
    glViewport(0, 0, (int) width, (int)height);
    _projection = glm::perspective((_degFOV), (float)width/ (float)height, _near, _far);

    return true;
}

long FAR PASCAL
cApp::_MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{

		case WM_TOGGLEFULLSCREEN:
			_bfullscreen = !_bfullscreen;
			PostMessage(_GethWnd(), WM_QUIT, 0, 0);
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
			_Resize(LOWORD(lParam), HIWORD(lParam));
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

int PASCAL WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR szCmdLine, int nCmdShow)
{
	cApp App;
	return App._Run();
}
