#include "simple_logger.h"
#include "simple_json.h"
#include "gf3d_obj_load.h"


#include "gfc_config.h"
#include "gfc_config_def.h"

#include "world.h"
#include "gf3d_mesh.h"
#include "entity.h"
#include "world_entity.h"



typedef struct
{
	World* worldData;
} WorldSystem;


static WorldSystem world_system = { 0 };

static World* theWorld;

World* world_new()
{
	World* world;
	world = gfc_allocate_array(sizeof(World), 1);
	if (!world) return NULL;
	//Do init stuff
	world->entities = gfc_list_new();
	//world->entities = gfc_allocate_array(sizeof(Entity), 1);
	return world;

}
//Take point where your currently at, and where you want to endup (end) and goes through worlds triangles and 
// proforms gfc edgetest (makes edge out of 2 points), go through all triangles in mesh and check if edge
// describing where your moving to is passing through it, if so return 1
Uint8 world_edge_test(World* world, GFC_Vector3D start, GFC_Vector3D end, GFC_Vector3D* contact)
{
	int i, j, c, d;
	GFC_Edge3D e;
	GFC_Triangle3D t;
	MeshPrimitive* prim;
	if (!world) return NULL;
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

World* world_load(const char* filename)
{
	World* world;
	//Space* space;
	const char* str;
	SJson* json, *config;
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
		return;
	}
	str = sj_object_get_string(config, "terrainMesh");
	if (!str)
	{
		slog("Failed to get string terrainMesh");
		sj_free(json);
		world_free(world);
		sj_free(str);
		return;

	}
	world->terrain = gf3d_mesh_load_obj(str);
	//slog("Terrain: %s", world->terrain->filename);
	world->texture = gf3d_texture_load(sj_object_get_string(config, "terrainTexture"));
	//slog("Terrain texture: %s", world->texture->filename);
	world->lightColor = GFC_COLOR_WHITE;
	world->lightPos = gfc_vector3d(0,0,50);
	//space = space_new();
	//space_set_iterations(space, 5);
	//world->space = space;
	gfc_line_cpy(world->name, "World");

	//If everything works, save world data then spawn in game entities
	theWorld = world;
	world_entity_building_spawn(gfc_vector3d(0,150,0), GFC_COLOR_RED);
	
	//sj_object_get_color_value(config, "lightColor", &world->lightColor);
	//sj_object_get_vector3d(config, "lightPos", &world->lightPos);


	sj_free(json);
	return world;
}

void world_free(World* world)
{
	if (!world) return;
	if (world->entities)
	{
		for (int i = 0; i < world->maxEnt; i++)
		{
			entity_free(&world->entities[i]);
		}
	}
	gf3d_mesh_free(world->terrain);
	gf3d_texture_free(world->texture);
	free(world);
}

void world_draw(World* world)
{
	GFC_Matrix4 modelMat;
	if (!world) return;
	gfc_matrix4_identity(modelMat);
	gf3d_mesh_draw(world->terrain, modelMat, GFC_COLOR_WHITE, world->texture, world->lightPos, GFC_COLOR_WHITE);
}


World* world_get_the()
{
	return theWorld;
}
