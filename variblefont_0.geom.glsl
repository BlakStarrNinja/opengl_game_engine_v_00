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

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 geom_Color[];
in vec2 geom_tc0[];

out vec3 ex_Color;
out vec2 ex_tc0;

void main(void)
{
    for (int i = 0; i < gl_in.length(); ++i)
    {
        ex_Color = geom_Color[i];
        ex_tc0 = geom_tc0[i];

        gl_Position =  gl_in[i].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
}
