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


uniform mat4 MVP = mat4(1.0f);

layout(location = 1) in vec3 in_color;
layout(location = 0) in vec3 in_position;
layout(location = 4) in vec2 in_tc0;

out vec3 ex_Color;

void main(void)
{
    gl_Position = MVP * vec4(in_position, 1.0f);
    ex_Color = in_color;
}
