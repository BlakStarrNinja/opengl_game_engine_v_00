#version 330 core

#define	VERT_POS		0
#define	VERT_COL		1
#define VERT_NORM		2
#define	VERT_TAN		3
#define VERT_TEXCOORD0	4
#define VERT_TEXCOORD1	5
#define VERT_TEXCOORD2	6
#define VERT_TEXCOORD3	7
#define VERT_TEXCOORD4	8

precision highp float;

//uniform mat4 unused44 = mat4(1.0f);

in vec3 ex_Color;

out vec4 out_Color;

void main(void)
{
//    out_Color = unused44 * vec4(ex_Color,1.0f);
    out_Color = vec4(ex_Color, 1.0f);
}
