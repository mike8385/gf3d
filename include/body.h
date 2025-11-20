#ifndef __BODY_H__
#define __BODY_H__

#include "gfc_list.h"
#include "gfc_primitives.h"
#include "gfc_text.h"
#include "gfc_callbacks.h"



struct Entity_S;
typedef struct Entity_S Entity;



typedef struct Body_S{
	GFC_TextLine	name;
	float			mass;
	GFC_Vector3D	position; //Center of mass
	GFC_Vector3D	velocity; //How fast we are currently moving
	GFC_Box			bounds;
	//Entity*			entity;
	GFC_Callback	collide;
	void			(*onCollide)(struct Body_S* self, struct Body_S* other, void* data);  //Call this on hit
	void*			data;  //For callback function

	GFC_Vector3D	stepVelocity;
	GFC_Vector3D	stepPosition;
	GFC_Vector3D	oldPosition;
	GFC_Vector3D	boundsOffset;


	Uint8			stopped; //Stop iterating on this body, we already hit something
} Body;


typedef void body_collide_func(Body* self, Body* other, void* data);



Body* body_new();

Body* body_free(Body* b);

void body_add_data(Body* b, Entity* ent);

void body_set_collision(Body* b, body_collide_func* collide, void* data);

void body_reset_for_updates(Body* b, float factor);


int body_test_body(Body* a, Body* b);










#endif


