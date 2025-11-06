/*


Vulkan: API to



Queues:

Pipelines: Specific Dataflow through the system

2D Graphics Overlay layer pipeline:



Rendering Pipeline Notes:
	-Descriptor set: UBO, Index buffer, Face buffer, and textures and binding locations






	on cpu we are grabbing the obj data

	Make a


	Things to Remove:
		-Move verticies
		-





IN BLENDER:
	-Select objects you want exported
	-File, export, (wavefront?).obj Also gltf embedded [.gltf]
	-Save a preset: gf3d
	-Must have a UV map
	-Must triangulate faces, write normals, and include UVs
	-Applied modifiers are optional
	-Y forward X left/right Z up and down
	-Save as .png with alpha channel (its the skin)


CODEBASE:
	-Meshload, follow codebase
	--Check if it already exists
	--Check if file exists
	--Make a new one if not
	--Make A new primitive





Entity:
	-













//A hashmap is a concept of a structure



JSON:
	--sj_copy:
	--sj_save: Saves over
{ <-- Root of JSON, which is an object denoted '{'
	[ <-- Array

Blender:
	--Made a plane and subdivided it a lot (3/4 or so times)
	--grabbed random verticies, then extended them and smoothed them to be mountains

	--UV Editor
	(U, then click Unwrap)
	-_Export UV map
	--Used gimp, made a new layer named texture, and export it as a skin


u






{
	"world":
	{
		"terrianMesh":"defs...."
	}

}


Collision: Walkable terrain
	--Circle Select: Select the walkable terrain
	--Shift-D to duplicate it
	--P makes it a seperate object
	--WIth everything selected, command under mesh:
				--Cleanup
				--Desimate geometry
				--Lower until its less verticies, but keep the shape. reduce the n for what your about to do next
	--File/export/OBJ
	--name it walk mask
	
Dot/Cross Product:
	--Dot Product answers question how similar are two vectors (Scaler respionse)
	--Normalize
	Cross Product: Perpindicual vector between two vectors  (New vector)

	--Math for collision is plane detection


Edge test
	--Square: 0 if its parallel or 1 if its not




More precise the collision test, more math is involved
	-Do simple first, then get more complex\
	-Ask permission not forgivness




Matricies:

3D is a 4x4 matrix
2D is a 3x3 matrix



Quaternion:
X,Y,Z direction and how much your rotating about it




Center of mass S







#include "simple_logger.h"


#include "space.h"


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
	space->step = 1/iterations;
}

void space_step(Space* space)
{
	Body* b;
	int i, c;
	if (!space)return;
	c = gfc_list_count(space->bodies);
	for (i = 0; i < c; i++)
	{
		b = gfc_list_nth(space->bodies, i);
		if (!b) continue;
		if (body_test_body(space, b)) //We hit
		{
			//handle collision

		}

		//gfc_vector3d_add(b->stepPosition, b->stepPosition, b->stepVelocity);
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

void space_add_body(Space* space, Body* body)
{
	if ((!space) || (!body)) return;
	gfc_list_append(space->bodies, body);
}

void space_add_static_body(Space* space, Body* body)
{
	if ((!space) || (!body)) return;
	gfc_list_append(space->staticBodies, body);
}



void space_edge_test(Space* space, GFC_Edge3D test, CollisionFilterMask mask);

#ifndef __BODY_H__
#define __BODY_H__

#include "gfc_list.h"
#include "gfc_primitives.h"
#include "gfc_text.h"
#include "gfc_callbacks.h"





typedef struct {
	GFC_TextLine	name;
	float			mass;
	GFC_Vector3D	position; //Center of mass
	GFC_Vector3D	velocity; //How fast we are currently moving
	GFC_List*		volumes;  //Some number of bounding shapes for collisions
	GFC_Callback	collide;
	void			(*onCollide)(struct Body_S* self, struct Body_S* other, void* data);  //Call this on hit
	void*			data;  //For callback function

	GFC_Vector3D	stepVelocity;
	GFC_Vector3D	stepPosition;

	Uint8			stopped; //Stop iterating on this body, we already hit something
} Body;


typedef void body_collide_func(Body* self, Body* other, void* data);



Body* body_new();

Body* body_free(Body* b);

void body_add_volume(Body* b, GFC_Primitive v);

void body_set_collision(Body* b, body_collide_func* collide, void* data);

void body_reset_for_updates(Body* b, float factor);


int body_test_body(Body* a, Body* b);










#endif






#ifndef __SPACE_H__
#define __SPACE_H__

#include "gfc_list.h"
#include "body.h"

typedef struct {

	GFC_List* StaticMeshes;  //Terrain(s)
	GFC_List* bodies;		//for entites and the like
	GFC_List* staticBodies;  //For non-moving platforms
	Uint32		iterations; //How many movement subdrivisions to use
	float		step; //Should be 1/iterations

} Space; //not entity aware


Space* space_new();

void space_set_iterations(Uint32 iterations);


void space_run(Space* space);

void space_add_body(Space* space, Body* body);

void space_add_static_body(Space* space, Body* body);


#endif

*/


