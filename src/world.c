#include <string.h>

#include "simple_logger.h"
#include "simple_json.h"
#include "gf3d_obj_load.h"


#include "gfc_config.h"
#include "gfc_config_def.h"

#include "world.h"
#include "gf3d_mesh.h"
#include "entity.h"
#include "world_entity.h"
#include "shards.h"
#include "player.h"
#include "monster.h"
#include "camera_entity.h"


typedef struct
{
	World* worldData;
} WorldSystem;


static WorldSystem world_system = { 0 };

static World* theWorld;

World* world_new()
{
	World* world = NULL;
	world = gfc_allocate_array(sizeof(World), 1);
	if (!world)
	{
		slog("Cannot allocate spacefor world");
		return NULL;
	}
	//Do init stuff
	world->entities = gfc_list_new();
	world->maxEnt = 0;
	world->navMesh = gfc_allocate_array(sizeof(NavMesh), 1);
	//world->entities = gfc_allocate_array(sizeof(Entity), 1);
	return world;

}
//Take point where your currently at, and where you want to endup (end) and goes through worlds triangles and 
// proforms gfc edgetest (makes edge out of 2 points), go through all triangles in mesh and check if edge
// describing where your moving to is passing through it, if so return 1
Uint8 world_edge_test(World* world, GFC_Vector3D start, GFC_Vector3D end, GFC_Vector3D* contact)
{
	int i, j, c, c2, d, f, p;
	GFC_Edge3D e, e2;
	GFC_Triangle3D t, t2;
	MeshPrimitive* prim = NULL, *prim2 = NULL;
	if (!world) return 0;
	e = gfc_edge3d_from_vectors(start, end); //Make an edge
	c = gfc_list_count(world->terrain->primitives); //Get all the terrain primitives
	
	for (i = 0; i < c; i++) //Iterate through them
	{
		prim = gfc_list_nth(world->terrain->primitives, i); //Find them all, and if they dont exist 
		if ((!prim) || (!prim->objData)) continue;
		d = prim->objData->face_count;
		for (j = 0; j < d; j++)
		{									//You count all the faces that are touching that object
			t.a = prim->objData->faceVertices[prim->objData->outFace[j].verts[0]].vertex;
			t.b = prim->objData->faceVertices[prim->objData->outFace[j].verts[1]].vertex;
			t.c = prim->objData->faceVertices[prim->objData->outFace[j].verts[2]].vertex;
			//slog("Contact Before: %f, %f, %f", contact->x, contact->y, contact->z);
			if (gfc_trigfc_angle_edge_test(e,t, contact))
			{
				//slog("Contact AFTER: %f, %f, %f", contact->x, contact->y, contact->z);
				return 1;
			}
		}
	}

	return 0;
}

Uint8 world_building_collision_test(World* world, GFC_Vector3D start, GFC_Vector3D end, GFC_Vector3D* contact)
{
	int i, j, c, d, f, p;
	GFC_Edge3D e;
	GFC_Triangle3D t;
	MeshPrimitive* prim;
	Entity* ent;
	e = gfc_edge3d_from_vectors(start, end); //Make an edge
	c = gfc_list_count(world->entities); //Get all the world entitiesterrain Mesh

	for (i = 0; i < c; i++) //Iterate through them
	{
		ent = gfc_list_get_nth(world->entities, i);
		if (!ent || !ent->mesh) continue;
		f = gfc_list_count(ent->mesh->primitives); //Get all the entity primitives
		for (p = 0; p < f; p++) //Iterate through them
		{
			prim = gfc_list_nth(ent->mesh->primitives, p); //Find them all, and if they dont exist 
			if ((!prim) || (!prim->objData)) continue;
			d = prim->objData->face_count;
			for (j = 0; j < d; j++)
			{									//You count all the faces that are touching that object
				t.a = prim->objData->faceVertices[prim->objData->outFace[j].verts[0]].vertex;
				t.b = prim->objData->faceVertices[prim->objData->outFace[j].verts[1]].vertex;
				t.c = prim->objData->faceVertices[prim->objData->outFace[j].verts[2]].vertex;

				t.a.x += ent->position.x;
				t.a.y += ent->position.y;
				t.a.z += ent->position.z;

				t.b.x += ent->position.x;
				t.b.y += ent->position.y;
				t.b.z += ent->position.z;

				t.c.x += ent->position.x;
				t.c.y += ent->position.y;
				t.c.z += ent->position.z;

				//slog("Contact Before: %f, %f, %f", contact->x, contact->y, contact->z);
				if (gfc_trigfc_angle_edge_test(e, t, contact))
				{
					//slog("Contact AFTER: %f, %f, %f", contact->x, contact->y, contact->z);
					return 1;
				}
			}
		}
	}

	return 0;
}

World* world_load(const char* filename)
{
	World* world = NULL;
	

	//Space* space;
	const char* str = NULL;
	float l = 0, w = 0;
	SJson* json = NULL, *config = NULL;
	SJson* shards;
	SJson* worldEntitiesArray;
	SJson* enemies;
	json = sj_load(filename);
	if (!json)
	{
		slog("Couldnt find filename % s", filename);
		return NULL;
	}
	world = world_new();
	if (!world)
	{
		slog("Failed to allocate a world for %s", filename);
		sj_free(json);
		return NULL;
	}
	config = sj_object_get_value(json, "world");
	if (!config)
	{
		slog("Failed to parse a world for %s", filename);
		sj_free(json);
		world_free(world);
		return NULL;
	}

	str = sj_object_get_string(config, "name");
	if (!str) gfc_line_cpy(str, "NoWorldName");
	gfc_line_cpy(world->name, str);

	str = sj_object_get_string(config, "terrainMesh");
	if (!str)
	{
		slog("Failed to get string terrainMesh");
		sj_free(json);
		world_free(world);
		sj_free(str);
		return NULL;

	}
	world->terrain = gf3d_mesh_load_obj(str);
	if (!world->terrain)
	{
		slog("Cant get world terrain");
		return;
	}
	//slog("Terrain: %s", world->terrain->filename);
	world->texture = gf3d_texture_load(sj_object_get_string(config, "terrainTexture"));
	if (!world->texture)
	{
		slog("No world texture added, assigning default");
	}

	//slog("Terrain texture: %s", world->texture->filename);
	GFC_Color lightColor = { 0 };
	lightColor = sj_object_get_color(config, "lightColor");
	world->lightColor = lightColor;

	GFC_Vector3D lightPos = { 0 };
	sj_object_get_vector3d(config, "lightPos", &lightPos);
	world->lightPos = lightPos;

	sj_object_get_float(config, "length", &l);
	world->length = l;
	sj_object_get_float(config, "width", &w);
	world->width = w;

	//space = space_new();
	//space_set_iterations(space, 5);
	//world->space = space;
	

	//If everything works, save world data then spawn in game entities
	theWorld = world;

	//World Entity Spawn:
	worldEntitiesArray = sj_object_get_value(json, "worldEntities");
	int arrayIndex = sj_array_get_count(worldEntitiesArray);

	for (int i = 0; i < arrayIndex; i++)
	{
		SJson* worldEntities = sj_array_get_nth(worldEntitiesArray, i);
		const char* type = sj_object_get_value_as_string(worldEntities, "type");
		GFC_Vector3D wEntPos = { 0 };
		GFC_Color wEntColor = { 0 };
		sj_object_get_vector3d(worldEntities, "position", &wEntPos);
		wEntColor = sj_object_get_color(worldEntities, "color");

		if (strcmp(type, "building") == 0)
		{
			world_entity_building_spawn(wEntPos, wEntColor);
		}
		else if (strcmp(type, "lamp") == 0)
		{
			world_entity_lamp_spawn(wEntPos, wEntColor);
		}
	}

	//Shard Spawn:
	SJson* shardsArray = sj_object_get_value(json, "shards");
	int shardsArrayIndex = sj_array_get_count(shardsArray);

	for (int i = 0; i < shardsArrayIndex; i++)
	{
		SJson* shards = sj_array_get_nth(shardsArray, i);
		GFC_Vector3D shardPos = { 0 };
		GFC_Color shardColor = { 0 };
		sj_object_get_vector3d(shards, "position", &shardPos);
		shardColor = sj_object_get_color(shards, "color");
		shard_spawn(shardPos, shardColor);
	}

	//Entity Spawn:
	SJson* entitiesArray = sj_object_get_value(json, "entities");
	int entityArrayIndex = sj_array_get_count(entitiesArray);

	for (int i = 0; i < entityArrayIndex; i++)
	{
		SJson* entities = sj_array_get_nth(entitiesArray, i);
		const char* type = sj_object_get_value_as_string(entities, "type");
		GFC_Vector3D entPos = { 0 };
		GFC_Color entColor = { 0 };
		sj_object_get_vector3d(entities, "position", &entPos);
		entColor = sj_object_get_color(entities, "color");
		if (!&entColor)
		{
			entColor = gfc_color(255, 255, 255, 255);
		}

		if (strcmp(type, "player") == 0)
		{
			player_spawn(entPos, entColor);
		}
		else if (strcmp(type, "monster") == 0)
		{
			monster_spawn(entPos, entColor);
		}
		else if (strcmp(type, "camera") == 0)
		{
			camera_entity_spawn(entPos, player_get_player());
		}
	}

	slog("%s, %f, %f", world->name, world->length, world->width);

	slog("Successfully built world");
	sj_free(json);
	return world;
}

void world_free(World* world)
{
	if (!world) return;
	if (world->entities)
	{
		int count = gfc_list_count(world->entities);
		for (int i = 0; i < count; i++)
		{
			Entity* e = gfc_list_get_nth(world->entities, i);
			entity_free(e);
		}
	}
	gf3d_mesh_free(world->terrain);
	gf3d_texture_free(world->texture);
	free(world);
}

void world_draw(World* world)
{
	GFC_Matrix4 modelMat;
	if (!world)
	{
		slog("Couldnt get world to draw");
		return;
	}
	gfc_matrix4_identity(modelMat);
	gf3d_mesh_draw(world->terrain, modelMat, GFC_COLOR_WHITE, world->texture, world->lightPos, GFC_COLOR_WHITE);
}


World* world_get_the()
{
	if (!theWorld)
	{
		slog("Couldnt get the world");
		return NULL;
	}
	
	return theWorld;
}


GFC_Vector2D world_get_length_width()
{
	if (!theWorld)
	{
		slog("Cannot get world");
		return gfc_vector2d(0,0);
	}
	return gfc_vector2d(theWorld->length, theWorld->width);
}



