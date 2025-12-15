#ifndef __WORLD_H_
#define __WORLD_H_

#include "gf3d_mesh.h"
//#include "space.h"


typedef struct
{
	Uint32 triangleCount;            // number of triangles
	GFC_List* neighbors;          // neighbors[i] = list of neighboring face indices
	Uint32 neighborCount;       // neighborCount[i] tells how many neighbors each face has
	GFC_Vector3D* faceCenters;   // center point of each triangle (optional but useful)

} NavMesh;

typedef struct
{
	Mesh*					terrain;
	Texture*				texture;
	GFC_List*				entities; //Just the ones in the world
	GFC_List*				terrainMesh;
	Uint8					maxEnt;
	Uint8					maxMesh;
	GFC_Color				lightColor;
	GFC_Vector3D			lightPos;
	GFC_TextLine			name;
	//Space*				space;
	float					length;
	float					width;
	NavMesh*				navMesh;

}World;



/*
* @brief get a pointer to a new blank world
* @return NULL on out of memory or other error, a pointer to a blank world otherwise
*/
World* world_new();

/**
 * @brief load world data from JSON filename
 * @param filename the name of the file to load
 * @return NULL on error or World data
 */
World* world_load(const char* filename);

/*
* @brief The free function for the world. Frees up all memory allocation and clears world data
* @param self the world itself
*/
void world_free(World* world);

/*
* @brief The draw function for the world
* @param self the world itself
*/
void world_draw(World* world);


Uint8 world_edge_test(World* world, GFC_Vector3D start, GFC_Vector3D end, GFC_Vector3D* contact);

Uint8 world_building_collision_test(World* world, GFC_Vector3D start, GFC_Vector3D end, GFC_Vector3D* contact);


World* world_get_the();


GFC_Vector2D world_get_length_width();

#endif
