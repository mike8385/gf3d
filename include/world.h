#ifndef __WORLD_H_
#define __WORLD_H_

#include "gf3d_mesh.h"

typedef struct
{
	Mesh*					terrain;
	Texture*				texture;
	GFC_List*				entities; //Just the ones in the world
	GFC_Color				lightColor;
	GFC_Vector3D			lightPos;

}World;



World* world_new();

World* world_load(const char* filename);

void world_free(World* world);

void world_draw(World* world);

#endif
#pragma once
