#include "simple_logger.h"


#include "body.h"


Body* body_new()
{
	Body* b;
	b = gfc_allocate_array(sizeof(Body), 1);
	if (!b) return NULL;

	b->volumes = gfc_list_new();

	return b;
}

Body* body_free(Body* b)
{
	if (!b) return;
	if (b->volumes)
	{
		gfc_list_foreach(b->volumes, free);
		gfc_list_delete(b->volumes);
	}
	free(b);
}

void body_add_volume(Body* b, GFC_Primitive v)
{
	GFC_Primitive* p;
	if (!b) return;
	p = gfc_allocate_array(sizeof(GFC_Primitive), 1);
	memcpy(p, &v, sizeof(GFC_Primitive));
	gfc_list_append(b->volumes, p);
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
	b->stepPosition = b->position;
	gfc_vector3d_scale(b->stepVelocity, b->velocity, factor);
	b->stopped = 0;
}

int body_test_body(Body* a, Body* b)
{
	//In compare, manually compare each shape with eachother (Like a switch)
	int i, c;
	int j, d;
	GFC_Primitive* ap, * bp;
	GFC_Primitive apTest, bpTest;
	if ((!a) || (!b)) return 0;
	//for each primitive in a, test each primitive in b
	c = gfc_list_count(a->volumes);
	for (i = 0; i < c; i++)
	{
		ap = gfc_list_nth(a->volumes, i);
		apTest = gfc_primitive_offset(*ap,a->stepPosition);
		d = gfc_list_count(b->volumes);
		for (j = 0; j < d; j++)
		{
			bp = gfc_list_nth(b->volumes, j);
			bpTest = gfc_primitive_offset(*bp, b->stepPosition);
			//TODO test if the two primitives, now in world spaec collide
			//if so, call the callbacks on A AND B
			//If so return 1
			
		}
	}
	return 0;
}
