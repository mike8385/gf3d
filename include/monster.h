#ifndef __MONSTER_H__
#define __MONSTER_H__
#include "gfc_vector.h"

#include "entity.h"




Entity* monster_spawn(GFC_Vector3D position, GFC_Color color);


void monster_think(Entity* self);

void monster_move(Entity* self);

void monster_update(Entity* self);

#endif
#pragma once
