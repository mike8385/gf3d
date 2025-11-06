#ifndef __SPACE_H__
#define __SPACE_H__

#include "gfc_list.h"
#include "body.h"
#include "entity.h"

typedef struct {

	GFC_List*	StaticMeshes;  //Terrain(s)
	GFC_List*	bodies;		//for entites and the like
	GFC_List*	staticBodies;  //For non-moving platforms
	Uint32		iterations; //How many movement subdrivisions to use
	float		step; //Should be 1/iterations

} Space; //not entity aware


Space* space_new();

void space_set_iterations(Uint32 iterations);


void space_run(Space* space);

void space_add_body(Space* space, Body* body);

void space_add_static_body(Space* space, Body* body);

Space* space_load();

int space_step_body(Space* space, Body* a);

#endif