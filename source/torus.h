#pragma once

typedef struct
{
	float x, y, z;
} vertex;

typedef struct
{
	vertex vert;
	vertex norm;
} vertexObj;

extern const vertexObj vertex_list[432];
#define vertex_list_count (sizeof(vertex_list)/sizeof(vertex_list[0]))
