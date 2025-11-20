#include "simple_logger.h"


#include "body.h"
#include "entity.h"


Body* body_new()
{
	Body* b;
	b = gfc_allocate_array(sizeof(Body), 1);
	if (!b) return NULL;

	b->bounds = gfc_box(1000, 0, -1000, 0, 0, 0);
	b->stopped = 0;
	return b;
}

Body* body_free(Body* b)
{
	if (!b) return;
	//if (b->volumes)
	//{
	//	gfc_list_foreach(b->volumes, free);
	//	gfc_list_delete(b->volumes);
	//}
	free(b);
}

void body_add_data(Body* b, Entity* ent)
{
	if ((!b) || (!ent)) return;
	b->position = ent->position;
	b->velocity = ent->velocity;
	b->bounds = ent->bounds;

	b->boundsOffset = gfc_vector3d(
		b->bounds.x - b->position.x,
		b->bounds.y - b->position.y,
		b->bounds.z - b->position.z
	);

	b->stopped = 0;
}




//This is like setting think and update Im pretty sure
void body_set_collision(Body* b, body_collide_func* collide, void* data)
{
	if ((!b) || (!collide)) return;
	b->onCollide = collide;
	b->data = data;
}


void body_reset_for_updates(Body* b, float factor)
{
	//If entity has body and is inuse, and the body is the same as that entities body update the body
	Entity* ent_list;
	Entity* ent;
	Uint32 ent_max;
	int i, j;
	if (!b) return;

	//If entity exists update values of body to current entity
	ent_list = entity_list_get();
	ent_max = entity_list_get_max();
	for (i = 0; i < ent_max; i++)
	{
		if (ent_list[i]._inuse && ent_list[i].body == b)
		{
			b->position = ent_list[i].position;
			b->velocity = ent_list[i].velocity;
			b->bounds = ent_list[i].bounds;

			b->stepPosition = b->position;
			gfc_vector3d_scale(b->stepVelocity, b->velocity, factor);
			//b->stopped = 0;
			ent_list[i].bodyStopped = b->stopped;

			b->boundsOffset = gfc_vector3d(
				b->bounds.x - b->position.x,
				b->bounds.y - b->position.y,
				b->bounds.z - b->position.z
			);


		}

	}


	//Reset stepPosition
	//b->stepPosition = b->position;
	//gfc_vector3d_scale(b->stepVelocity, b->velocity, factor);
	//b->stopped = 0;
}

int body_test_body(Body* a, Body* b)
{
	//In compare, manually compare each shape with eachother (Like a switch)
	int i, c;
	int j, d;
	GFC_Box aBox;
	GFC_Box bBox;
	GFC_Vector3D aOffset;
	GFC_Vector3D bOffset;
	//GFC_Primitive* ap, * bp;
	//GFC_Primitive apTest, bpTest;
	if ((!a) || (!b)) return 0;
	if (a == b) return 0;

	if (a->bounds.w <= 0 || a->bounds.h <= 0 || a->bounds.d <= 0) return 0;
	if (b->bounds.w <= 0 || b->bounds.h <= 0 || b->bounds.d <= 0) return 0;

	//Get bounds for each entity
	aBox = a->bounds;
	bBox = b->bounds;

	//Reason we do this math is because the way we calculate step posiiton, allows the movement changes to work
	aOffset = gfc_vector3d(a->stepPosition.x - a->position.x,
							a->stepPosition.y - a->position.y,
							a->stepPosition.z - a->position.z);
	aBox.x += aOffset.x; aBox.y += aOffset.y; aBox.z += aOffset.z;

	bOffset = gfc_vector3d(b->stepPosition.x - b->position.x,
		b->stepPosition.y - b->position.y,
		b->stepPosition.z - b->position.z);
	bBox.x += bOffset.x; bBox.y += bOffset.y; bBox.z += bOffset.z;

	//As of now it only works if B is a static object
	if (gfc_box_overlap(aBox, bBox))
	{
		//slog("collision");
		return 1;
	}
	else
	{
		return 0;
	}


	return 0;
}


//void body_update(Body* body)
//{
//	if (!body) return;
//	if (body->entity->doGenericUpdate)
//}