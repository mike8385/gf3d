#ifndef __POWER_H__
#define __POWER_H__

#include <stdalign.h>
#include <vulkan/vulkan.h>

#include "gfc_text.h"
#include "gfc_matrix.h"

#include "particles.h"

typedef enum
{
	PT_NONE = 1,
	PT_TIMER = 2, //This is if the power needs a TTL (ex. lightning behind player)
	PT_DISTANCE = 4 // THis is if the power needs to dissapear after distance (ex. shooting)

}PowerType;


typedef struct Power_S
{
	Uint8							_inuse;
	GFC_TextLine					name;
	Particle*						particle;
	Texture*						texture;
	GFC_Color						color;
	GFC_Matrix4						matrix;
	GFC_Vector3D					position;
	GFC_Vector3D					drawOffset;
	GFC_Vector3D					rotation;
	GFC_Vector3D					scale;
	GFC_Vector3D					velocity;
	GFC_Vector3D					spawnPos;
	float							spawnTime;
	float							TTL;

	PowerType						powerType;



	GFC_Vector3D					acceleration;


	void							(*draw)(struct Power_S* self);
	void							(*think)(struct Power_S* self);
	void							(*update)(struct Power_S* self);
	void							(*move)(struct Power_S* self);
	void							(*free)(struct Power_S* self);
	void							(*collide)(struct Power_S* self, struct Power_S* other);


	void* data;
}Power;

Power* power_new();

/*
* @brief free a previously new'd power
* @param ent tje entioty to be freed
* @note the memory address should no longer be used
*/
void power_free(Power* ent);

/*
@brief Initilizes the enttiy subsystem
@param max_ents jpw ,amu tp sipport concurrently
*/
void power_system_init(Uint32 max_ents);

/*
* @brief draw all entittes with the provided light
* @param lightPos where the loght is in world space
* @
*/
void power_system_draw_all(GFC_Vector3D lightPos, GFC_Color lightColor);


void power_system_think_all();

void power_system_update_all();

void power_think(Power* ent);

void power_update(Power* ent);

void power_system_move_all();

void power_move();

void power_draw_shadow(Power* ent);

/*
* @brief checks if an power collides with worlds entities
* @param self the power itself
* @param the world itself
* @note will access entities of world within code
*/
void power_check_collisions();


Power* power_list_get();

Uint32 power_list_get_max();

Uint8 power_hitscan(GFC_Vector3D start, GFC_Vector3D end);

Power* power_spawn(GFC_Vector3D position, GFC_Color color);

#endif