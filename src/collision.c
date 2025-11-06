#include "simple_logger.h"

#include "collision.h"
#include "gfc_shape.h"

Uint8 collision_test(Entity* self, Entity* other)
{
	if ((!self) || (!other)) return 0;
	if (gfc_box_overlap(self->bounds, other->bounds))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


