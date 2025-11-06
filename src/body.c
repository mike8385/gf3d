#include "simple_logger.h"


#include "body.h"


Body* body_new()
{
	Body* b;
	b = gfc_allocate_array(sizeof(Body), 1);
	if (!b) return NULL;

	b->bounds = gfc_box(1000, 0, -1000, 0, 0, 0);
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

void body_add_bounds(Body* b, Entity* ent)
{
	if ((!b) || (!ent)) return;
	b->entity = ent;
	b->position = ent->position;
	b->velocity = ent->velocity;
	b->bounds = ent->bounds;
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
	if (!b) return;

	//If entity exists update values of body to current entity
	if (b->entity && b->entity->_inuse)
	{
		b->position = b->entity->position;
		b->velocity = b->entity->velocity;
		b->bounds = b->entity->bounds;
	}

	//Reset stepPosition
	b->stepPosition = b->position;
	gfc_vector3d_scale(b->stepVelocity, b->velocity, factor);
	b->stopped = 0;
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

	if (a->entity->collidedType == CT_Player)
	{
		slog("%s Bounds: %f,%f,%f", a->entity->name, a->entity->bounds.x, a->entity->bounds.y, a->entity->bounds.z);
		slog("A Bounds: %f,%f,%f", a->bounds.x, a->bounds.y, a->bounds.z);
		slog("aBox Bounds: %f,%f,%f", aBox.x, aBox.y, aBox.z);
	}





	if (gfc_box_overlap(aBox, bBox))
	{
		return 1;
	}
	else
	{
		return 0;
	}


	return 0;
}
