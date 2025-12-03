#include "simple_logger.h"

#include "collision.h"
#include "gfc_shape.h"

Uint8 collision_test(Entity* self, Entity* other)
{
	int i, c;
	int j, d;
	GFC_Box aBox;
	GFC_Box bBox;
	GFC_Vector3D aOffset;
	GFC_Vector3D bOffset;
	if ((!self) || (!other)) return 0;
	if (self == other) return 0;

	if (self->bounds.w <= 0 || self->bounds.h <= 0 || self->bounds.d <= 0) return 0;
	if (other->bounds.w <= 0 || other->bounds.h <= 0 || other->bounds.d <= 0) return 0;


	//Get bounds for each entity
	aBox = self->bounds;
	gfc_vector3d_add(self->newPos, self->position, self->velocity);

	bBox = other->bounds;
	gfc_vector3d_add(other->newPos, other->position, other->velocity);


	//Reason we do this math is because the way we calculate step posiiton, allows the movement changes to work
	aOffset = gfc_vector3d(self->newPos.x - self->position.x,
		self->newPos.y - self->position.y,
		self->newPos.z - self->position.z);
	aBox.x += aOffset.x; aBox.y += aOffset.y; aBox.z += aOffset.z;

	bOffset = gfc_vector3d(other->newPos.x - other->position.x,
		other->newPos.y - other->position.y,
		other->newPos.z - other->position.z);
	bBox.x += bOffset.x; bBox.y += bOffset.y; bBox.z += bOffset.z;

	if (gfc_box_overlap(aBox, other->bounds))
	{
		//slog("Collisiion");
		return 1;
	}
	else
	{
		//self->stopped = 0;
		return 0;
	}
}


