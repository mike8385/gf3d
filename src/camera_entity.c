#include "simple_logger.h"

#include "gfc_input.h"
#include "gf3d_camera.h"

#include "player.h"

#include "camera_entity.h"


typedef struct
{
	//GFC_Vector3D target;
	Entity* target;
	float followHeight;
	float angle;
	float followDistance;

}CameraEntityData;



void camera_entity_free(Entity* self)
{
	CameraEntityData* data;
	if ((!self) || (!self->data)) return;
	data = self->data;
	free(data);
}

void camera_entity_think(Entity* self) {
	GFC_Vector3D d = { 0 }, offset;
	CameraEntityData* data;
	float look = 1;
	float turn = 0.1;

	if ((!self) || (!self->data)) return;
	data = self->data;
	if (!data->target) return;

	/*
	From what I understand this does, It makes a vector, and positions it in front
	Of player, then gets an offset. That offset; using the math below then orients the
	Camera a certian distance away and position, and the data->angle is the angle of the camera
	itself. But since I dont move it anymore after initilizing, it never moves.
	*/
	offset = gfc_vector3d(0, 1, 0); //Initalize
	gfc_vector3d_rotate_about_z(&offset, data->angle); //Rotate vector so it faces left <----
	gfc_vector3d_scale(offset, offset, data->followDistance); //Make the followDistance work/be same after player moves
	offset.z = data->followHeight; //Moves the offset z so it follows the players height after movement
	gfc_vector3d_add(self->position, offset, data->target->position);
	gf3d_camera_look_at(data->target->position, &self->position);


	//Move Camera/Model
	if (gfc_input_command_down("panright"))
	{
		//slog("Pressed Right");

		data->angle += turn;

	}
	if (gfc_input_command_down("panleft"))
	{
		//slog("Pressed Left");
		data->angle -= turn;
	}

	if (gfc_input_command_down("panup"))
	{
		//slog("Pressed UP");

		data->followHeight += look;
	}
	if (gfc_input_command_down("pandown"))
	{
		data->followHeight -= look;
	}

}



Entity* camera_entity_spawn(GFC_Vector3D position, Entity* target)
{
	GFC_Vector3D dir;
	CameraEntityData* data = NULL;
	Entity* self = NULL;
	if (!target)
	{
		slog("No camera target");
		return;
	}
	self = entity_new();
	if (!self)
	{
		slog("Failed to create/allocate space for new camera entity");
		return NULL;
	}
	data = gfc_allocate_array(sizeof(CameraEntityData), 1);
	if (!data)
	{
		slog("NO CAMERA ENTITY DATA");
		entity_free(self);
		return NULL;
	}
	self->collidedType = CT_None;
	data->followHeight = 50;
	data->followDistance = 100;
	data->angle = GFC_PI;
	self->data = data;
	self->position = position;
	self->think = camera_entity_think;
	self->free = camera_entity_free;
	gfc_vector3d_sub(dir, target->position, position);	//Gets distance from 2 objects
	gfc_vector3d_normalize(&dir);	//Sets vector to 1, so it knows where to point

	//gfc_vector3d_add(target->position, position, dir);
	data->target = target;

	gf3d_camera_look_at(data->target->position, &self->position);
	slog("Sucessfully returned and created camera entity");
	player_set_camera_ent(data->target, self);
	return self;
}
