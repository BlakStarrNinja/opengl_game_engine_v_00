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

uniform mat4 MVP = mat4(1.0f);
uniform mat4 orthoProjection = mat4(1.0f);

layout(location = VERT_COL) in vec3 in_color;
layout(location = VERT_POS) in vec3 in_position;
layout(location = VERT_TEXCOORD0) in vec2 in_tc0;

out vec3 geom_Color;
out vec2 geom_tc0;

void main(void)
{
    //gl_Position = MVP * vec4(in_position, 1.0f);
    gl_Position = orthoProjection * vec4(in_position, 1.0f);
    geom_Color = in_color;
    geom_tc0 = in_tc0;
}
