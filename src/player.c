#include <string.h>

#include "simple_logger.h"

#include "player.h"

#include "space.h"

#include "gfc_input.h"
#include "gf3d_obj_load.h"
#include "gfc_config.h"
#include "gfc_primitives.h"
#include "electric.h"

typedef struct
{
	Entity*		cam;

	Uint32		shards;
	Uint8		canFloat;
	Uint8		canShoot;
	Uint8		hasMissle;


	//Uint32		health;
	//Uint32		energy;
	//Uint32		maxHealth;
	//Uint32		maxEnergy;



}PlayerEntityData;

typedef struct
{
	Entity* playerData;
}PlayerSystem;



static PlayerSystem player_system = { 0 }; /**<Initalize a LOCAL global entity manager*/

Entity* player_spawn_json(const char* filename)
{

	Entity* self = NULL;
	PlayerEntityData* data = NULL;
	const char* str = NULL;
	SJson* json = NULL, * config = NULL;
	GFC_Color color = { 0 };
	GFC_Vector3D position = { 0 };
	GFC_Vector3D drawOffset = { 0 };
	Uint8 f, m;
	Uint32 s;

	json = sj_load(filename);
	if (!json)
	{
		slog("Couldnt find player filename % s", filename);
		return NULL;
	}
	self = entity_new();
	if (!self)
	{
		slog("Failed to allocate a world for %s", filename);
		sj_free(json);
		return NULL;
	}

	config = sj_object_get_value(json, "player");
	if (!config)
	{
		slog("Failed to parse a player");
		sj_free(json);
		entity_free(self);
		return NULL;
	}

	str = sj_object_get_string(config, "name");
	if (!str) gfc_line_cpy(str, "NoPlayerName");
	gfc_line_cpy(self->name, str);

	str = sj_object_get_string(config, "mesh");
	if (!str)
	{
		slog("Failed to get string mesh");
		sj_free(json);
		entity_free(self);
		sj_free(str);
		return NULL;
	}
	self->mesh = gf3d_mesh_load_obj(str);

	self->texture = gf3d_texture_load(sj_object_get_string(config, "texture"));
	if (!self->texture)
	{
		slog("No player texture added, assigning default");
	}
	
	color = sj_object_get_color(config, "color");
	self->color = color;
	if (!&self->color) self->color = gfc_color(255, 255, 255, 255);

	sj_object_get_vector3d(config, "position", &position);
	self->position = position;
	self->newPos = gfc_vector3d(self->position.x,
		self->position.y,
		self->position.z);

	self->acceleration = gfc_vector3d(0, 0, 0);
	self->rotation = gfc_vector3d(0, 0, 0);
	self->velocity = gfc_vector3d(0, 0, 0);

	//Health & Energy Info

	sj_object_get_float(config, "maxHealth", &self->maxHealth);
	sj_object_get_float(config, "maxEnergy", &self->maxEnergy);
	self->health = self->maxHealth;
	self->energy = self->maxEnergy;

	//Bounds:
	self->bounds = gfc_box(self->position.x - 7, self->position.y - 7, self->position.z, 14, 14, 14);
	self->floorBounds = gfc_box(self->position.x - 7, self->position.y - 7, self->position.z - 0.5, 14, 1, 14);
	sj_object_get_vector3d(config, "drawOffset", &drawOffset);
	self->drawOffset = drawOffset;

	sj_object_get_uint8(config, "drawShadow", &self->drawShadow);


	//Data Info
	data = gfc_allocate_array(sizeof(PlayerEntityData), 1);
	self->data = data;

	sj_object_get_uint8(config, "canFloat", &f);
	sj_object_get_uint8(config, "hasMissle", &m);
	sj_object_get_uint32(config, "shards", &s);

	data->canFloat = f;
	data->hasMissle = m;
	data->shards = s;

	str = sj_object_get_string(config, "collidedType");
	self->collidedType = string_to_collision_type(str);

	//Extra Info
	self->bounds = gfc_box(self->position.x - 7, self->position.y - 7, self->position.z, 14, 14, 14);
	self->floorBounds = gfc_box(self->position.x - 7, self->position.y - 7, self->position.z - 0.5, 14, 1, 14);
	//self->drawOffset = gfc_vector3d(0, 0, 6);
	self->think = player_think;
	self->free = player_free;
	self->update = player_update;

	entity_get_floor_pos(self, world_get_the(), &self->position);

	self->doGenericUpdate = 1;
	self->justSpawned = 1;
	self->canJump = 1;
	self->onGround = 1;
	//self->collidedType = CT_Player;
	self->stopped = 0;
	self->forward = gfc_vector3d(0, 1, 0);
	self->newPos = gfc_vector3d(self->position.x, self->position.y, self->position.z);

	//Animation?


	SJson* animationArray = sj_object_get_value(json, "animation");

	self->meshList = NULL;
	self->totalFrames = 0;
	self->currentFrame = 0;

	//if (animationArray)
	//{
	//	int count = sj_array_count(animationArray);

	//	self->totalFrames = count;

	//	self->meshList = gfc_list_new();


	//	for (int i = 0; i < 5; i++)
	//	{
	//		SJson* animation = sj_array_get_nth(animationArray, i);
	//		int frame = 0;
	//		sj_object_get_int(animation, "frame", &frame);
	//		str = sj_object_get_string(animation, "filename");
	//		slog("Frame: %d, Filename: %s", frame, str);
	//		Mesh* frameMesh = gf3d_mesh_load_obj(str);
	//		gfc_list_append(self->meshList, frameMesh);

	//	}
	//}


	//self->lastAnimateTime = SDL_GetTicks();

	player_system.playerData = self;

	sj_free(json);

	return self;
}


Entity* player_spawn(GFC_Vector3D position, GFC_Color color)
{
	Space* space = NULL;
	Entity* self = NULL;
	Body* body = NULL;
	PlayerEntityData* data = NULL;
	self = entity_new();
	if (!self) return NULL;
	//Basic Important Stuff
	gfc_line_cpy(self->name, "notAugmon");
	self->mesh = gf3d_mesh_load_obj("models/dino/dino_skin.obj");
	self->texture = gf3d_texture_load("models/dino/dino.png");
	self->color = color;
	self->position = position;
	self->newPos = gfc_vector3d(self->position.x,
		self->position.y,
		self->position.z);

	self->acceleration = gfc_vector3d(0, 0, 0);
	self->rotation = gfc_vector3d(0, 0, 0);
	self->velocity = gfc_vector3d(0, 0, 0);


	//Data Info
	data = gfc_allocate_array(sizeof(PlayerEntityData), 1);
	self->data = data;
	data->canFloat = 0;
	data->hasMissle = 0;
	data->shards = 0;


	//Health & Energy Info
	self->maxHealth = 100;
	self->maxEnergy = 100;
	self->health = self->maxHealth;
	self->energy = self->maxEnergy;

	self->drawShadow = 1;
	self->think = player_think;
	self->free = player_free; 
	self->update = player_update;

	entity_get_floor_pos(self, world_get_the(), &self->position);

	self->doGenericUpdate = 1;
	self->justSpawned = 1;
	self->canJump = 1;
	self->onGround = 1;
	self->collidedType = CT_Player;
	self->stopped = 0;
	self->forward = gfc_vector3d(0, 1, 0);
	self->newPos = gfc_vector3d(self->position.x, self->position.y, self->position.z);




	player_system.playerData = self;

	return self;
}
//7 side, 7 down, 2 up

void player_free(Entity* self)
{
	if (!self) return;

	if (self->data)
	{
		free(self->data);
		self->data = NULL;
	}

	//if (self->texture)
	//{
	//	gf3d_texture_free(self->texture);
	//	self->texture = NULL;
	//}

	//if (self->mesh)
	//{
	//	gf3d_mesh_free(self->mesh);
	//	self->mesh = NULL;
	//}

	if (player_system.playerData == self)
	{
		player_system.playerData = NULL;
	}

	//free(self);
}


void player_set_camera_ent(Entity* self, Entity* cam)
{
	PlayerEntityData* data = NULL;
	if ((!self) || (!cam)) return;
	data = self->data;
	data->cam = cam;
}



void player_think(Entity* self)
{
	Body* body = NULL;
	Space* space = NULL;
	if (!self) return;
	player_move(self);
	
	//Space Stuff
	space = space_get_the();
	if (!space)
	{
		slog("No space available");
		return;
	}
	
	//Body Stuff
	//body_reset_for_updates(body, space->step);
	player_attack(self);
	
	if (self->energy > self->maxEnergy)
	{
		self->energy = self->maxEnergy;
	}
	//self->currentFrame++;
	//player_power_drain(self);
	//slog("Player Bounds: %f,%f,%f,%f,%f,%f", self->bounds.x, self->bounds.y, self->bounds.z, self->bounds.w, self->bounds.h, self->bounds.d);
}


void player_update(Entity* self)
{
	GFC_Vector3D floorPos = { 0 };
	PlayerEntityData* data = NULL;
	if ((!self) || (!self->data)) return;
	data = self->data;
	entity_get_floor_pos(self, world_get_the(), &floorPos);


	self->bounds = gfc_box(self->position.x - 7, self->position.y + 7, self->position.z, 14, 14, 14);
	self->floorBounds = gfc_box(self->position.x - 7, self->position.y - 7, self->position.z - 0.5, 14, 1, 14);

	if (self->justSpawned)
	{
		self->position.z = floorPos.z;
		self->justSpawned = 0;

	}

	//If the player is at floor or below, they are supposed to be on ground, set to ground
	//And set onground to 1
	if (self->floorBounds.z <= floorPos.z)
	{
		self->onGround = 1;
		self->position.z = floorPos.z;
		self->canJump = 1;
		self->velocity.z = 0;
		self->acceleration.z = 0;
		data->canFloat = 0;
	}



	if (data->canFloat == 1 && gfc_input_command_down("jump") && data->shards > 0)
	{
			self->acceleration.z = -.1;
			Power* power = NULL;
			power = electric_spawn_small(gfc_vector3d(self->position.x + 5, self->position.y, self->position.z - 2), GFC_COLOR_BLUE);
			power->powerType = PT_TIMER;

			Power* power2 = NULL;
			power2 = electric_spawn_small(gfc_vector3d(self->position.x - 5, self->position.y, self->position.z - 2), GFC_COLOR_BLUE);
			power2->powerType = PT_TIMER;

		

	}
	else
	{
		self->acceleration.z = -.5;
	}



	////This helps with uneven terrain
	////if (fabs(self->position.z <= position.z) > 1.0f)
	//if (self->position.z <= position.z)
	//{
	//	self->onGround = 0;
	//}
	//else
	//{
	//	self->canJump = 1;
	//	self->onGround = 1;
	//	self->acceleration.z = 0;
	//	self->position.z = position.z;// +5;

	//}

if (self->meshList && self->totalFrames > 0)
{
    Uint32 now = SDL_GetTicks();
    if (now - self->lastAnimateTime > 100)
    {
        self->currentFrame++;
        self->lastAnimateTime = now;
    }

    if (self->currentFrame >= self->totalFrames)
    {
        self->currentFrame = 0;
    }

    self->mesh = gfc_list_get_nth(self->meshList, self->currentFrame);
}


}

void player_move(Entity* self)
{
	float move = 0;
	float step;
	float dash = 2;
	GFC_Vector3D cameraDir;
	PlayerEntityData* data = NULL;
	if ((!self) || (!self->data)) return;
	data = self->data;
	if (!data->cam) return;
	gfc_vector3d_sub(cameraDir, self->position, data->cam->position); //Gets vector from camera to player
	gfc_vector3d_normalize(&cameraDir); //Now its a direction arrow, where to go like position wise, points at player

	if ((gfc_input_command_down("run") && self->energy > 0) && data->shards > 2)
	{
		Power* power = NULL;
		power = electric_spawn(self->position, GFC_COLOR_BLUE);
		power->powerType = PT_TIMER;
		step = 3;
		self->energy -= 5;

	}
	else
	{
		step = 0.5;		
	}



	//Movement Up/Down
	if (gfc_input_command_down("walkforward"))
	{
		//slog("Pressed w");
		move += step;
		//if (gfc_input_command_down("dash"))
		//{
		//	self->position.x =  self->position.x + dash;
		//}
	}
	if (gfc_input_command_down("walkback"))
	{
		//slog("Pressed s");
		move -= step;
	}

	if (move) //if move happened
	{
		gfc_vector2d_scale(cameraDir, cameraDir, move); //Scale the direction, so it orients to player?
		gfc_vector2d_add(self->velocity, self->velocity, cameraDir); //Add how much the player 'moved' and the current velocity at 
																	//which they moved and set it to velocity new
		self->forward = cameraDir;
	}

	move = 0;
	gfc_vector3d_sub(cameraDir, self->position, data->cam->position); //Gets vector from camera to player
	gfc_vector3d_normalize(&cameraDir); //Now its a direction arrow, where to go like position wise, points at player
	gfc_vector3d_rotate_about_z(&cameraDir, GFC_HALF_PI); //Now tracks left/right
	//Movement Up/Down
	if (gfc_input_command_down("walkright"))
	{
		//slog("Pressed w");
		move -= step;
	}
	if (gfc_input_command_down("walkleft"))
	{
		//slog("Pressed s");
		move += step;
	}

	if (move) //if move happened
	{
		gfc_vector2d_scale(cameraDir, cameraDir, move); //Scale the direction, so it orients to player?
		gfc_vector2d_add(self->velocity, self->velocity, cameraDir); //Add how much the player 'moved' and the current position at 
																		//which they moved and set it to position new
		self->forward = cameraDir;


	}

	if ((self->velocity.x) || (self->velocity.y))
	{
		self->rotation.z = gfc_vector2d_angle(gfc_vector3dxy(self->velocity));
	}

	if (gfc_input_command_down("jump") && (self->canJump == 1))
	{
		if (self->onGround == 1)
		{
			//slog("CanJump: %f", self->canJump);
			self->canJump = 0;
			self->onGround = 0;
			self->velocity.z = 15;
			self->acceleration.z = -.5;
			//Make accel .1 when i glide
			data->canFloat = 1;
		}
		

	}



}

void player_collide(Entity* self, Entity* other)
{

	Uint8 pType, oType;
	if ((!self) || (!other)) return;
	if ((!self->collidedType) || (!other->collidedType)) return;
	pType = self->collidedType;
	oType = other->collidedType;
	if (other->collidedType != CT_Player)
	{
		/*self->position = 0;*/
	}

}


//Make a new function called player abilities that is just attack, except its only abilities and not attacking
//I have dashing, teleporting, floating, "parkour"and might need 2 more

void player_attack(Entity* self)
{
	int mx, my;

	PlayerEntityData* data;
	GFC_Vector3D end, start;
	GFC_Vector3D forward;
	GFC_Vector3D vertical;
	GFC_Vector3D contact;
	CollidedType type = CT_None;
	Entity* hit = NULL;


	if ((!self) || (!self->data)) return;

	data = self->data;

	if (self->energy > 0)
	{
		if (SDL_GetMouseState(&mx, &my) == SDL_BUTTON(1))
		{
			Power* power;
			float speed = 5.0;

			start = self->position; //Players position
			forward = self->forward; //Players forward direction
			forward.z = 0;

			gfc_vector3d_normalize(&forward); //Now its a direction arrow, where to go like position wise, points at player
			//slog("%f, %f, %f", forward.x, forward.y, forward.z);

			power = electric_spawn(gfc_vector3d(self->position.x, self->position.y, self->position.z + 5), GFC_COLOR_BLUE);
			power->powerType = PT_DISTANCE;
			if (!power) return;
			gfc_vector3d_scale(power->velocity, forward, speed);

			//power->velocity = forward;


			/*end = start + forward * 50*/
			gfc_vector3d_scale(forward, forward, 50); //forward = forward * 50
			gfc_vector3d_add(end, start, forward);
			//slog("Ray start: %f %f %f", start.x, start.y, start.z);
			//slog("Ray end:   %f %f %f", end.x, end.y, end.z);
			hit = entity_hitscan(self, start, end, &type);
			if (hit)
			{
				if (type == CT_Player) return;
				if (type != CT_Monster) return;
				slog("Hit monster");
				entity_free(hit);
			}
			//self->position = end; <-- This lets me teleport

			self->energy -= 3;

		}

		if (SDL_GetMouseState(&mx, &my) == SDL_BUTTON(3) && data->shards > 1)
		{
			Power* power;
			float speed = 3.0;

			start = self->position; //Players position
			forward = self->forward; //Players forward direction
			forward.z = 0;

			gfc_vector3d_normalize(&forward); //Now its a direction arrow, where to go like position wise, points at player
			//slog("%f, %f, %f", forward.x, forward.y, forward.z);

			power = electric_spawn_big(gfc_vector3d(self->position.x, self->position.y, self->position.z + 5), GFC_COLOR_BLUE);
			power->powerType = PT_DISTANCE;
			if (!power) return;
			gfc_vector3d_scale(power->velocity, forward, speed);

			/*end = start + forward * 50*/
			gfc_vector3d_scale(forward, forward, 250); //forward = forward * 50
			gfc_vector3d_add(end, start, forward);
			//slog("Ray start: %f %f %f", start.x, start.y, start.z);
			//slog("Ray end:   %f %f %f", end.x, end.y, end.z);
			hit = entity_hitscan(self, start, end, &type);
			if (hit)
			{
				if (type == CT_Player) return;
				if (type != CT_Monster) return;
				slog("Hit monster");
				entity_free(hit);
			}

			self->energy -= 10;

		}

		if (gfc_input_command_down("tele") && data->shards > 3) 
		{
			Power* power;
			Power* power2;
			Power* power3;
			float speed = 1.5;

			start = self->position; //Players position
			forward = self->forward; //Players forward direction
			forward.z = 0;


			gfc_vector3d_normalize(&forward); //Now its a direction arrow, where to go like position wise, points at player
			//slog("%f, %f, %f", forward.x, forward.y, forward.z);

			vertical = gfc_vector3d(0, 0, 10);
			gfc_vector3d_normalize(&vertical);

			power = electric_spawn(gfc_vector3d(self->position.x, self->position.y, self->position.z + 10), GFC_COLOR_BLUE);
			power->powerType = PT_TIMER;
			if (!power) return;
			gfc_vector3d_scale(power->velocity, vertical, speed);
			power2 = electric_spawn(gfc_vector3d(self->position.x, self->position.y, self->position.z + 5), GFC_COLOR_BLUE);
			power2->powerType = PT_TIMER;
			if (!power2) return;
			gfc_vector3d_scale(power2->velocity, vertical, speed);
			power3 = electric_spawn(gfc_vector3d(self->position.x, self->position.y, self->position.z + 10), GFC_COLOR_BLUE);
			power3->powerType = PT_TIMER;
			if (!power3) return;
			gfc_vector3d_scale(power3->velocity, vertical, speed);

			//power->velocity = forward;


			/*end = start + forward * 50*/
			gfc_vector3d_scale(forward, forward, 50); //forward = forward * 50
			gfc_vector3d_add(end, start, forward);

			self->position = end; //<-- This lets me teleport
			power = electric_spawn(gfc_vector3d(self->position.x, self->position.y, self->position.z + 10), GFC_COLOR_BLUE);
			power->powerType = PT_TIMER;
			if (!power) return;
			gfc_vector3d_scale(power->velocity, -vertical, speed);
			power2 = electric_spawn(gfc_vector3d(self->position.x, self->position.y, self->position.z + 5), GFC_COLOR_BLUE);
			power2->powerType = PT_TIMER;
			if (!power2) return;
			gfc_vector3d_scale(power2->velocity, -vertical, speed);
			power3 = electric_spawn(gfc_vector3d(self->position.x, self->position.y, self->position.z + 10), GFC_COLOR_BLUE);
			power3->powerType = PT_TIMER;
			if (!power3) return;
			gfc_vector3d_scale(power3->velocity, -vertical, speed);

			self->energy -= 25;

		}


	}
	else
	{
		self->energy = 0;

		if (SDL_GetMouseState(&mx, &my) == SDL_BUTTON(1))
		{

			start = self->position; //Players position
			forward = self->forward; //Players forward direction
			forward.z = 0;

			gfc_vector3d_normalize(&forward); //Now its a direction arrow, where to go like position wise, points at player
			//slog("%f, %f, %f", forward.x, forward.y, forward.z);

			/*end = start + forward * 50*/
			gfc_vector3d_scale(forward, forward, 2); //forward = forward * 50
			gfc_vector3d_add(end, start, forward);
			//slog("Ray start: %f %f %f", start.x, start.y, start.z);
			//slog("Ray end:   %f %f %f", end.x, end.y, end.z);
			hit = entity_hitscan(self, start, end, &type);
			if (hit)
			{
				if (type == CT_Player) return;
				if (type != CT_Monster) return;
				slog("Hit monster");
				entity_free(hit);
			}



		}

	}

}

void player_power_drain(Entity* self)
{
	if (!self) return;
	int i, j, k;
	GFC_Sphere drainRadius, drainRadiusOther;
	Entity* entityList = entity_list_get();
	Uint32 maxEnt = entity_list_get_max();

	drainRadius = gfc_sphere(self->position.x, self->position.y, self->position.z, 10);


	for (i = 0; i < maxEnt; i++)
	{
		if (&entityList[i] == self) continue;
		if (!&entityList[i].powered) continue;
		//drainRadiusOther = gfc_sphere(entityList[i].position.x, entityList[i].position.y, entityList[i].position.z, 10);
		//if (gfc_sphere_overlap(drainRadius, drainRadiusOther))
		//{
		//	//slog("Overlap");
		//}
		//if (gfc_edge3d_to_sphere_intersection())
	}
}


Entity* player_get_player()
{
	if (!player_system.playerData)
	{
		slog("No player found");
		return NULL;
	}

	return player_system.playerData;
}

void player_add_shard()
{
	Entity* player = player_system.playerData;
	if (!player) return;
	
	PlayerEntityData* data;
	data = player->data;
	if (!data) return;

	data->shards++;
}

Uint32 player_get_shards()
{
	Entity* player = player_system.playerData;
	if (!player) return;

	PlayerEntityData* data;
	data = player->data;
	if (!data) return;

	return data->shards;
}

