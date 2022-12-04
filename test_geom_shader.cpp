#include "core_global.h"
#include "test_geom_shader.h"

#define SCREEN_W		250
#define SCREEN_H		250
#define IMAGE_W			2048
#define IMAGE_H			2048
#define LINE_HEIGHT_PX	250
#define	LINE_HEIGHT		0.03
#define SPACE_X_ADVANCE	67

cApp::cApp()
{
    _appWndWidth = 256;
    _appWndHeight = 256;

//   _appFullWidth = 640;
//   _appFullHeight = 480;

    _numObjs = 2;
    _squareObjects = NULL;

    _degFOV = 45.0f;
    _perspectiveNear = 0.1f;
    _perspectiveFar = 1000.0f;
    _orthogonalNear = -1.f;
    _orthogonalFar = 1.f;

    _degTheta = 0.0f;

	_lastTime = _Timer._GetTime();
	_uniLocMatrix = 0;
}

bool
cApp::_Resize(long width, long height)
{
	if (height==0) height = 1;

	_SetWidth(width);
	_SetHeight(height);

	glViewport(0, 0, (int)width, (int)height);

	_Graphics._SetWidth(width);
	_Graphics._SetHeight(height);
	_Graphics._SetPerspective(_degFOV, (float)width/ (float)height, _perspectiveNear, _perspectiveFar);
	_Graphics._SetOrthogonal(0, (float)width, -(float)height, 0, _orthogonalNear, _orthogonalFar);

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

    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

//  Enable Blending (implemented 11/28/2018 at 5:26pm)
/**/
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    _Timer._Reset();
    _lastTime = _Timer._GetTime();




    double fontSize = 1000.0;

    _fontData._SetAspectRatio(_GetWidth(), _GetHeight());
	_fontData._SetLineSizes(LINE_HEIGHT, LINE_HEIGHT_PX);
	_fontData._SetSpaceWidth(SPACE_X_ADVANCE);
//M=77
//char id=77   x=1592  y=572   width=164   height=176   xoffset=24    yoffset=29    xadvance=186
//A=65
//char id=65   x=734   y=1438  width=145   height=153   xoffset=6     yoffset=42    xadvance=151
//R=82
//char id=82   x=1414  y=928   width=148   height=163   xoffset=16    yoffset=43    xadvance=159
//.=46
//char id=46   x=2016  y=1892  width=27    height=27    xoffset=30    yoffset=165   xadvance=65
	_fontData._SetCharacterData(IMAGE_W, 77, 1592, 572, 164, 176, 24, 29, 186);
	_fontData._SetCharacterData(IMAGE_W, 65, 734, 1438, 145, 153, 6, 42, 151);
	_fontData._SetCharacterData(IMAGE_W, 82, 1414, 928, 148, 163, 16, 43, 159);
	_fontData._SetCharacterData(IMAGE_W, 46, 2016, 1892, 27, 27, 30, 165, 65);
	_fontData._PrintCharacterData(77);
	_fontData._PrintCharacterData(65);
	_fontData._PrintCharacterData(82);
	_fontData._PrintCharacterData(46);


    cCharacter character = _fontData._GetCharacter((int)'M');

    double x = (character._GetxOff() * fontSize);
    double y = (character._GetyOff() * fontSize);
    double xMax = x + (character._GetSizeX() * fontSize);
    double yMax = y + (character._GetSizeY() * fontSize);
		/****TODO: May have to convert to the proper Ortho Coords here***/
		/*
		double properX = (2 * x) - 1;
		double properY = (2 * y) + 1;  //may have to multiply this by -1
		double properXMax = (2 * xMax) - 1;
		double properYMax = (2 * yMax) + 1;
		*/
		//2. create a vertex with that data
/*    float vertex[] =
    {
        float(x)		, float(y)		  , //v0   upper left
        float(x)		, float(y + yMax) , //v1   lower left
        float(x + xMax), float(y + yMax) , //v2   lower right
        float(x + xMax), float(y)		   //v3   upper right
    };
*/
		//3. get texCoord data
    double u = character._GetUTexCoord();
    double v = character._GetVTexCoord();
    double uMax = u + character._GetUMaxTexCoord();
    double vMax = v + character._GetVMaxTexCoord();
		//4. create texCoord vertex with that data
    float texCoord[] =
    {
        float(u)	, float(v)  ,    //v0   upper left
        float(u)	, float(vMax),    //v1   lower left
        float(uMax), float(vMax),    //v2   lower right
        float(uMax), float(v)		       //v3   upper right
    };
    std::cout<<std::to_string(float(u))+","+std::to_string(float(v))+","+std::to_string(float(uMax))+","+std::to_string(float(vMax))<<std::endl;

	float vertex[] =
	{
		-.5f, .5f, //.0f,		// 1
		-.5f,-.5f, //.0f,		// 2
		 .5f,-.5f,  //, .0f		// 3
		 .5f, .5f //.0f,		// 0
	};
























	/////////	game objects
	unsigned numTex = 1;
	unsigned numVerts = 4;
	unsigned numTris = 2;
/*
    float texCoord[] =
    {
        1.f, 1.f,   //upper right
        0.f, 1.f,   //upper left
        0.f, 0.f,   //lower left (origin)
        1.f, 0.f    //lower right
    };
*/
/*
    float texCoord[] =
    {
        0.f, 1.f,   //upper left
        0.f, 0.f,   //lower left (origin)
        1.f, 0.f,    //lower right
        1.f, 1.f   //upper right
    };

    float texCoord[] =
    {
        0.42919921875f, 0.2978515625f,   //upper right
        0.3583984375f, 0.2978515625f,   //upper left
        0.3583984375f, 0.2231445313f,   //lower left (origin)
        0.42919921875f, 0.2231445313f    //lower right
    };

	float vertex[] =
	{
		 .5f, .5f, //.0f,		// 0
		-.5f, .5f, //.0f,		// 1
		-.5f,-.5f, //.0f,		// 2
		 .5f,-.5f  //, .0f		// 3
	};
*/
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



/*
const float shaderimageSize = 2048.f;
const int shaderw = 145, shaderh = -153;
const glm::ivec2 shadertexPosition = glm::ivec2(734,1438);
glm::vec2 shadertexCoord[4];
*/
    //if (!_texture._Load("data/bloodsinb.tga"))  //currently does not work.  don't use.
    //if (!_texture._Load("data/bloodsin.gif"))    //currently does not work.  don't use.
    //if (!_texture._Load("data/font005-1024.tga"))
    //if (!_texture._Load("data/lucidaConsole_comicSansMS_2048_00.tga"))
    if (!_texture._Load("data/Lucinda_Handwriting_2048x2048_01_0.tga"))
    //if (!_texture._Load("D:\\Users\\Rani\\Projects\\C++Projects\\tests\\OpenGL.x64.CodeBase\\data\\kueken256-dxt5.dds"))
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

	//_squareObject._GetShader()->_CreateShaderObject("minimal_04.vert.glsl", VERT_SHADER);
	//_squareObject._GetShader()->_CreateShaderObject("minimal_04.geom.glsl", GEOM_SHADER);
	//_squareObject._GetShader()->_CreateShaderObject("minimal_04.frag.glsl", FRAG_SHADER);
	_squareObject._GetShader()->_CreateShaderObject("variblefont_0.vert.glsl", VERT_SHADER);
	_squareObject._GetShader()->_CreateShaderObject("variblefont_0.geom.glsl", GEOM_SHADER);
	_squareObject._GetShader()->_CreateShaderObject("variblefont_0.frag.glsl", FRAG_SHADER);

/*
	_squareObject._GetShader()->_CreateShaderObject("minimal_03.vert.glsl", VERT_SHADER);
	//_squareObject._GetShader()->_CreateShaderObject("minimal_04.geom.glsl", GEOM_SHADER);
	_squareObject._GetShader()->_CreateShaderObject("minimal_03.frag.glsl", FRAG_SHADER);
*/
	_squareObject._GetShader()->_CreateProgram();

	_squareObject._SetUniformMVPLocation();
	_squareObject._SetUniformTextureSamplers();

//	_squareObject._GetShader()->_Bind(true);

        _uniLocMatrix = _squareObject._GetShader()->_CreateUniformLocationID("uniMatrix");

		//_squareObject._GetShader()->_LoadUniformMat4v(_uniIDOrthoProj, glm::val_ptr(_orthoMat));
		_uniIDOrthoProj = _squareObject._GetShader()->_CreateUniformLocationID("orthoProjection");
//	_squareObject._GetShader()->_Bind(false);


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

	_degTheta += .001f * elapsedTime;
	if (_degTheta >= 360.f) _degTheta = 0.f;

/*
    _testMat4 = glm::mat4(1.f);
    _testMat4 = glm::rotate(_testMat4, _degTheta, glm::vec3(0.f, 0.f, 1.0f));
    _testMat4 = glm::rotate(_testMat4, _degTheta*-2, glm::vec3(1.f, 0.f, 0.0f));

    _testvec4SIMD *= glm::simdMat4(_testMat4);
*/
/**/
    _squareObject._Reset();
    _squareObject._Move(glm::vec3(50.f, -50.f, 0.f));
	//_squareObject._Rotate(glm::vec3(0.0f, 0.0f, _degTheta*10.f));
    _squareObject._Scale(glm::vec3(18.125f, 18.125f, 0.f));


    //_orthoMat = glm::ortho(0.f, 1.f, 0.f, 1.f, _orthogonalNear, _orthogonalFar);// * _squareObject._GetMatrix();
    //_orthoMat[1][1] *= -1.f;
    //_orthoMat *= _squareObject._GetMatrix();;
    _orthoMat = _Graphics._GetOrthogonalMatrix() * _squareObject._GetMatrix();

	_squareObject._GetShader()->_Bind(true);
        _squareObject._GetShader()->_LoadUniformMat4v(_uniIDOrthoProj, glm::value_ptr(_orthoMat));
	_squareObject._GetShader()->_Bind(false);





	//_squareObject._Update();

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
