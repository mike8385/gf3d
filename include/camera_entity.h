#ifndef __CAMERA_ENTITY_H__
#define __CAMERA_ENTITY_H__


#include "entity.h"
#include "gfc_vector.h"

#include "simple_logger.h"

#include "gfc_input.h"
#include "gf3d_camera.h"

#include "camera_entity.h"




/*
* @brief The free function for freeing the camera entity
* @param self the entity itself
*/
void camera_entity_free(Entity* self);

/*
* @brief The think function for the camera entity
* @param self the entity itself
*/
void camera_entity_think(Entity* self);

/*
* @brief The spawn function for the camera entity based on the target it wants to look at
* @param position the position of said target
* @param target the entity the camera wants to look at
* @return entity the camera entity itself
*/
Entity* camera_entity_spawn(GFC_Vector3D position, Entity* target);


#endif

