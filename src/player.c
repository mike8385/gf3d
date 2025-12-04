#include "simple_logger.h"

#include "player.h"

#include "space.h"

#include "gfc_input.h"
#include "gf3d_obj_load.h"
#include "gfc_primitives.h"

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
	self->mesh = gf3d_mesh_load_obj("models/dino/dino.obj");
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

	//Extra Info
	self->bounds = gfc_box(self->position.x - 7, self->position.y - 7, self->position.z, 14, 14, 14);
	self->floorBounds = gfc_box(self->position.x - 7, self->position.y - 7, self->position.z - 0.5, 14, 1, 14);
	self->drawOffset = gfc_vector3d(0, 0, 6);
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



	if (data->canFloat == 1)
	{
		if (gfc_input_command_down("jump"))
		{
			self->acceleration.z = -.1;

		}
		else
		{
			self->acceleration.z = -.5;
		}
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

	if (gfc_input_command_down("run"))
	{
		step = 0.9;
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


void player_attack(Entity* self)
{
	int mx, my;

	PlayerEntityData* data;
	GFC_Vector3D end, start;
	GFC_Vector3D forward;
	GFC_Vector3D contact;
	CollidedType type = CT_None;
	Entity* hit = NULL;


	if ((!self) || (!self->data)) return;

	data = self->data;

	if (self->energy > 0)
	{
		if (SDL_GetMouseState(&mx, &my) == SDL_BUTTON(1))
		{

			start = self->position; //Players position
			forward = self->forward; //Players forward direction
			forward.z = 0;

			gfc_vector3d_normalize(&forward); //Now its a direction arrow, where to go like position wise, points at player
			//slog("%f, %f, %f", forward.x, forward.y, forward.z);

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

			self->energy -= 3;

		}
	}
	else
	{
		self->energy = 0;
	}

}

void player_power_drain(Entity* self)
{
	if (!self) return;
	int i, j, k;
	GFC_Sphere drainRadius;
	Entity* entityList = entity_list_get();
	Uint32 maxEnt = entity_list_get_max();

	drainRadius = gfc_sphere(self->position.x, self->position.y, self->position.z, 10);


	for (i = 0; i < maxEnt; i++)
	{
		if (&entityList[i] == self) continue;
		if (!&entityList[i].powered) continue;
		
		//if (gfc_edge3d_to_sphere_intersection())
	}
}


Entity* player_get_player()
{
	if (!player_system.playerData) return;

	return player_system.playerData;
}


