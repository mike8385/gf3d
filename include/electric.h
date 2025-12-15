#ifndef __ELECTRIC_H__
#define __ELECTRIC_H__

#include <stdalign.h>
#include <vulkan/vulkan.h>


#include "power.h"

Power* electric_spawn(GFC_Vector3D position, GFC_Color color);

void electric_free(Power* self);




void electric_think(Power* self);


void electric_update(Power* self);

void electric_move(Power* self);


void electric_collide(Power* self, Power* other);



void electric_attack(Power* self);


#endif