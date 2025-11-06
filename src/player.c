#include "simple_logger.h"

#include "player.h"

#include "gfc_input.h"

typedef struct
{
	Entity*		cam;
	Uint8		canFloat;
}PlayerEntityData;

Entity* player_spawn(GFC_Vector3D position, GFC_Color color)
{
	Entity* self;
	self = entity_new();
	PlayerEntityData* data;
	if (!self) return NULL;
	data = gfc_allocate_array(sizeof(PlayerEntityData), 1);
	self->data = data;
	gfc_line_cpy(self->name, "notAugmon");
	self->mesh = gf3d_mesh_load_obj("models/dino/dino.obj");
	self->texture = gf3d_texture_load("models/dino/dino.png");
	self->color = color;
	self->position = position;
	self->bounds = gfc_box(self->position.x -7, self->position.y + 7, self->position.z, 14, 14, 14);
	self->drawOffset = gfc_vector3d(0, 0, 6);
	self->drawShadow = 1;
	self->think = player_think;
	self->free = player_free;
	self->update = player_update;
	//Sself->rotation.z = 90;
	self->velocity = gfc_vector3d(0,0,0);
	//self->velocity.z = gfc_crandom();
	self->doGenericUpdate = 1;
	entity_get_floor_pos(self, world_get_the(), &self->position);
	self->justSpawned = 1;
	self->canJump = 1;
	self->onGround = 1;
	self->collidedType = CT_Player;
	//self->collide = player_collide;
	data->canFloat = 0;
	return self;
}
//7 side, 7 down, 2 up

void player_free(Entity* self)
{
	if (!self) return;

}


void player_set_camera_ent(Entity* self, Entity* cam)
{
	PlayerEntityData* data;
	if ((!self) || (!cam)) return;
	data = self->data;
	data->cam = cam;
}



void player_think(Entity* self)
{
	if (!self) return;
	player_move(self); 
	//slog("Player Bounds: %f,%f,%f,%f,%f,%f", self->bounds.x, self->bounds.y, self->bounds.z, self->bounds.w, self->bounds.h, self->bounds.d);
}


void player_update(Entity* self)
{
	GFC_Vector3D floorPos;
	PlayerEntityData* data;
	if ((!self) || (!self->data)) return;
	data = self->data;
	entity_get_floor_pos(self, world_get_the(), &floorPos);
	self->bounds = gfc_box(self->position.x - 7, self->position.y + 7, self->position.z, 14, 14, 14);

	if (self->justSpawned)
	{
		self->position.z = floorPos.z;
		self->justSpawned = 0;

	}

	//If the player is at floor or below, they are supposed to be on ground, set to ground
	//And set onground to 1
	if (self->position.z <= floorPos.z)
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
	GFC_Vector3D cameraDir;
	PlayerEntityData* data;
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
			self->velocity.z = 10;
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