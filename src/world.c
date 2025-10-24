#include "simple_logger.h"
#include "simple_json.h"
#include "gf3d_obj_load.h"

#include "world.h"
#include "gf3d_mesh.h"

World* world_new()
{
	World* world;
	world = gfc_allocate_array(sizeof(World), 1);
	if (!world) return NULL;
//Do init stuff
	return world;

}

//Uint8 world_edge_test(World* world, GFC_Vector3D start, GFC_Vector3D end, GFC_Vector3D* contact)
//{
//	int i, j, c, d;
//	GFC_Edge3D e;
//	GFC_Triangle3D t;
//	MeshPrimitive* prim;
//	if (!world) return NULL;
//	e = gfc_edge3d_from_vectors(start, end);
//	c = list_count
//	c = list_count(world->mesh->prim);
//	for (i = 0; i < c; i++)
//	{
//		//prim = gfc_list_nth(world->mesh->prim, i
//	}
//		//Do edge test against it
//	//e
//	//t
//	//contact
//}

World* world_load(const char* filename)
{
	World* world;
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
	slog("Terrain: %s", world->terrain->filename);
	world->texture = gf3d_texture_load(sj_object_get_string(config, "terrainTexture"));
	slog("Terrain texture: %s", world->texture->filename);
	world->lightColor = GFC_COLOR_WHITE;
	//sj_object_get_color_value(config, "lightColor", &world->lightColor);
	//sj_object_get_vector3d(config, "lightPos", &world->lightPos);
	sj_free(json);
	return world;
}

void world_free(World* world)
{
	if (!world) return;
	gf3d_mesh_free(world->terrain);
	gf3d_texture_free(world->texture);
	free(world);

}

void world_draw(World* world)
{
	GFC_Matrix4 modelMat;
	GFC_Vector3D lightPos = { 0, 0, 50 };
	if (!world) return;
	gfc_matrix4_identity(modelMat);
	gf3d_mesh_draw(world->terrain, modelMat, GFC_COLOR_WHITE, world->texture, lightPos, GFC_COLOR_WHITE);
}
