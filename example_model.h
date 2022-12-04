#ifndef	_EXAMPLE_MODEL_H_
#define _EXAMPLE_MODEL_H_

//		glm::simdVec4 _testvec4;
/*
glm::simdMat4 g_testMat4SIMD = glm::simdMat4(1.f);
glm::simdVec4 g_testVec4SIMD = glm::simdVec4(0.f);
glm::mat4 g_testMat4         = glm::mat4_cast(g_testMat4SIMD);
glm::vec4 g_testVec4         = glm::vec4_cast(g_testVec4SIMD);
*/

//glm::simdVec4 g_sv4 = glm::simdVec4(0.f);

/*
class cOpenGLGraphics : public class cGraphics
{
	protected:
		glm::mat4	_projection;
		glm::mat4	_veiw;

		float _clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.5f };

	public:
		cOpenGLGraphics();
//		~cOpenGLGraphics();

		inline glm::mat4	_GetProjectionMatrix(void) const { return _projection; }
		inline glm::mat4	_GetViewMatrix(void) const { return _view; }
		inline glm::mat4	_GetViewProjectionMatrix(void) const { return (_projection * _view); }

		bool	_SetPerspective( float degFOV, float aspect, float near, float far);
		bool	_Clear(void);
		bool	_ClearDisplay(void);
		bool	_ClearDepthBuffer(void);
		bool	_SetClearColor(float color[4]);
		bool	_SetClearColor(float red, float blue, float green, float alpha);
};
*/

class cApp : public cApplication
{
	private:	//system objects
		cGraphics	_Graphics;

		cTimer	_Timer;

	protected:
        float _degTheta;
        float _degFOV;
        float _near;
        float _far;

		double _lastTime;

        cTexture _texture;
		cMesh _squareMesh;
		unsigned long _numObjs;
		cObject *_squareObjects;
//		cObject _square;
		cObject _squareObject;

//		glm::mat4       _testMat4;
//		glm::simdMat4   _testvec4SIMD;


	protected:
		virtual bool _Update(double elapsedTime);
		virtual bool _Render(void);

	public:
		cApp();

        bool _Resize(long width, long height);

        long FAR PASCAL _MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        bool _Init(void);
        bool _Shutdown(void);
        bool _Frame(void);
};

#endif	//_EXAMPLE_MODEL_H_
