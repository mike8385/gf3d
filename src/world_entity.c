#include "simple_logger.h"

#include "world_entity.h"
#include "world.h"

typedef struct {
	Uint8			type;
} WorldEntityData; //The data attributes for each world entity (this means if its a tree or building and what else)


Entity* world_entity_spawn(GFC_Vector3D position, GFC_Color color)
{
	Entity* self;
	self = entity_new();
	if (!self) return NULL;
	gfc_line_cpy(self->name, "notAugmon");
	self->mesh = gf3d_mesh_load_obj("models/dino/dino.obj");
	self->texture = gf3d_texture_load("models/dino/dino.png");
	self->color = color;
	self->position = position;
	self->think = world_entity_think;
	self->rotation.z = 180;
	self->velocity.x = 0;
	//slog("Made it here");
	return self;
}

Entity* world_entity_building_spawn(GFC_Vector3D position, GFC_Color color)
{
	Entity* self;
	World* world;
	WorldEntityData* data;
	data = gfc_allocate_array(sizeof(WorldEntityData), 1);
	self = entity_new();
	if (!self) return NULL;
	gfc_line_cpy(self->name, "Building1");
	self->mesh = gf3d_mesh_load_obj("models/terrain/building1.obj");
	self->texture = gf3d_texture_load("models/terrain/building1.png");
	self->color = color;
	self->position = position;
	self->bounds = gfc_box(position.x - 100, position.y - 100, position.z, 200, 200, 50);
	self->think = world_entity_think;
	data->type = BUILDING;
	self->collidedType = CT_Building;
	self->data = data;
	world = world_get_the();
	if (!world)
	{
		slog("Couldnt get world data");
		return NULL;
	}
	gfc_list_append(world->entities, self);
	world->maxEnt += 1;
	slog("Got world data to spawn building");
	return self;
}



void world_entity_think(Entity* self)
{
	if (!self) return;
	world_entity_move(self);


}

void world_entity_move(Entity* self)
{

}