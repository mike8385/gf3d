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










*/

/* 
Monster C
#include "simple_logger.h"

#include "monster.h"


Entity* monster_spawn(GFC_Vector3D position, GFC_Color color)
{
	Entity* self;
	self = entity_new();
	if (!self) return NULL;
	gfc_line_cpy(self->name, "notAugmon");
	self->mesh = gf3d_mesh_load_obj("models/dino/dino.obj");
	self->texture = gf3d_texture_load("models/dino/dino.png");
	self->color = color;
	self->position = position;
	self->think = monster_think;
	self->rotation.z = 180;
	self->velocity.x = 0;
	//self->velocity.z = gfc_crandom();
	slog("Made it here");
	return self;
}


void monster_think(Entity* self)
{
	if (!self) return;
	monster_move(self);


}

void monster_move(Entity* self)
{

}
*/
