

#include "simple_logger.h"
#include "simple_json.h"
#include "gf3d_obj_load.h"

#include "world.h"






World* world_new()
{
	World* world;
	world = gfc_allocate_array(sizeof(World), 1);
	if (!world) return NULL;
//Do init stuff
	return world;

}

Uint8 world_edge_test(World* world, GFC_Vector3D start, GFC_Vector3D end, GFC_Vector3D* contact)
{
	int i, j, c, d;
	GFC_Edge3D e;
	GFC_Triangle3D t;
	//MeshPrim* prim
	if (!world) return NULL;
	e = gfc_edge3d_from_vectors(start, end);
	//c = list_count(world->mesh->prim
	for (i = 0; i < c; i++)
	{
		//prim = gfc_list_nth(world->mesh->prim, i
	}
		//Do edge test against it
	//e
	//t
	//contact
}

World* world_load(const char* filename)
{
	World* world;
	SJson* json, *config;
	json = sj_load(filename);
	if (!json)
	{
		//Slog why it failed
		return NULL;
	}
	world = world_new();
	if (!world)
	{
		//Slog why
		sj_free(json);
		return NULL;
	}
	config = sj_object_get_value(json, "world");
	if (!config)
	{
		//Slog
		//sj_free both
		//return
	}
	//str = sj_obj_get_string(config, "TerrainMesh")
	//if str world->terrain = mesh_load(str)
	//world->texture = gf3d_texture_load(sj_object_get_string(config, "terrainTexture"))
	//world->lightcolor = GFC_COLOR_WHITE
	sj_object_get_color_value();

	sj_free(json);
	return world;
}

void world_free(World* world)
{
	//If ! world, return
	//gf3d_world_free(terrain)
	//texture free
	//free(world)

}

void world_draw(World* world)
{
	GFC_Matrix4 modelMat;
	if (!world) return;
	gfc_matrix4_identity(modelMat);
	//mesh_draw
	//Draw entities here
}
