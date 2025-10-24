#ifndef __CAMERA_ENTITY_H__
#define __CAMERA_ENTITY_H__


#include "entity.h"
#include "gfc_vector.h"

#include "simple_logger.h"

#include "gfc_input.h"
#include "gf3d_camera.h"

#include "camera_entity.h"





void camera_entity_free(Entity* self);

void camera_entity_think(Entity* self);

Entity* camera_entity_spawn(GFC_Vector3D position, Entity* target);


#endif

