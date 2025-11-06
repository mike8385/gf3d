#include "simple_logger.h"


#include "space.h"
#include "entity.h"

/*
Make each space make a new space, add all entities to said space,
and set this up in init/run. Then ion run i think i want to make all steps

*/


typedef enum
{
	CF_None = 0,
	CF_Player = 1,
	CF_Monster = 2,
	CF_Item = 4,
	CF_World = 8,
	CF_Interactable = 16
}CollisionFilterMask;

Space* space_new()
{
	Space* space;
	space = gfc_allocate_array(sizeof(Space), 1);
	if (!space)return NULL;
	space->StaticMeshes = gfc_list_new();
	space->bodies = gfc_list_new();
	space->staticBodies = gfc_list_new();
	space->iterations = 1;
	space->step = 1;

	return space;
}

void space_set_iterations(Space* space, Uint32 iterations)
{
	if (!space) return;
	if (iterations == 0)
	{
		slog("Cannot make a space have 0 iterations");
		return;
	}
	space->iterations = iterations;
	space->step = 1.0f / (float)iterations;
}

//Before i step update all entities in space

int space_step_body(Space* space, Body* a)
{
	Body* b;
	int i, c;
	c = gfc_list_count(space->bodies);
	for (i = 0; i < c; i++)
	{
		b = gfc_list_nth(space->bodies, i);
		if (!b) continue;

		if (body_test_body(a, b)) //We hit
		{
			slog("Collision happened");
			return 1;
		}
	}
	return 0;
}

void space_step(Space* space)
{
	Body* b;
	int i, c;
	if (!space) return;
	c = gfc_list_count(space->bodies);
	for (i = 0; i < c; i++)
	{
		b = gfc_list_nth(space->bodies, i);
		if (!b) continue;
		gfc_vector3d_add(b->stepPosition, b->stepPosition, b->stepVelocity);
	}
	for (i = 0; i < c; i++)
	{
		b = gfc_list_nth(space->bodies, i);
		if (!b) continue;
		if (space_step_body(space, b)) //We hit
		{

		}

	}

}


void space_run(Space* space)
{
	int i, c;
	if (!space) return;
	//Prepare the space for an iteration
	c = gfc_list_count(space->bodies);
	for (i = 0; i < c; i++)
	{
		body_reset_for_updates(gfc_list_nth(space->bodies, i), space->step);
	}
	for (i = 0; i < space->iterations; i++)
	{
		space_step(space);
	}
}

//Add entities shen you create the space, a copy of each entity
void space_add_body(Space* space, Body* body)
{
	if ((!space)) return;
	gfc_list_append(space->bodies, body);
}

void space_add_static_body(Space* space, Body* body);



void space_edge_test(Space* space, GFC_Edge3D test, CollisionFilterMask mask);

Space* space_load()
{
	int i, c;
	Entity* ent_list;
	Body* b;
	Uint32 ent_max;
	Space* space;
	Uint32 iter = 10;

	ent_list = entity_list_get();
	ent_max = entity_list_get_max();
	if (!ent_list) return;
	if (!ent_max) return;
	space = space_new();
	if (!space)
	{
		slog("No space created");
		return;
	}
	space_set_iterations(space, iter);
	for (i = 0; i < ent_max; i++)
	{
		if (!ent_list[i]._inuse) continue;
		b = body_new();
		body_add_bounds(b, &ent_list[i]);
		space_add_body(space, b);
	}


	return space;
}

