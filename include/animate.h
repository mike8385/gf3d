//#ifndef __ANIMATE_H__
//#define __ANIMATE_H__
//
//#include "gfc_text.h"
//#include "gfc_matrix.h"
//
//#include "gf3d_mesh.h"
////Include vector, maatrix, 
//
//
//
//typedef struct Animate_S
//{
//
//
//}Animation;
//
//
//
///*
//* @brief get a pointer to a new blank animate
//* @return NULL on out of memory or other error, a pointer to a blank animate otherwise
//*/
//Animation* animate_new();
//
///*
//* @brief free a previously new'd animate
//* @param ent tje entioty to be freed
//* @note the memory address should no longer be used
//*/
//void animate_free(Animation* ent);
//
///*
//@brief Initilizes the enttiy subsystem
//@param max_ents jpw ,amu tp sipport concurrently
//*/
//void animate_system_init(Uint32 max_ents);
//
///*
//* @brief draw all entittes with the provided light
//* @param lightPos where the loght is in world space
//* @
//*/
//void animate_system_draw_all(GFC_Vector3D lightPos, GFC_Color lightColor);
//
//
//void animate_system_think_all();
//
//void animate_system_update_all();
//
//void animate_think(Animation* ent);
//
//void animate_update(Animation* ent);
//
//void animate_system_move_all();
//
//void animate_move();
//
//Uint8 animate_get_floor_pos(Animation* ent, World* world, GFC_Vector3D* contact);
//
//void animate_draw_shadow(Animation* ent);
//
//
//
//Animation* animate_list_get();
//
//Uint32 animate_list_get_max();
//
///*
//* @brief Does a hitscan against all entities that exist. Sets the type to whatever the entities type is
//* @param self the animate itself
//* @param other the other animate hit
//* @param start the starting position of the scan
//* @param end the ending position of scan
//* @param type the collision type (if wanted)
//* @return the animate hit
//*/
//#endif