//#include <SDL.h>    //Precompiled     
//#include <SDL_net.h>
//
//#include "simple_json.h"
//#include "simple_logger.h"
//#include "player.h"
//#include "monster.h"
//#include "world.h"
//#include "gf3d_obj_load.h"
//
//
//typedef struct 
//{
//	GFC_Triangle3D			triangle;
//
//	GFC_List*				neighbors;
//	Uint32					max_neighbors;
//	float					g;
//	float					f;
//	float					h;
//} NavNode;
//
//
//
//void follow_player(Entity* self)
//{
//	int i, v, f, c, j;
//	MeshPrimitive* prim;
//	GFC_Triangle3D t, t2;
//	NavNode start, end;
//	GFC_Vector3D startContact = { 0 };
//	GFC_Vector3D endContact = { 0 };
//	GFC_List neighbors;
//	if (!self) return;
//	World* world = world_get_the();
//	Entity* player = player_get_player();
//	if (!player) return;
//	if (!world) return;
//	
//	start = entity_get_triangle_pos(self, world, &startContact);
//	end = entity_get_triangle_pos(player, world, &endContact);
//
//	//gfc_list_append(&neighbors, &end);
//	if ((start.prim == NULL) || (end.prim == NULL))
//	{
//		slog("No primtive found under entity");
//		return;
//	}
//
//
//
//	//c = gfc_list_count(world->terrain->primitives);
//
//	//for (i = 0; i < c; i++)
//	//{
//	//	prim = gfc_list_get_nth(world->terrain->primitives, i);
//	//	if ((!prim) || (!prim->objData)) continue;
//	//	f = prim->objData->face_count;
//	//	for (j = 0; j < f; j++)
//	//	{									//You count all the faces that are touching that object
//	//		t.a = prim->objData->faceVertices[prim->objData->outFace[j].verts[0]].vertex;
//	//		t.b = prim->objData->faceVertices[prim->objData->outFace[j].verts[1]].vertex;
//	//		t.c = prim->objData->faceVertices[prim->objData->outFace[j].verts[2]].vertex;
//	//		//slog("Contact Before: %f, %f, %f", contact->x, contact->y, contact->z);
//	//		//if (t.a == end->prim->f)
//
//	//}
//
//}
//
///*
//* on update, check if player moved, if not we are good if so update the navmesh
//* This loops through every face and finds neighbors by testing shared edges.
//This function is called ONE TIME when world loads.
//
//for each face, which am i for specific vertex
//*/
//void world_build_navmesh(World* world)
//{
//	int i, c, f, s;
//	if (!world) return;
//	NavNode* navNode;
//	MeshPrimitive* prim;
//
//	
//
//	c = gfc_list_count(world->terrain->primitives);
//	for (i = 0; i < c; i++)
//	{
//		prim = gfc_list_get_nth(world->terrain->primitives, i);
//		f = gfc_list_count(prim->faceCount);
//
//		for (s = 0; s < f; s++)
//		{
//			navMesh->
//		}
//	}
//
//	//c = world->terrain->primitives->f
//
//}
//
///*
//get list of face from vertex
//
//*/
//
//NavNode entity_get_triangle_pos(Entity* ent, World* world, GFC_Vector3D* contact)
//{
//	int i, j, c, d;
//
//	GFC_Vector3D start, end;
//	Uint8 floorCollide, roofCollide;
//	NavNode node = {0};
//
//	GFC_Edge3D e;
//	GFC_Triangle3D t;
//	MeshPrimitive* prim = NULL;
//
//
//	if ((!ent) || (!world)) return node;
//	start = ent->position;
//	start.z += 100;
//	end = ent->position;
//	//down.z = -1000;
//
//
//	if (!world) return node;
//	e = gfc_edge3d_from_vectors(start, end); //Make an edge
//	c = gfc_list_count(world->terrain->primitives); //Get all the terrain primitives
//
//	for (i = 0; i < c; i++) //Iterate through them
//	{
//		prim = gfc_list_nth(world->terrain->primitives, i); //Find them all, and if they dont exist 
//		if ((!prim) || (!prim->objData)) continue;
//		d = prim->objData->face_count;
//		for (j = 0; j < d; j++)
//		{									//You count all the faces that are touching that object
//			t.a = prim->objData->faceVertices[prim->objData->outFace[j].verts[0]].vertex;
//			t.b = prim->objData->faceVertices[prim->objData->outFace[j].verts[1]].vertex;
//			t.c = prim->objData->faceVertices[prim->objData->outFace[j].verts[2]].vertex;
//			//slog("Contact Before: %f, %f, %f", contact->x, contact->y, contact->z);
//			if (gfc_trigfc_angle_edge_test(e, t, contact))
//			{
//				node.triangle = t;
//				return node;
//			}
//		}
//	}
//
//	return node;
//}
