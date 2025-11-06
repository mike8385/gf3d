#ifndef __GF3D_DRAW_H__
#define __GF3D_DRAW_H__

#include <stdalign.h>
#include <vulkan/vulkan.h>

#include "gfc_vector.h"
#include "gfc_list.h"
#include "gfc_text.h"
#include "gfc_matrix.h"
#include "gfc_primitives.h"

#include "gf3d_pipeline.h"
#include "gfc_color.h"
#include "gf3d_buffers.h"
#include "gfc_shape.h"
#include "gf3d_mesh.h"


//typedef struct
//{
//    GFC_Matrix4     model;
//    GFC_Matrix4     view;
//    GFC_Matrix4     proj;
//    GFC_Vector4D    color;
//}WireUBO;

/**
 * @brief initializes the draw system / configures internal data about draw based rendering
 * @param draw_max the maximum allowed simultaneous drawes supported at once.  Must be > 0
 * @note keep in mind that many models will be comprised of multiple sub drawes.  So this number may need to be very large
 */
void gf3d_draw_init(Uint32 draw_max);


void gf3d_draw_manager_close();


/**
 * @brief get a new empty model
 * @return NULL on error, or an empty model
 */
Mesh* gf3d_draw_new();


/**
 * @brief load draw data from an obj filename.
 * @note: currently only supporting obj files
 * @note this free's the intermediate data loaded from the obj file, no longer needed for most applications
 * @param filename the name of the file to load
 * @return NULL on error or Mesh data
 */
Mesh* gf3d_draw_load_obj(const char* filename);

/*
* @brief draw a draw given the parameters
*
*/
void gf3d_draw_draw(Mesh* draw, GFC_Matrix4 modelMat, GFC_Color mod, Texture* texture, GFC_Vector3D lightPos, GFC_Color lightColor);

/**
 * @brief queue up a render for the current draw frame
 * @param draw the draw to render
 * @param pipe the pipeline to use
 * @param uboData the data to use to draw the draw
 * @param texture texture data to use
 */
void gf3d_draw_queue_render(Mesh* draw, Pipeline* pipe, void* uboData, Texture* texture);


void gf3d_draw_primitive_queue_render(MeshPrimitive* prim, Pipeline* pipe, void* uboData, Texture* texture);


/**
 * @brief allocate a zero initialized draw primitive
 * @return NULL on error or the primitive
 */
MeshPrimitive* gf3d_draw_primitive_new();



void gf3d_draw_primitive_free(MeshPrimitive* prim);




/**
 * @brief get the input attribute descriptions for draw based rendering
 * @param count (optional, output) the number of attributes
 * @return a pointer to a vertex input attribute description array
 */
VkVertexInputAttributeDescription* gf3d_draw_get_attribute_descriptions(Uint32* count);

/**
 * @brief get the binding description for draw based rendering
 * @return vertex input binding descriptions compatible with draw data
 */
VkVertexInputBindingDescription* gf3d_draw_get_bind_description();

/**
 * @brief free a draw that has been loaded from memory
 */
void gf3d_draw_free(Mesh* draw);

void gf3d_draw_delete(Mesh* draw);





/**
 * @brief create a draw's internal buffers based on vertices
 * @param primitive the draw primitive to populate
 * @note the primitive must have the objData set and it must have be organizes in buffer order
 */
void gf3d_draw_primitive_create_vertex_buffer(MeshPrimitive* primitive);

/**
 * @brief create a draw's internal buffers based on faces
 * @param primitive the draw primitive to populate
 * @note the primitive must have the objData set and it must have be organizes in buffer order
 */
void gf3d_draw_primitive_create_face_buffer(MeshPrimitive* primitive);

void gf3d_draw_move_vertices(Mesh* in, GFC_Vector3D offset, GFC_Vector3D rotation);

/**
 * @brief get the pipeline that is used to render basic 3d drawes
 * @return NULL on error or the pipeline in question
 */
Pipeline* gf3d_draw_get_pipeline();

/**
 * @brief given a model matrix and basic color, build the drawUBO needed to render a model
 * @param modelMat the model Matrix
 * @param colorMod the color for the UBO
 */
MeshUBO gf3d_draw_get_ubo(
    GFC_Matrix4 modelMat,
    GFC_Color colorMod);








#endif