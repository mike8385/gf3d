#include "simple_logger.h"

#include "monster.h"


Entity* monster_spawn(GFC_Vector3D position, GFC_Color color)
{
	Entity* self;
	self = entity_new();
	if (!self) return NULL;
	gfc_line_cpy(self->name, "notAugmon");
	self->mesh = gf3d_mesh_load_obj("models/dino/dino.obj");
	self->texture = gf3d_texture_load("models/dino/dino.png");
	self->color = color;
	self->drawOffset = gfc_vector3d(0, 0, 6);
	entity_get_floor_pos(self, world_get_the(), &self->position);
	self->position = position;
	self->think = monster_think;
	self->update = monster_update;
	self->rotation.z = 180;
	self->velocity = gfc_vector3d(0, 0, 0);
	self->acceleration = gfc_vector3d(0, 0, 0);
	//self->velocity.z = gfc_crandom();
	slog("Made it here");
	return self;
}

  
void monster_think(Entity* self)
{
	if (!self) return;
	monster_move(self);


}

void monster_update(Entity* self)
{
	GFC_Vector3D floorPos;
	if (!self) return;
	entity_get_floor_pos(self, world_get_the(), &floorPos);
	self->position.z = floorPos.z;
}

void monster_move(Entity* self)
{

}