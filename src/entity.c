#include "simple_logger.h"

#include "entity.h"
#include "gf3d_mesh.h"
#include "gfc_vector.h"
#include "world.h"
#include "collision.h"

typedef struct 
{
	Entity* entity_list;
	Uint32	entity_max;
}EntitySystem;

static EntitySystem entity_system = { 0 };

#define DAMPEN 0.8

Entity* entity_new()
{
	int i;
	if (entity_system.entity_list)
	{
		for (i = 0; i < entity_system.entity_max; i++)
		{
			if (!entity_system.entity_list[i]._inuse)
			{
				entity_system.entity_list[i]._inuse = 1;
				entity_system.entity_list[i].color = GFC_COLOR_WHITE;
				entity_system.entity_list[i].scale = gfc_vector3d(1, 1, 1);
				return &entity_system.entity_list[i];
			}
		}
	}
	return NULL;
}

void entity_free(Entity* ent)
{
	if (!ent) return;
	if (ent->free)ent->free(ent);
	gf3d_mesh_free(ent->mesh);
	gf3d_texture_free(ent->texture);
	memset(ent, 0, sizeof(Entity));
}

//void entity_system.entity_list(Enitity* ent);

void entity_system_close()
{
	int i;
	if (entity_system.entity_list)
	{
		for (i = 0; i < entity_system.entity_max; i++)
		{
			if (entity_system.entity_list[i]._inuse)
			{
				entity_free(&entity_system.entity_list[i]);
;			}
		}
		free(entity_system.entity_list);
	}
	memset(&entity_system, 0, sizeof(EntitySystem));
}

void entity_system_init(Uint32 max_ents)
{
	if (!max_ents)
	{
		slog("Cannot init entity system with zero ents");
		return;
	}
	entity_system.entity_list = gfc_allocate_array(sizeof(Entity), max_ents);
	if (!entity_system.entity_list)
	{
		slog("Failed to allocate %i entities for the system", max_ents);
		return;
	}
	entity_system.entity_max = max_ents;
	atexit(entity_system_close);
	slog("Entity system initialized");
}

void entity_draw_shadow(Entity* ent)
{
	GFC_Vector3D drawPos;
	GFC_Matrix4 modelMat;

	if ((!ent) || (!ent->drawShadow)) return;
	gfc_vector3d_copy(drawPos, ent->position);
	drawPos.z += 0.1;
	gfc_matrix4_from_vectors(
		modelMat,
		drawPos,
		ent->rotation,
		gfc_vector3d(ent->scale.x, ent->scale.y, 0.01));
	gf3d_mesh_draw(
		ent->mesh,
		modelMat,
		gfc_color8(0, 0, 0, 128),
		ent->texture,
		gfc_vector3d(0,0,0),
		gfc_color8(0,0,0,0)
	);
}



void entity_draw(Entity* ent, GFC_Vector3D lightPos, GFC_Color lightColor)
{
	GFC_Vector3D drawPos;
	GFC_Matrix4 modelMat;
	gfc_vector3d_add(drawPos, ent->drawOffset, ent->position);
	if (!ent) return;
	gfc_matrix4_from_vectors(
		modelMat,
		drawPos,
		ent->rotation,
		ent->scale);
	gf3d_mesh_draw(
		ent->mesh,
		modelMat,
		ent->color,
		ent->texture,
		lightPos,
		lightColor
	);
	entity_draw_shadow(ent);
}

void entity_system_draw_all(GFC_Vector3D lightPos, GFC_Color lightColor)
{
	int i;
	for (i = 0; i < entity_system.entity_max; i++)
	{
		if (entity_system.entity_list[i]._inuse)
		{
			entity_draw(&entity_system.entity_list[i], lightPos, lightColor);
		}
	}
}

void entity_think(Entity* ent)
{
	if (!ent) return;

	if (ent->think) ent->think(ent);
}


void entity_system_think_all()
{
	//same as draw all but think
	int i;
	for (i = 0; i < entity_system.entity_max; i++)
	{
		if (entity_system.entity_list[i]._inuse)
		{
			entity_think(&entity_system.entity_list[i]);
		}
	}
}


void entity_system_update_all()
{
	int i;
	for (i = 0; i < entity_system.entity_max; i++)
	{
		if (entity_system.entity_list[i]._inuse)
		{
			entity_update(&entity_system.entity_list[i]);
		}
	}
}

void entity_update(Entity* ent)
{
	if (!ent) return;
	if (ent->doGenericUpdate)
	{
		gfc_vector3d_add(ent->position, ent->position, ent->velocity);
		gfc_vector3d_scale(ent->velocity, ent->velocity, DAMPEN);
		if (gfc_vector3d_magnitude_squared(ent->velocity) < 0.01)
		{
			ent->velocity = gfc_vector3d(0,0,0);
		}
	}
	if (ent->update) ent->update(ent);
}


void entity_system_move_all()
{
	int i;
	for (i = 0; i < entity_system.entity_max; i++)
	{
		if (entity_system.entity_list[i]._inuse)
		{
			entity_move(&entity_system.entity_list[i]);
		}
	}
}


void entity_move(Entity* self)
{
	if (!self) return;

	if (self->move) self->move(self);

	gfc_vector3d_add(self->velocity, self->velocity, self->acceleration);

	gfc_vector3d_add(self->position, self->position, self->velocity);



}


Uint8 entity_get_floor_pos(Entity* ent, World* world, GFC_Vector3D* contact)
{
	GFC_Vector3D up, down;
	if ((!ent) || (!world)) return 0;
	up = ent->position;
	up.z += 100;
	down = ent->position;
	//down.z = -1000;
	
	return world_edge_test(world, up, down, contact);
}

void entity_check_collisions()//, World* world);
{
	Entity* a, *b;
	int i, c;
	Uint8 collision;
	for (i = 0; i < entity_system.entity_max; i++)
	{
		a = &entity_system.entity_list[i];
		if (!a->_inuse) continue;
		if (a->collidedType == CT_None) continue;

		for (c = i + 1; c < entity_system.entity_max; c++)
		{
			b = &entity_system.entity_list[c];
			if (!b->_inuse) continue;
			if (b->collidedType == CT_None) continue;
			collision = collision_test(a, b);
			if (collision)
			{
				//if (a->collide)
				//{
				//	a->collide(a, b);
				//}
				////TODO: Do callbacks on A and B, and do whatever they need to do
				//
				//if (b->collide)
				//{
				//	b->collide(b, a);
				//}
				slog("Collision: %d, (%s <-> %s)", collision_test(a, b), a->name, b->name);
				

			}
		}

	}
}

Entity* entity_list_get()
{
	EntitySystem* system;
	system = entity_system.entity_list;
	if (!system)
	{
		slog("Cant get entity List");
		return;
	}
	return system;

}

void entity_step()
{

}

void entity_step_all()
{

}


//void entity_collide(Entity* ent, Entity* other)
//{
//	if ((!ent) || (!other)) return;
//
//	if (ent->collide) ent->collide(ent, other);
//}