#include "core_global.h"
#include "example_model.h"

cApp::cApp()
{
    _appWndWidth = 256;
    _appWndHeight = 256;

//   _appFullWidth = 640;
//   _appFullHeight = 480;

    _numObjs = 2;
    _squareObjects = NULL;

    _degTheta = 0.0f;
    _degFOV = 45.0f;
    _near = 0.1f;
    _far = 1000.0f;

//    _testMat4       = glm::mat4(1.f);
//    _testvec4SIMD   = glm::simdMat4(1.f);

	_lastTime = _Timer._GetTime();
}

bool
cApp::_Resize(long width, long height)
{
	if (height==0) height = 1;

	_SetWidth(width);
	_SetHeight(height);

	glViewport(0, 0, (int)width, (int)height);

	_Graphics._SetPerspective(_degFOV, (float)width/ (float)height, _near, _far);
//	_Graphics._SetPerspectiveSIMD(_degFOV, (float)width/ (float)height, _near, _far);

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

bool
cApp::_Init(void)
{
	/////////	app/system objects
	_Graphics._Init();
	_Graphics._SetMode(_GethDC(), _IsFullscreen(), true, _GetWidth(), _GetHeight(), _GetBPP());
    _Graphics._InitialStates();

//    glClearColor(1.f, 1.f, 1.f, 1.f);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

//  Enable Blending (implemented 11/28/2018 at 5:26pm)
/**/
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    _Timer._Reset();
    _lastTime = _Timer._GetTime();

	/////////	game objects
	unsigned numTex = 1;
	unsigned numVerts = 4;
	unsigned numTris = 2;

    float texCoord[] =
    {
        1.f, 1.f,   //upper right
        0.f, 1.f,   //upper left
        0.f, 0.f,   //lower left (origin)
        1.f, 0.f    //lower right
    };
/*
    float texCoord[] =
    {
        0.f, 1.f,   //upper left
        0.f, 0.f,   //lower left (origin)
        1.f, 0.f,    //lower right
        1.f, 1.f   //upper right
    };
*/
	float vertex[] =
	{
		 .5f, .5f, //.0f,		// 0
		-.5f, .5f, //.0f,		// 1
		-.5f,-.5f, //.0f,		// 2
		 .5f,-.5f  //, .0f		// 3
	};
/*
    float texCoord[] =
    {
        0.f, 0.f,   //lower left (origin)
        1.f, 0.f,    //lower right
        1.f, 1.f,   //upper right
        0.f, 1.f   //upper left
    };

	float vertex[] =
	{
		-.5f,-.5f, //.0f,		// 2
		 .5f,-.5f,  //, .0f		// 3
		 .5f, .5f, //.0f,		// 0
		-.5f, .5f //.0f,		// 1
	};
*/
	float color[] =
	{
		1.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 0.f, 1.f,
		.3f, .9f,.66f
	};

	unsigned int triangle[] =
	{
		0, 1, 2,
		0, 2, 3
	};


    //if (!_texture._Load("data/bloodsinb.tga"))  //currently does not work.  don't use.
    //if (!_texture._Load("data/bloodsin.gif"))    //currently does not work.  don't use.
    //if (!_texture._Load("data/font005-1024.tga"))
    if (!_texture._Load("D:\\Users\\Rani\\Projects\\C++Projects\\tests\\OpenGL.x64.CodeBase\\data\\kueken256-dxt5.dds"))
    //if (!_texture._Load("data/GlassHiFa.bmp"))
    //if (!_texture._Load("data/helloa.jpg"))
    //if (!_texture._Load("data/helloa.bmp"))
    {
//        _MB("",""); //TODO: ERROR MESSAGE!!
        return false;
    }

    _squareMesh._SetNumTextures(numTex);
    _squareMesh._SetTexture(&_texture);
    _squareMesh._CreateBuffer(BUFF_VERT_TEXCOORD0, TWO_D, GL_ARRAY_BUFFER, GL_STATIC_DRAW,
			TWO_D * numVerts * sizeof(float), texCoord);
	_squareMesh._SetNumVertices(numVerts);
	_squareMesh._CreateBuffer(BUFF_VERT_POS, TWO_D, GL_ARRAY_BUFFER, GL_STATIC_DRAW,
			TWO_D * numVerts * sizeof(float), vertex);
	_squareMesh._CreateBuffer(BUFF_VERT_COL, THREE_D, GL_ARRAY_BUFFER, GL_STATIC_DRAW,
			THREE_D * numVerts * sizeof(float), color);
	_squareMesh._SetNumTriangles(numTris);
	_squareMesh._CreateBuffer(BUFF_VERT_INDEX, THREE_D, GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW,
			THREE_D * numTris * sizeof(unsigned), triangle);

	_squareMesh._CreateVertexAttribs();


	_squareObject._Create(&_Graphics, &_squareMesh);
	_squareObject._GetShader()->_CreateShaderObject("minimal_03.vert.glsl", VERT_SHADER);
	_squareObject._GetShader()->_CreateShaderObject("minimal_03.frag.glsl", FRAG_SHADER);
	_squareObject._GetShader()->_CreateProgram();

	_squareObject._SetUniformMVPLocation();
	_squareObject._SetUniformTextureSamplers();


    _squareObjects = new cObject[_numObjs];

	for (int i=0; i<_numObjs; ++i)
	{
		_squareObjects[i]._Create(&_Graphics, &_squareMesh);
		_squareObjects[i]._GetShader()->_CreateShaderObject("minimal_02.vert.glsl", VERT_SHADER);
		_squareObjects[i]._GetShader()->_CreateShaderObject("minimal_02.frag.glsl", FRAG_SHADER);
		_squareObjects[i]._GetShader()->_CreateProgram();

		_squareObjects[i]._SetUniformMVPLocation();
		_squareObjects[i]._Update();
//		_squareObjects[i]._UpdateSIMD();
	}
/**/
/*
    int i = 0;

//    glGetIntegerv(GL_TEXTURE1, &i);
    cout<<"GL_TEXTURE0 = "<<GL_TEXTURE0<<endl;
    cout<<"GL_TEXTURE1 = "<<GL_TEXTURE1<<endl;
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &i);
 //   cout<<"GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS = "<<GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS<<endl;
    cout<<"GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS = "<<i<<endl;
    glGetIntegerv(GL_ACTIVE_TEXTURE, &i);
    cout<<"GL_ACTIVE_TEXTURE = "<<i<<endl;
    glActiveTexture(GL_TEXTURE1);
    glGetIntegerv(GL_ACTIVE_TEXTURE, &i);
    cout<<"GL_ACTIVE_TEXTURE = "<<i<<endl;
    glActiveTexture(0);
    glGetIntegerv(GL_ACTIVE_TEXTURE, &i);
    cout<<"GL_ACTIVE_TEXTURE = "<<i<<endl;
*/


	return true;
}

bool
cApp::_Shutdown(void)
{
    if (_squareObjects)
        for (int i=0; i<_numObjs; ++i)
            _squareObjects[i]._Free();

    delete[] _squareObjects;

	_squareObject._Free();

	_squareMesh._Free();

	_texture._Free();

	_Graphics._Shutdown();

	return true;
}

bool
cApp::_Frame(void)
{
	_Update(_Timer._GetTime() - _lastTime);
	_Render();
	_SwapBuffers();

	return true;
}

bool
cApp::_Update(double elapsedTime)
{
 /**/
	_lastTime = _Timer._GetTime();

	_degTheta += .00001f * elapsedTime;
	if (_degTheta >= 360.f) _degTheta = 0.f;

/*
    _testMat4 = glm::mat4(1.f);
    _testMat4 = glm::rotate(_testMat4, _degTheta, glm::vec3(0.f, 0.f, 1.0f));
    _testMat4 = glm::rotate(_testMat4, _degTheta*-2, glm::vec3(1.f, 0.f, 0.0f));

    _testvec4SIMD *= glm::simdMat4(_testMat4);
*/
/**/
    _squareObject._Reset();
    _squareObject._Move(glm::vec3(0, 0, -2.f));
	_squareObject._Rotate(glm::vec3(0.0f, _degTheta, 0.0f));
	_squareObject._Update();

/**/

	for (int i=0; i<_numObjs; ++i)
	{
		float negpos =0;
		if ( i % 2) negpos = 1.f;
		else negpos = -1.f;

		_squareObjects[i]._Reset();
		_squareObjects[i]._Move(glm::vec3(negpos * .75f, 0.f, -4.f));
		_squareObjects[i]._Rotate(glm::vec3(0.0f, 0.0f, _degTheta));
		_squareObjects[i]._Update();
	}


/*
int i =0;

	for (int i=0; i<_numObjs; ++i)
	{
		float negpos =0;
		if ( i % 2) negpos = 1.f;
		else negpos = -1.f;

		_squareObjects[i]._Reset();
		_squareObjects[i]._MoveSIMD(glm::vec3(0.f, 0.f, -3.f));
		_squareObjects[i]._RotateSIMD(glm::vec3(0.0f, -_degTheta, 0.0f));
//      _squareObjects[i]._RotateZSIMD(_degTheta);
//		_squareObjects[i]._UpdateSIMD();
	}
*/

	return true;
}

bool
cApp::_Render(void)
{
/**/
    _Graphics._Clear();

    for (int i=0; i<_numObjs; ++i)
        _squareObjects[i]._Render();

    _squareObject._Render();

	return true;
}

int PASCAL WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR szCmdLine, int nCmdShow)
{
	cApp App;
	return App._Run();
}
