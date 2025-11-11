#ifndef __PLAYER_H__
#define __PLAYER_H__
#include "gfc_vector.h"

#include "entity.h"



/*
* @brief Spawns in a player in the world
* @param position - where to spawn player
* @param color - the color that will be rendered for player
* @return the entity player itself
*/
Entity* player_spawn(GFC_Vector3D position, GFC_Color color);

/*
* @brief The think function for the player
* @param self - the player entity itself
* @note handles movement and compute velocity
*/
void player_think(Entity* self);

/*
* @brief The think function for the player
* @param self - the player entity itself
*/
void player_move(Entity* self);

/*
* @brief The think function for the player
* @param self - the player entity itself
*/
void player_free(Entity* self);
/*
* @brief The think function for the player
* @param self - the player entity itself
*/
void player_update(Entity* self);



/*
* @brief Sets the
* @param self - the player entity itself
*/
void player_set_camera_ent(Entity* self, Entity* cam);

void player_collide(Entity* self, Entity* other);


void player_attack(Entity* self);




#endif