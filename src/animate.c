//#include "simple_logger.h"
//
//#include "animate.h"
//#include "gf3d_mesh.h"
//#include "gfc_vector.h"
//#include "world.h"
//#include "collision.h"
//#include "gf3d_obj_load.h"
//
//
//typedef struct
//{
//	Animation* animate_list;
//	Uint32	animate_max;
//}AnimationSystem;
//
//static AnimationSystem animate_system = { 0 };
//
//
//CollidedType string_to_collision_type(const char* typeStr) {
//	if (!typeStr) return CT_None; // Default
//
//	if (strcmp(typeStr, "CT_Player") == 0) return CT_Player;
//	if (strcmp(typeStr, "CT_Monster") == 0) return CT_Monster;
//	if (strcmp(typeStr, "CT_Building") == 0) return CT_Building;
//	if (strcmp(typeStr, "CT_Power") == 0) return CT_Power;
//	if (strcmp(typeStr, "CT_Shard") == 0) return CT_Shard;
//	if (strcmp(typeStr, "CT_MAX") == 0) return CT_MAX;
//
//	slog("Unknown collision type: %s", typeStr);
//	return CT_None; // Default fallback
//}
//
//#define DAMPEN 0.8
//
//Animation* animate_new()
//{
//	int i;
//	if (animate_system.animate_list)
//	{
//		for (i = 0; i < animate_system.animate_max; i++)
//		{
//			if (!animate_system.animate_list[i]._inuse)
//			{
//				memset(&animate_system.animate_list[i], 0, sizeof(Animation));   //clears ALL garbage memory
//				animate_system.animate_list[i]._inuse = 1;
//				animate_system.animate_list[i].color = GFC_COLOR_WHITE;
//				animate_system.animate_list[i].scale = gfc_vector3d(1, 1, 1);
//				animate_system.animate_list[i].powered = 0;
//
//				return &animate_system.animate_list[i];
//			}
//		}
//	}
//	return NULL;
//}
//
//void animate_free(Animation* ent)
//{
//	if (!ent) return;
//	if (ent->free)ent->free(ent);
//	gf3d_mesh_free(ent->mesh);
//	gf3d_texture_free(ent->texture);
//	memset(ent, 0, sizeof(Animation));
//}
//
////void animate_system.animate_list(Enitity* ent);
//
//void animate_system_close()
//{
//	int i;
//	if (animate_system.animate_list)
//	{
//		for (i = 0; i < animate_system.animate_max; i++)
//		{
//			if (animate_system.animate_list[i]._inuse)
//			{
//				animate_free(&animate_system.animate_list[i]);
//				;
//			}
//		}
//		free(animate_system.animate_list);
//	}
//	memset(&animate_system, 0, sizeof(AnimationSystem));
//}
//
//void animate_system_init(Uint32 max_ents)
//{
//	if (!max_ents)
//	{
//		slog("Cannot init animate system with zero ents");
//		return;
//	}
//	animate_system.animate_list = gfc_allocate_array(sizeof(Animation), max_ents);
//	if (!animate_system.animate_list)
//	{
//		slog("Failed to allocate %i entities for the system", max_ents);
//		return;
//	}
//	animate_system.animate_max = max_ents;
//	atexit(animate_system_close);
//	slog("Animation system initialized");
//}
//
//void animate_draw_shadow(Animation* ent)
//{
//	GFC_Vector3D drawPos;
//	GFC_Matrix4 modelMat;
//
//	if ((!ent) || (!ent->drawShadow)) return;
//	gfc_vector3d_copy(drawPos, ent->position);
//	drawPos.z += 0.1;
//	gfc_matrix4_from_vectors(
//		modelMat,
//		drawPos,
//		ent->rotation,
//		gfc_vector3d(ent->scale.x, ent->scale.y, 0.01));
//	gf3d_mesh_draw(
//		ent->mesh,
//		modelMat,
//		gfc_color8(0, 0, 0, 128),
//		ent->texture,
//		gfc_vector3d(0, 0, 0),
//		gfc_color8(0, 0, 0, 0)
//	);
//}
//
//
//
//void animate_draw(Animation* ent, GFC_Vector3D lightPos, GFC_Color lightColor)
//{
//	GFC_Vector3D drawPos;
//	GFC_Matrix4 modelMat;
//	gfc_vector3d_add(drawPos, ent->drawOffset, ent->position);
//	if (!ent)
//	{
//		slog("Couldnt draw animate");
//		return;
//	}
//	gfc_matrix4_from_vectors(
//		modelMat,
//		drawPos,
//		ent->rotation,
//		ent->scale);
//	gf3d_mesh_draw(
//		ent->mesh,
//		modelMat,
//		ent->color,
//		ent->texture,
//		lightPos,
//		lightColor
//	);
//	animate_draw_shadow(ent);
//}
//
//void animate_system_draw_all(GFC_Vector3D lightPos, GFC_Color lightColor)
//{
//	int i;
//	for (i = 0; i < animate_system.animate_max; i++)
//	{
//		if (animate_system.animate_list[i]._inuse)
//		{
//			animate_draw(&animate_system.animate_list[i], lightPos, lightColor);
//		}
//	}
//}
//
//void animate_think(Animation* ent)
//{
//	if (!ent) return;
//
//	if (ent->think) ent->think(ent);
//}
//
//
//void animate_system_think_all()
//{
//	//same as draw all but think
//	int i;
//	for (i = 0; i < animate_system.animate_max; i++)
//	{
//		if (animate_system.animate_list[i]._inuse)
//		{
//			animate_think(&animate_system.animate_list[i]);
//		}
//	}
//}
//
//
//void animate_system_update_all()
//{
//	int i;
//	for (i = 0; i < animate_system.animate_max; i++)
//	{
//		if (animate_system.animate_list[i]._inuse)
//		{
//			animate_update(&animate_system.animate_list[i]);
//		}
//	}
//}
//
//void animate_update(Animation* ent)
//{
//	if (!ent) return;
//	if (ent->doGenericUpdate)
//	{
//		if (ent->stopped == 0)
//		{
//			gfc_vector3d_add(ent->position, ent->position, ent->velocity);
//		}
//
//		gfc_vector3d_scale(ent->velocity, ent->velocity, DAMPEN);
//		if (gfc_vector3d_magnitude_squared(ent->velocity) < 0.01)
//		{
//			ent->velocity = gfc_vector3d(0, 0, 0);
//		}
//	}
//	if (ent->update) ent->update(ent);
//}
//
//
//
//
//void animate_system_move_all()
//{
//	int i;
//	for (i = 0; i < animate_system.animate_max; i++)
//	{
//		if (animate_system.animate_list[i]._inuse)
//		{
//			animate_move(&animate_system.animate_list[i]);
//		}
//	}
//}
//
//
//void animate_move(Animation* self)
//{
//	if (!self) return;
//
//	if (self->move) self->move(self);
//
//	gfc_vector3d_add(self->velocity, self->velocity, self->acceleration);
//
//	if (self->position.x > 400)
//	{
//		self->position.x = 395;
//	}
//	if (self->position.x < -400)
//	{
//		self->position.x = -395;
//	}
//
//	if (self->position.y > 400)
//	{
//		self->position.y = 395;
//	}
//	if (self->position.y < -400)
//	{
//		self->position.y = -395;
//	}
//
//	//(self->position, self->position, self->velocity);
//
//
//
//}
//
//
//Uint8 animate_get_floor_pos(Animation* ent, World* world, GFC_Vector3D* contact)
//{
//	GFC_Vector3D up, down;
//	Uint8 floorCollide, roofCollide;
//	if ((!ent) || (!world)) return 0;
//	up = ent->position;
//	up.z += 100;
//	down = ent->position;
//	//down.z = -1000;
//
//	roofCollide = world_building_collision_test(world, up, down, contact);
//	//slog("%d", roofCollide);
//
//	if (roofCollide)
//	{
//		return 1;
//	}
//	else
//	{
//		return world_edge_test(world, up, down, contact);
//	}
//}
//
//
//void animate_check_collisions()//, World* world);
//{
//	Animation* a, * b;
//	int i, c;
//	Uint8 collision;
//	for (i = 0; i < animate_system.animate_max; i++)
//	{
//		a = &animate_system.animate_list[i];
//		if (!a->_inuse) continue;
//		if (a->collidedType == CT_None) continue;
//
//		a->stopped = 0;
//
//		for (c = 0; c < animate_system.animate_max; c++)
//		{
//
//			b = &animate_system.animate_list[c];
//			if (!b->_inuse) continue;
//			if (b->collidedType == CT_None) continue;
//
//			collision = collision_test(a, b);
//			if (collision)
//			{
//				a->stopped = 1;
//				if (b->powered == 1)
//				{
//					a->energy += 5;
//				}
//				break;
//
//				//slog("Collision: %d, (%s <-> %s)", collision_test(a, b), a->name, b->name);
//
//
//			}
//		}
//
//	}
//}
//
//Animation* animate_list_get()
//{
//
//	if (!animate_system.animate_list)
//	{
//		slog("Cant get animate List");
//		return NULL;
//	}
//	return animate_system.animate_list;
//
//}
//
//Uint32 animate_list_get_max()
//{
//	if (!animate_system.animate_max)
//	{
//		slog("Cant get animate list max");
//		return 0;
//	}
//	return animate_system.animate_max;
//}
//
//
//
////void animate_collide(Animation* ent, Animation* other)
////{
////	if ((!ent) || (!other)) return;
////
////	if (ent->collide) ent->collide(ent, other);
////}
//
//
//Animation* animate_hitscan(Animation* self, GFC_Vector3D start, GFC_Vector3D end, CollidedType* type)
//{
//	int i, j, c, d, f, p;
//
//	GFC_Triangle3D t;
//	GFC_Vector3D contact;
//	GFC_Edge3D e;
//
//
//	e = gfc_edge3d_from_vectors(start, end); //Make an edge
//
//
//	//Check for collision
//	if (!world_get_the()) return;
//	//c = gfc_list_count(world_get_the()->entities); //Get all the world entities
//	c = animate_list_get_max();
//	Animation* animateArray = animate_list_get();
//
//	for (i = 0; i < c; i++) //Iterate through them
//	{
//		Animation* ent = &animateArray[i];
//
//		if (!ent || !ent->mesh) continue;
//		if (!ent->mesh->primitives) continue;
//		if (!ent->_inuse) continue;
//		if (ent == self) continue;
//		f = gfc_list_count(ent->mesh->primitives); //Get all the animate primitives
//		for (p = 0; p < f; p++) //Iterate through them
//		{
//			MeshPrimitive* prim = gfc_list_nth(ent->mesh->primitives, p); //Find them all, and if they dont exist 
//			if ((!prim) || (!prim->objData)) continue;
//			d = prim->objData->face_count;
//			for (j = 0; j < d; j++)
//			{									//You count all the faces that are touching that object
//				t.a = prim->objData->faceVertices[prim->objData->outFace[j].verts[0]].vertex;
//				t.b = prim->objData->faceVertices[prim->objData->outFace[j].verts[1]].vertex;
//				t.c = prim->objData->faceVertices[prim->objData->outFace[j].verts[2]].vertex;
//
//				t.a.x += ent->position.x;
//				t.a.y += ent->position.y;
//				t.a.z += ent->position.z;
//
//				t.b.x += ent->position.x;
//				t.b.y += ent->position.y;
//				t.b.z += ent->position.z;
//
//				t.c.x += ent->position.x;
//				t.c.y += ent->position.y;
//				t.c.z += ent->position.z;
//
//				//slog("Contact Before: %f, %f, %f", contact->x, contact->y, contact->z);
//				if (gfc_trigfc_angle_edge_test(e, t, &contact))
//				{
//					//slog("%d", ent->collidedType);
//					*type = ent->collidedType;
//					return ent;
//
//
//				}
//			}
//		}
//	}
//	return NULL;
//}
//
