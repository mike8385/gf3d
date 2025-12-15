#ifndef __SHARDS_H__
#define __SHARDS_H__

#include <stdalign.h>
#include <vulkan/vulkan.h>


#include "entity.h"

#include "particles.h"


typedef struct Shard_S
{
	Uint8							_inuse;
	GFC_TextLine					name;
	Particle* particle;
	Texture* texture;
	GFC_Color						color;
	GFC_Matrix4						matrix;
	GFC_Vector3D					position;
	GFC_Vector3D					drawOffset;
	GFC_Vector3D					rotation;
	GFC_Vector3D					scale;

	GFC_Box							bounds;
	


	void							(*draw)(struct Shard_S* self);
	void							(*think)(struct Shard_S* self);
	void							(*update)(struct Shard_S* self);
	void							(*move)(struct Shard_S* self);
	void							(*free)(struct Shard_S* self);
	void							(*collide)(struct Shard_S* self, struct Shard_S* other);


	void* data;
}Shard;

Shard* shard_new();

/*
* @brief free a previously new'd shard
* @param ent tje entioty to be freed
* @note the memory address should no longer be used
*/
void shard_free(Shard* ent);

/*
@brief Initilizes the enttiy subsystem
@param max_ents jpw ,amu tp sipport concurrently
*/
void shard_system_init(Uint32 max_ents);

/*
* @brief draw all entittes with the provided light
* @param lightPos where the loght is in world space
* @
*/
void shard_system_draw_all(GFC_Vector3D lightPos, GFC_Color lightColor);


void shard_system_think_all();

void shard_system_update_all();

void shard_think(Shard* ent);

void shard_update(Shard* ent);

void shard_system_move_all();

void shard_move();

void shard_draw_shadow(Shard* ent);

/*
* @brief checks if an shard collides with worlds entities
* @param self the shard itself
* @param the world itself
* @note will access entities of world within code
*/
void shard_check_collisions();


Shard* shard_list_get();

Uint32 shard_list_get_max();

Uint8 shard_hitscan(GFC_Vector3D start, GFC_Vector3D end);

Shard* shard_spawn(GFC_Vector3D position, GFC_Color color);


#endif