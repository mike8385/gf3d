#include "simple_logger.h"

#include "electric.h"
#include "SDL_timer.h"






Power* electric_spawn(GFC_Vector3D position, GFC_Color color)
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
	self->move = electric_move;
	self->think = electric_think;
	self->update = electric_update;
	self->spawnPos = self->position;
	self->powerType = PT_NONE;
	self->spawnTime = SDL_GetTicks();
	self->TTL = 500;



	self->velocity = gfc_vector3d(0, 0, 0);
	return self;
}

void electric_free(Power* self)
{
	if (!self) return;


}




void electric_think(Power* self)
{
	if (!self) return;

}


void electric_update(Power* self)
{
	if (!self) return;

}

void electric_move(Power* self)
{
	if (!self) return;
	GFC_Vector3D diff;
	gfc_vector3d_sub(diff, self->position, self->spawnPos);
	if (self->powerType == PT_DISTANCE)
	{
		if (gfc_vector3d_magnitude(diff) > 200)
		{
			power_free(self);
		}
	}

	if (self->powerType == PT_TIMER)
	{
		float curr = SDL_GetTicks();
		if (curr - self->spawnTime > self->TTL)
		{
			power_free(self);
		}
	}


	gfc_vector3d_add(self->position, self->position, self->velocity);



}

void electric_collide(Power* self, Power* other)
{
	if (!self) return;
	if (!other) return;
	
}


//Make a new function called electric abilities that is just attack, except its only abilities and not attacking
//I have dashing, teleporting, floating, "parkour"and might need 2 more

void electric_attack(Power* self)
{
	if (!self) return;

}


