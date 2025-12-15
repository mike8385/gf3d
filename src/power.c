#include "simple_logger.h"

#include "power.h"


typedef struct
{
	Power* power_list;
	Uint32	power_max;
}PowerSystem;

static PowerSystem power_system = { 0 };

#define DAMPEN 0.8




Power* power_new()
{
	int i;
	if (power_system.power_list)
	{
		for (i = 0; i < power_system.power_max; i++)
		{
			if (!power_system.power_list[i]._inuse)
			{
				memset(&power_system.power_list[i], 0, sizeof(Power));   //clears ALL garbage memory
				power_system.power_list[i]._inuse = 1;
				power_system.power_list[i].scale = gfc_vector3d(1, 1, 1);
				power_system.power_list[i].rotation = gfc_vector3d(0, 0, 0);
				power_system.power_list[i].color = GFC_COLOR_BLUE;
				//power_system.power_list[i].scale = gfc_vector3d(1, 1, 1);
				return &power_system.power_list[i];
			}
		}
	}
	return NULL;
}

void power_free(Power* ent)
{
	if (!ent) return;
	if (ent->free)ent->free(ent);
	//particle_free(ent->particle);
	gf3d_texture_free(ent->texture);
	memset(ent, 0, sizeof(Power));
}

//void power_system.power_list(Enitity* ent);

void power_system_close()
{
	int i;
	if (power_system.power_list)
	{
		for (i = 0; i < power_system.power_max; i++)
		{
			if (power_system.power_list[i]._inuse)
			{
				power_free(&power_system.power_list[i]);
				;
			}
		}
		free(power_system.power_list);
	}
	memset(&power_system, 0, sizeof(PowerSystem));
}

void power_system_init(Uint32 max_ents)
{
	if (!max_ents)
	{
		slog("Cannot init power system with zero ents");
		return;
	}
	power_system.power_list = gfc_allocate_array(sizeof(Power), max_ents);
	if (!power_system.power_list)
	{
		slog("Failed to allocate %i entities for the system", max_ents);
		return;
	}
	power_system.power_max = max_ents;
	atexit(power_system_close);
	slog("Power system initialized");
}



void power_draw(Power* power, GFC_Vector3D lightPos, GFC_Color lightColor)
{
	GFC_Vector3D drawPos;
	GFC_Matrix4 modelMat;
	gfc_matrix4_identity(modelMat);


	gfc_vector3d_add(drawPos, power->drawOffset, power->position);
	if (!power)
	{
		slog("Couldnt draw power");
		return;
	}
	if (power->particle == NULL)
	{
		slog("particle is NULL");
	}
	gfc_matrix4_from_vectors(
		modelMat,
		drawPos,
		power->rotation,
		power->scale);
	gf3d_particle_draw(
		power->particle,
		modelMat,
		power->color,
		power->texture
	);
	//slog("Power Drawn");
}

void power_system_draw_all(GFC_Vector3D lightPos, GFC_Color lightColor)
{
	int i;
	for (i = 0; i < power_system.power_max; i++)
	{
		if (power_system.power_list[i]._inuse)
		{
			power_draw(&power_system.power_list[i], lightPos, lightColor);
		}
	}
}

void power_think(Power* self)
{
	if (!self) return;

	if (self->think) self->think(self);
	
}


void power_system_think_all()
{
	//same as draw all but think
	int i;
	for (i = 0; i < power_system.power_max; i++)
	{
		if (power_system.power_list[i]._inuse)
		{
			power_think(&power_system.power_list[i]);
		}
	}
}


void power_system_update_all()
{
	int i;
	for (i = 0; i < power_system.power_max; i++)
	{
		if (power_system.power_list[i]._inuse)
		{
			power_update(&power_system.power_list[i]);
		}
	}
}

void power_update(Power* ent)
{
	if (!ent) return;

	if (ent->update) ent->update(ent);
}




void power_system_move_all()
{
	int i;
	for (i = 0; i < power_system.power_max; i++)
	{
		if (power_system.power_list[i]._inuse)
		{
			power_move(&power_system.power_list[i]);
		}
	}
}


void power_move(Power* self)
{
	if (!self) return;

	if (self->move) self->move(self);



	//gfc_vector3d_add(self->position, self->position, self->velocity);

	//(self->position, self->position, self->velocity);



}



void power_check_collisions()//, World* world);
{
	Power* a, * b;
	int i, c;
	Uint8 collision;
	for (i = 0; i < power_system.power_max; i++)
	{
		a = &power_system.power_list[i];
		if (!a->_inuse) continue;
		//if (a->collidedType == CT_None) continue;

		//a->stopped = 0;

		for (c = 0; c < power_system.power_max; c++)
		{

			b = &power_system.power_list[c];
			if (!b->_inuse) continue;
			//if (b->collidedType == CT_None) continue;

			collision = collision_test(a, b);
			if (collision)
			{
			//	a->stopped = 1;
				break;

				//slog("Collision: %d, (%s <-> %s)", collision_test(a, b), a->name, b->name);


			}
		}

	}
}

Power* power_list_get()
{

	if (!power_system.power_list)
	{
		slog("Cant get power List");
		return NULL;
	}
	return power_system.power_list;

}

Uint32 power_list_get_max()
{
	if (!power_system.power_max)
	{
		slog("Cant get power list max");
		return 0;
	}
	return power_system.power_max;
}



//void power_collide(Power* ent, Power* other)
//{
//	if ((!ent) || (!other)) return;
//
//	if (ent->collide) ent->collide(ent, other);
//}


Uint8 power_hitscan(GFC_Vector3D start, GFC_Vector3D end)
{

}

Power* power_spawn(GFC_Vector3D position, GFC_Color color)
{
	Power* self = NULL;
	Particle* particle = NULL;

	self = power_new();
	particle = gf3d_particle_load2(position);

	if (!self) return NULL;
	if (!particle)
	{
		slog("gf3d_particle_load2: FAILED to allocate particle!");
		return NULL;
	}
	self->acceleration = gfc_vector3d(0, 0, 0);
	self->velocity = gfc_vector3d(0, 0, 0);

	self->rotation = gfc_vector3d(0, 0, 0);
	gfc_line_cpy(self->name, "powers");
	self->particle = particle;
	self->color = color;
	self->position = position;
	self->drawOffset = gfc_vector3d(0, 0, 6);
	//self->move = power_move;
	//self->think = power_think;
	//self->update = power_update;
	self->TTL = 0;

	self->velocity = gfc_vector3d(0, 0, 0);
	return self;
}