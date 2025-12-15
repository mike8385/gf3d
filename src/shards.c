#include "simple_logger.h"


#include "shards.h"
#include "SDL_timer.h"
#include "player.h"




typedef struct
{
	Shard* shard_list;
	Uint32	shard_max;
}ShardSystem;

static ShardSystem shard_system = { 0 };

#define DAMPEN 0.8




Shard* shard_new()
{
	int i;
	if (shard_system.shard_list)
	{
		for (i = 0; i < shard_system.shard_max; i++)
		{
			if (!shard_system.shard_list[i]._inuse)
			{
				memset(&shard_system.shard_list[i], 0, sizeof(Shard));   //clears ALL garbage memory
				shard_system.shard_list[i]._inuse = 1;
				shard_system.shard_list[i].scale = gfc_vector3d(1, 1, 1);
				shard_system.shard_list[i].rotation = gfc_vector3d(0, 0, 0);
				shard_system.shard_list[i].color = GFC_COLOR_BLUE;
				//shard_system.shard_list[i].scale = gfc_vector3d(1, 1, 1);
				return &shard_system.shard_list[i];
			}
		}
	}
	return NULL;
}

void shard_free(Shard* ent)
{
	if (!ent) return;
	if (ent->free)ent->free(ent);
	gf3d_particle_free(ent->particle);
	gf3d_texture_free(ent->texture);
	memset(ent, 0, sizeof(Shard));
}

//void shard_system.shard_list(Enitity* ent);

void shard_system_close()
{
	int i;
	if (shard_system.shard_list)
	{
		for (i = 0; i < shard_system.shard_max; i++)
		{
			if (shard_system.shard_list[i]._inuse)
			{
				shard_free(&shard_system.shard_list[i]);
				;
			}
		}
		free(shard_system.shard_list);
	}
	memset(&shard_system, 0, sizeof(ShardSystem));
}

void shard_system_init(Uint32 max_ents)
{
	if (!max_ents)
	{
		slog("Cannot init shard system with zero ents");
		return;
	}
	shard_system.shard_list = gfc_allocate_array(sizeof(Shard), max_ents);
	if (!shard_system.shard_list)
	{
		slog("Failed to allocate %i entities for the system", max_ents);
		return;
	}
	shard_system.shard_max = max_ents;
	atexit(shard_system_close);
	slog("Shard system initialized");
}



void shard_draw(Shard* shard, GFC_Vector3D lightPos, GFC_Color lightColor)
{
	GFC_Vector3D drawPos;
	GFC_Matrix4 modelMat;
	gfc_matrix4_identity(modelMat);


	gfc_vector3d_add(drawPos, shard->drawOffset, shard->position);
	if (!shard)
	{
		slog("Couldnt draw shard");
		return;
	}
	if (shard->particle == NULL)
	{
		slog("particle is NULL");
	}
	gfc_matrix4_from_vectors(
		modelMat,
		drawPos,
		shard->rotation,
		shard->scale);
	gf3d_particle_draw(
		shard->particle,
		modelMat,
		shard->color,
		shard->texture
	);
	//slog("Shard Drawn");
}

void shard_system_draw_all(GFC_Vector3D lightPos, GFC_Color lightColor)
{
	int i;
	for (i = 0; i < shard_system.shard_max; i++)
	{
		if (shard_system.shard_list[i]._inuse)
		{
			shard_draw(&shard_system.shard_list[i], lightPos, lightColor);
		}
	}
}

void shard_think(Shard* self)
{
	if (!self) return;

	if (self->think) self->think(self);

}


void shard_system_think_all()
{
	//same as draw all but think
	int i;
	for (i = 0; i < shard_system.shard_max; i++)
	{
		if (shard_system.shard_list[i]._inuse)
		{
			shard_think(&shard_system.shard_list[i]);
		}
	}
}


void shard_system_update_all()
{
	int i;
	for (i = 0; i < shard_system.shard_max; i++)
	{
		if (shard_system.shard_list[i]._inuse)
		{
			shard_update(&shard_system.shard_list[i]);
		}
	}
}

void shard_update(Shard* ent)
{
	if (!ent) return;

	if (ent->update) ent->update(ent);
}




void shard_system_move_all()
{
	int i;
	for (i = 0; i < shard_system.shard_max; i++)
	{
		if (shard_system.shard_list[i]._inuse)
		{
			shard_move(&shard_system.shard_list[i]);
		}
	}
}


void shard_move(Shard* self)
{
	if (!self) return;

	if (self->move) self->move(self);



	//gfc_vector3d_add(self->position, self->position, self->velocity);

	//(self->position, self->position, self->velocity);



}



void shard_check_collisions()//, World* world);
{
	Shard* a;
	Entity* player;
	int i, c;
	Uint8 collision;
	for (i = 0; i < shard_system.shard_max; i++)
	{
		a = &shard_system.shard_list[i];
		if (!a->_inuse) continue;
		//if (a->collidedType == CT_None) continue;

		player = player_get_player();
		if (!player) return;

		collision = gfc_box_overlap(a->bounds, player->bounds);
		if (collision)
		{
			player_add_shard();
			shard_free(a);
			break;
		}

	}
}

Uint8 shard_collision(Shard* shard)
{
	if (!shard) return 0;
	Entity* player = player_get_player();
	if (!player) return 0;


}

Shard* shard_list_get()
{

	if (!shard_system.shard_list)
	{
		slog("Cant get shard List");
		return NULL;
	}
	return shard_system.shard_list;

}

Uint32 shard_list_get_max()
{
	if (!shard_system.shard_max)
	{
		slog("Cant get shard list max");
		return 0;
	}
	return shard_system.shard_max;
}



void shard_collide(Shard* ent, Shard* other)
{
	if ((!ent) || (!other)) return;

	if (ent->collide) ent->collide(ent, other);
}




Shard* shard_spawn(GFC_Vector3D position, GFC_Color color)
{
	Shard* self = NULL;
	Particle* particle = NULL;
	Entity* ent;

	self = shard_new();
	particle = gf3d_particle_load2(position);
	ent = entity_new();
	

	if (!self) return NULL;
	if (!particle)
	{
		slog("gf3d_particle_load2: FAILED to allocate particle!");
		return NULL;
	}
	if (!ent)
	{
		slog("FAILED to allocate entity for a shard!");
		return NULL;
	}

	self->rotation = gfc_vector3d(0, 0, 0);
	gfc_line_cpy(self->name, "shards");
	self->particle = particle;
	self->color = color;
	self->position = position;
	self->drawOffset = gfc_vector3d(0, 0, 6);
	//self->move = shard_move;
	//self->think = shard_think;
	//self->update = shard_update;
	self->bounds = gfc_box(self->position.x, self->position.y, self->position.z, 1, 1, 1);
	return self;
}
