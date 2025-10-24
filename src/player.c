#include "simple_logger.h"

#include "player.h"

#include "gfc_input.h"


typedef struct
{
	Entity* cam;
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
	self->drawOffset = gfc_vector3d(0, 0, 6);
	self->drawShadow = 1;
	self->think = player_think;
	self->free = player_free;
	//Sself->rotation.z = 90;
	self->velocity.x = 0;
	//self->velocity.z = gfc_crandom();
	self->doGenericUpdate = 1;
	slog("Made it here");
	return self;
}

void player_free(Entity* self)
{

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

}

void player_update(Entity* self)
{
	GFC_Vector3D position = { 0 };
	PlayerEntityData* data;
	if ((!self) || (!self->data)) return;
	data = self->data;
	//entity_get_floor_pos(self, world)
	self->position.z = position.z;
}

void player_move(Entity* self)
{
	float move = 0;
	float step = 0.5;
	GFC_Vector3D cameraDir;
	PlayerEntityData* data;
	if ((!self) || (!self->data)) return;
	data = self->data;
	if (!data->cam) return;
	gfc_vector3d_sub(cameraDir, self->position, data->cam->position); //Gets vector from camera to player
	gfc_vector3d_normalize(&cameraDir); //Now its a direction arrow, where to go like position wise, points at player
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






	////Movement left/right
	//if (gfc_input_command_down("walkleft"))
	//{
	//	//slog("Pressed a");
	//	self->velocity.x = -3;
	//}
	//else if (gfc_input_command_down("walkright"))
	//{
	//	//slog("Pressed d");
	//	self->velocity.x = 3;
	//}
	//else
	//{
	//	self->velocity.x = 0;
	//}

	if ((self->velocity.x) || (self->velocity.y))
	{
		self->rotation.z = gfc_vector2d_angle(gfc_vector3dxy(self->velocity));
	}

}