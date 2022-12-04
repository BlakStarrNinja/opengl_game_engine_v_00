#include "temp_cObject.h"

cObject::cObject()
{
	_vboID[0] = _vboID[1] = 0L;

	memset(_vertArray, 0, (sizeof(float) * MAX_VERTS * THREE_D));
	memset(_colArray, 0, (sizeof(float) * MAX_VERTS * THREE_D));
}

cShader * cObject::_GetShader(void) { return &_shader; }

void cObject::_Init(void) { }
void cObject::_Shutdown(void) { }
void cObject::_Render(void) { }

void
cObj::_Init(void)
{
    float	vertArray[MAX_VERTS][THREE_D] =
    {
        { 0.0f, 0.5f, 0.0f},
        {-0.5f,-0.5f, 0.0f},
        { 0.5f,-0.5f, 0.0f}
    },

            colArray[MAX_VERTS][THREE_D] =
    {
        { 1.0f, 0.0f, 0.0f},
        { 0.0f, 1.0f, 0.0f},
        { 0.0f, 0.0f, 1.0f}
    };

	memcpy(_vertArray, vertArray, (sizeof(float) * MAX_VERTS * THREE_D));
	memcpy(_colArray, colArray, (sizeof(float) * MAX_VERTS * THREE_D));

    glGenBuffers(2, (GLuint *)_vboID);

    glBindBuffer(GL_ARRAY_BUFFER, _vboID[0]);
    glBufferData(GL_ARRAY_BUFFER, MAX_VERTS * THREE_D * sizeof(GLfloat), vertArray, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, _vboID[1]);
    glBufferData(GL_ARRAY_BUFFER, MAX_VERTS * THREE_D * sizeof(GLfloat), colArray, GL_STATIC_DRAW);
}

void
cObj::_Render(void)
{
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    _shader._Bind(true);

    glBindBuffer(GL_ARRAY_BUFFER, _vboID[0]);
    glVertexAttribPointer(VERT_POS, THREE_D, GL_FLOAT, GL_FALSE, 0, NULL);

    glBindBuffer(GL_ARRAY_BUFFER, _vboID[1]);
    glVertexAttribPointer(VERT_COL, THREE_D, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(VERT_POS);
    glEnableVertexAttribArray(VERT_COL);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(VERT_COL);
    glDisableVertexAttribArray(VERT_POS);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    _shader._Bind(false);
}

void cObj::_Shutdown(void) { glDeleteBuffers(2, (GLuint *)_vboID); }
