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






Sound: OUTPUT and INPUT channels

Mixer: Mixes different audio output channels

1 channel dedicated for background music

Level has music filename

if file length is > 0, try to load file

Mix_Music

Mix_LoadMUS

MX_FreeMusic when your done with it.

MX_PlayMusic (-1 on if you want it to be repeated forever)

.mp3 is suppored if your pc has mp3 libraries
.wav, .tif should work

Mix_Chunk (sound chunk, effect, byte)
smaller wave of special effects

Allocate channels for chunks
Halt playing music before freeing it


Mix_PlayChannel (-1 for whatever is available, chunk, and # of loops)

loops-> (0 if its once, 2 if you want it 2 times)

if no channels available it wont play the sound

Maxsounds: How many audio files you can load at once

load_music ->





Lighting:









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






#include "simple_logger.h"

#include "entity.h"
#include "gf3d_mesh.h"
#include "gfc_vector.h"
#include "world.h"
#include "collision.h"

typedef struct
{
	Entity* entity_list;
	Uint32	entity_max;
}EntitySystem;

static EntitySystem entity_system = { 0 };

#define DAMPEN 0.8

Entity* entity_new()
{
	int i;
	if (entity_system.entity_list)
	{
		for (i = 0; i < entity_system.entity_max; i++)
		{
			if (!entity_system.entity_list[i]._inuse)
			{
				entity_system.entity_list[i]._inuse = 1;
				entity_system.entity_list[i].color = GFC_COLOR_WHITE;
				entity_system.entity_list[i].scale = gfc_vector3d(1, 1, 1);
				return &entity_system.entity_list[i];
			}
		}
	}
	return NULL;
}

void entity_free(Entity* ent)
{
	if (!ent) return;
	if (ent->free)ent->free(ent);
	gf3d_mesh_free(ent->mesh);
	gf3d_texture_free(ent->texture);
	memset(ent, 0, sizeof(Entity));
}

//void entity_system.entity_list(Enitity* ent);

void entity_system_close()
{
	int i;
	if (entity_system.entity_list)
	{
		for (i = 0; i < entity_system.entity_max; i++)
		{
			if (entity_system.entity_list[i]._inuse)
			{
				entity_free(&entity_system.entity_list[i]);
;			}
		}
		free(entity_system.entity_list);
	}
	memset(&entity_system, 0, sizeof(EntitySystem));
}

void entity_system_init(Uint32 max_ents)
{
	if (!max_ents)
	{
		slog("Cannot init entity system with zero ents");
		return;
	}
	entity_system.entity_list = gfc_allocate_array(sizeof(Entity), max_ents);
	if (!entity_system.entity_list)
	{
		slog("Failed to allocate %i entities for the system", max_ents);
		return;
	}
	entity_system.entity_max = max_ents;
	atexit(entity_system_close);
	slog("Entity system initialized");
}

void entity_draw_shadow(Entity* ent)
{
	GFC_Vector3D drawPos;
	GFC_Matrix4 modelMat;

	if ((!ent) || (!ent->drawShadow)) return;
	gfc_vector3d_copy(drawPos, ent->position);
	drawPos.z += 0.1;
	gfc_matrix4_from_vectors(
		modelMat,
		drawPos,
		ent->rotation,
		gfc_vector3d(ent->scale.x, ent->scale.y, 0.01));
	gf3d_mesh_draw(
		ent->mesh,
		modelMat,
		gfc_color8(0, 0, 0, 128),
		ent->texture,
		gfc_vector3d(0,0,0),
		gfc_color8(0,0,0,0)
	);
}



void entity_draw(Entity* ent, GFC_Vector3D lightPos, GFC_Color lightColor)
{
	GFC_Vector3D drawPos;
	GFC_Matrix4 modelMat;
	gfc_vector3d_add(drawPos, ent->drawOffset, ent->position);
	if (!ent)
	{
		slog("Couldnt draw entity");
		return;
	}
	gfc_matrix4_from_vectors(
		modelMat,
		drawPos,
		ent->rotation,
		ent->scale);
	gf3d_mesh_draw(
		ent->mesh,
		modelMat,
		ent->color,
		ent->texture,
		lightPos,
		lightColor
	);
	entity_draw_shadow(ent);
}

void entity_system_draw_all(GFC_Vector3D lightPos, GFC_Color lightColor)
{
	int i;
	for (i = 0; i < entity_system.entity_max; i++)
	{
		if (entity_system.entity_list[i]._inuse)
		{
			entity_draw(&entity_system.entity_list[i], lightPos, lightColor);
		}
	}
}

void entity_think(Entity* ent)
{
	if (!ent) return;

	//ent->oldPos = ent->position;
	if (ent->think) ent->think(ent);
}


void entity_system_think_all()
{
	//same as draw all but think
	int i;
	for (i = 0; i < entity_system.entity_max; i++)
	{
		if (entity_system.entity_list[i]._inuse)
		{
			entity_think(&entity_system.entity_list[i]);
		}
	}
}


void entity_system_update_all()
{
	int i;
	for (i = 0; i < entity_system.entity_max; i++)
	{
		if (entity_system.entity_list[i]._inuse)
		{
			entity_update(&entity_system.entity_list[i]);
		}
	}
}

void entity_update(Entity* ent)
{
	if (!ent) return;
	if (ent->doGenericUpdate)
	{
		gfc_vector3d_scale(ent->velocity, ent->velocity, DAMPEN);
		if (gfc_vector3d_magnitude_squared(ent->velocity) < 0.01)
		{
			ent->velocity = gfc_vector3d(0,0,0);
		}
	}
	if (ent->update) ent->update(ent);
}




void entity_system_move_all()
{
	int i;
	for (i = 0; i < entity_system.entity_max; i++)
	{
		if (entity_system.entity_list[i]._inuse)
		{
			entity_system.entity_list[i].oldPos = entity_system.entity_list[i].position;
			entity_move(&entity_system.entity_list[i]);
			entity_system.entity_list[i].newPos = entity_system.entity_list[i].position;
			entity_system.entity_list[i].newPos.x += entity_system.entity_list[i].velocity.x;
			entity_system.entity_list[i].newPos.y += entity_system.entity_list[i].velocity.y;
		}
	}
}


void entity_move(Entity* self)
{

	if (!self) return;
	/*if ((!self->position) || (!self->velocity))

if (self->move) self->move(self);

gfc_vector3d_add(self->velocity, self->velocity, self->acceleration);
self->position.z += self->velocity.z;


//gfc_vector3d_add(self->position, self->position, self->velocity);


}


Uint8 entity_get_floor_pos(Entity* ent, World* world, GFC_Vector3D* contact)
{
	GFC_Vector3D up, down;
	if ((!ent) || (!world)) return 0;
	up = ent->position;
	up.z += 100;
	down = ent->position;
	//down.z = -1000;

	return world_edge_test(world, up, down, contact);
}



void entity_check_collisions()
{
	for (int i = 0; i < entity_system.entity_max; i++)
	{
		Entity* a = &entity_system.entity_list[i];
		if (!a->_inuse) continue;
		if (a->collidedType == CT_None) continue;

		Uint8 blocked = 0;

		for (int c = 0; c < entity_system.entity_max; c++)
		{
			if (i == c) continue;

			Entity* b = &entity_system.entity_list[c];
			if (!b->_inuse) continue;
			if (b->collidedType == CT_None) continue;

			if (collision_test(a, b))
			{
				blocked = 1;
				break;
			}
		}

		if (blocked)
		{
			// Reject horizontal movement
			a->position.x = a->oldPos.x;
			a->position.y = a->oldPos.y;

			// Stop horizontal sliding
			a->velocity.x = 0;
			a->velocity.y = 0;
		}
		else
		{
			// Accept horizontal movement
			a->position.x = a->newPos.x;
			a->position.y = a->newPos.y;
		}

		// DO NOT touch a->position.z or a->velocity.z here

	}
}


Entity* entity_list_get()
{

	if (!entity_system.entity_list)
	{
		slog("Cant get entity List");
		return NULL;
	}
	return entity_system.entity_list;

}

Uint32 entity_list_get_max()
{
	if (!entity_system.entity_max)
	{
		slog("Cant get entity list max");
		return 0;
	}
	return entity_system.entity_max;
}



//void entity_collide(Entity* ent, Entity* other)
//{
//	if ((!ent) || (!other)) return;
//
//	if (ent->collide) ent->collide(ent, other);
//}










#include "simple_logger.h"

#include "player.h"

#include "space.h"

#include "gfc_input.h"

typedef struct
{
	Entity*		cam;
	Uint8		canFloat;
}PlayerEntityData;

Entity* player_spawn(GFC_Vector3D position, GFC_Color color)
{
	Space* space = NULL;
	Entity* self = NULL;
	Body* body = NULL;
	self = entity_new();
	PlayerEntityData* data = NULL;
	if (!self) return NULL;
	data = gfc_allocate_array(sizeof(PlayerEntityData), 1);
	self->data = data;
	gfc_line_cpy(self->name, "notAugmon");
	self->mesh = gf3d_mesh_load_obj("models/dino/dino.obj");
	self->texture = gf3d_texture_load("models/dino/dino.png");
	self->color = color;
	self->position = position;
	self->bounds = gfc_box(self->position.x -7, self->position.y + 7, self->position.z, 14, 14, 14);
	self->drawOffset = gfc_vector3d(0, 0, 6);
	self->drawShadow = 1;
	self->think = player_think;
	self->free = player_free;
	self->update = player_update;
	//self->move = player_move;
	//Sself->rotation.z = 90;
	self->velocity = gfc_vector3d(0,0,0);
	//self->velocity.z = gfc_crandom();
	self->doGenericUpdate = 1;
	entity_get_floor_pos(self, world_get_the(), &self->position);
	self->justSpawned = 1;
	self->canJump = 1;
	self->onGround = 1;
	self->collidedType = CT_Player;
	//self->collide = player_collide;
	data->canFloat = 0;

	//Body Stuff
	body = body_new();
	if (!body)
	{
		slog("No body found");
		return NULL;
	}
	self->body = body;

	//Space Stuff
	space = space_get_the();
	if (!space)
	{
		slog("Cant find space");
		return NULL;
	}
	space_add_body(space, body);
	body_add_data(body, self);

	return self;
}
//7 side, 7 down, 2 up

void player_free(Entity* self)
{
	if (!self) return;

}


void player_set_camera_ent(Entity* self, Entity* cam)
{
	PlayerEntityData* data = NULL;
	if ((!self) || (!cam)) return;
	data = self->data;
	data->cam = cam;
}



void player_think(Entity* self)
{
	Body* body = NULL;
	Space* space = NULL;
	if (!self) return;
	player_move(self);

	//Space Stuff
	space = space_get_the();
	if (!space)
	{
		slog("No space available");
		return;
	}

	//Body Stuff
	//body_reset_for_updates(body, space->step);
	player_attack(self);
	//slog("Player Bounds: %f,%f,%f,%f,%f,%f", self->bounds.x, self->bounds.y, self->bounds.z, self->bounds.w, self->bounds.h, self->bounds.d);
}


void player_update(Entity* self)
{
	GFC_Vector3D floorPos;
	PlayerEntityData* data = NULL;
	if ((!self) || (!self->data)) return;
	data = self->data;
	entity_get_floor_pos(self, world_get_the(), &floorPos);





	self->bounds = gfc_box(self->position.x - 7, self->position.y + 7, self->position.z, 14, 14, 14);

	if (self->justSpawned)
	{
		self->position.z = floorPos.z;
		self->justSpawned = 0;

	}

	//If the player is at floor or below, they are supposed to be on ground, set to ground
	//And set onground to 1
	if (self->position.z <= floorPos.z)
	{
		self->onGround = 1;
		self->position.z = floorPos.z;
		self->canJump = 1;
		self->velocity.z = 0;
		self->acceleration.z = 0;
		data->canFloat = 0;
	}

	if (data->canFloat == 1)
	{
		if (gfc_input_command_down("jump"))
		{
			self->acceleration.z = -.1;

		}
		else
		{
			self->acceleration.z = -.5;
		}
	}



	////This helps with uneven terrain
	////if (fabs(self->position.z <= position.z) > 1.0f)
	//if (self->position.z <= position.z)
	//{
	//	self->onGround = 0;
	//}
	//else
	//{
	//	self->canJump = 1;
	//	self->onGround = 1;
	//	self->acceleration.z = 0;
	//	self->position.z = position.z;// +5;

	//}


}

void player_move(Entity* self)
{
	float move = 0;
	float step;
	float dash = 2;
	GFC_Vector3D cameraDir;
	PlayerEntityData* data = NULL;
	if ((!self) || (!self->data)) return;
	data = self->data;
	if (!data->cam) return;
	gfc_vector3d_sub(cameraDir, self->position, data->cam->position); //Gets vector from camera to player
	gfc_vector3d_normalize(&cameraDir); //Now its a direction arrow, where to go like position wise, points at player

	if (gfc_input_command_down("run"))
	{
		step = 0.9;
	}
	else
	{
		step = 0.5;
	}



	//Movement Up/Down
	if (gfc_input_command_down("walkforward"))
	{
		//slog("Pressed w");
		move += step;
		//if (gfc_input_command_down("dash"))
		//{
		//	self->position.x =  self->position.x + dash;
		//}
	}
	if (gfc_input_command_down("walkback"))
	{
		//slog("Pressed s");
		move -= step;
	}

	if (move) //if move happened
	{
		gfc_vector2d_scale(cameraDir, cameraDir, move); //Scale the direction, so it orients to player?
		gfc_vector2d_add(self->velocity, self->velocity, cameraDir); //Add how much the player 'moved' and the current velocity at
																	//which they moved and set it to velocity new
	}

	move = 0;
	gfc_vector3d_sub(cameraDir, self->position, data->cam->position); //Gets vector from camera to player
	gfc_vector3d_normalize(&cameraDir); //Now its a direction arrow, where to go like position wise, points at player
	gfc_vector3d_rotate_about_z(&cameraDir, GFC_HALF_PI); //Now tracks left/right
	//Movement Up/Down
	if (gfc_input_command_down("walkright"))
	{
		//slog("Pressed w");
		move -= step;
	}
	if (gfc_input_command_down("walkleft"))
	{
		//slog("Pressed s");
		move += step;
	}

	if (move) //if move happened
	{
		gfc_vector2d_scale(cameraDir, cameraDir, move); //Scale the direction, so it orients to player?
		gfc_vector2d_add(self->velocity, self->velocity, cameraDir); //Add how much the player 'moved' and the current position at
																		//which they moved and set it to position new
	}

	if ((self->velocity.x) || (self->velocity.y))
	{
		self->rotation.z = gfc_vector2d_angle(gfc_vector3dxy(self->velocity));
	}

	if (gfc_input_command_down("jump") && (self->canJump == 1))
	{
		if (self->onGround == 1)
		{
			//slog("CanJump: %f", self->canJump);
			self->canJump = 0;
			self->onGround = 0;
			self->velocity.z = 10;
			self->acceleration.z = -.5;
			//Make accel .1 when i glide
			data->canFloat = 1;
		}


	}



}

void player_collide(Entity* self, Entity* other)
{

	Uint8 pType, oType;
	if ((!self) || (!other)) return;
	if ((!self->collidedType) || (!other->collidedType)) return;
	pType = self->collidedType;
	oType = other->collidedType;
	if (other->collidedType != CT_Player)
	{
		/*self->position = 0;
	}

}


void player_attack(Entity* self)
{
	int mx, my;

	GFC_Vector3D projectilePos;
	GFC_Vector3D projectileDir;

	PlayerEntityData* data;


	if ((!self) || (!self->data)) return;

	if (SDL_GetMouseState(&mx, &my) == SDL_BUTTON(1))
	{

	}



	//if (SDL_GetMouseState(&mx, &my) Somehow update pos here)
	//{
	//Make an edge
	//gfc_vector3d_sub(projectileDir, self->position, projectilePos); //Gets vector from camera to player
	//gfc_vector3d_normalize(&projectileDir); //Now its a direction arrow, where to go like position wise, points at player

	// Make a raytrace from the player to shooting position
	// Check if the line hits an entity what type it is
	// If its not an enemy, return
	// if it is, damage and return
	//	slog("CLICKED");
	//}
	//ALSO make sure you DONT do particles unless you check if its within a certain distance


}





#include "simple_logger.h"

#include "collision.h"
#include "gfc_shape.h"

Uint8 collision_test(Entity* self, Entity* other)
{
	int i, c;
	int j, d;
	GFC_Box aBox;
	GFC_Box bBox;
	GFC_Vector3D aOffset;
	GFC_Vector3D bOffset;
	if ((!self) || (!other)) return 0;
	if (self == other) return 0;

	if (self->bounds.w <= 0 || self->bounds.h <= 0 || self->bounds.d <= 0) return 0;
	if (other->bounds.w <= 0 || other->bounds.h <= 0 || other->bounds.d <= 0) return 0;


	//Get bounds for each entity
	aBox = self->bounds;
	//gfc_vector3d_add(self->newPos, self->oldPos, self->velocity);

	bBox = other->bounds;
	//gfc_vector3d_add(other->newPos, other->oldPos, other->velocity);


	//Reason we do this math is because the way we calculate step posiiton, allows the movement changes to work
	aOffset = gfc_vector3d(self->newPos.x - self->oldPos.x,
		self->newPos.y - self->oldPos.y,
		self->newPos.z - self->oldPos.z);
	aBox.x += aOffset.x; aBox.y += aOffset.y; aBox.z += aOffset.z;

	bOffset = gfc_vector3d(other->newPos.x - other->oldPos.x,
		other->newPos.y - other->oldPos.y,
		other->newPos.z - other->oldPos.z);
	bBox.x += bOffset.x; bBox.y += bOffset.y; bBox.z += bOffset.z;

	if (gfc_box_overlap(aBox, other->bounds))
	{
		slog("Collisiion");
		return 1;
	}
	else
	{
		return 0;
	}
}








*/


