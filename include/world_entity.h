#ifndef __WORLD_ENTITY_H__
#define __WORLD_ENTITY_H__
#include "gfc_vector.h"

#include "entity.h"

typedef enum {
	BUILDING = 0,
	ROAD = 1,
	TREE = 2,
	OTHER = 3,
	MAX
} WorldEntityType;


/*
* @brief Spawns in a world model entity for the map
* @param Position: Where to place
* @param Color: What color is it
*/
Entity* world_entity_spawn(GFC_Vector3D position, GFC_Color color);

/*
* @brief Spawns in a world model entity for the map
* @param Position: Where to place the building
* @param Color: What color is it
*/
Entity* world_entity_building_spawn(GFC_Vector3D position, GFC_Color color);

void world_entity_think(Entity* self);

void world_entity_move(Entity* self);

#endif
#pragma once
