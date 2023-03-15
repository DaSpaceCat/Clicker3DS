#pragma once

typedef struct
{
	float x, y, z;
	float nx, ny, nz;
} vertex;

extern const vertex vertex_list[162];
#define vertex_list_count (sizeof(vertex_list)/sizeof(vertex_list[0]))
