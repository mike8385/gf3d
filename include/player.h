#ifndef __PLAYER_H__
#define __PLAYER_H__
#include "gfc_vector.h"

#include "entity.h"




Entity* player_spawn(GFC_Vector3D position, GFC_Color color);


void player_think(Entity* self);

void player_move(Entity* self);
void player_free(Entity* self);
void player_update(Entity* self);
void player_move(Entity* self);



void player_set_camera_ent(Entity* self, Entity* cam);

#endif