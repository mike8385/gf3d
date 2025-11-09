#ifndef __GF3D_PARTICLES_H__
#define __GF3D_PARTICLES_H__

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


//forward declaration:
typedef struct ObjData_S ObjData;

//absolute basics of the particles information sent to the graphics card
typedef struct
{
    GFC_Matrix4     model;
    GFC_Matrix4     view;
    GFC_Matrix4     proj;
    GFC_Vector4D    color;
    GFC_Vector4D    camera;
    GFC_Vector4D    lightPos;
    GFC_Vector4D    lightColor;
}ParticleUBO;


typedef struct
{
    GFC_Vector3D pos;
    GFC_Vector2D vertex;
    GFC_Vector2D texel;
}ParticlePoint;

typedef struct
{
    Uint16  verts[3];
}SpriteFace;


typedef struct
{
    GFC_TextLine        filename;
    Uint32              _refCount;
    Uint8               _inuse;
    GFC_List           *primitives;
    GFC_Box             bounds;
    Uint32              vertexCount;
    VkBuffer            vertexBuffer;
    VkDeviceMemory      vertexBufferMemory;
    Uint32              faceCount;
    VkBuffer            faceBuffer;
    VkDeviceMemory      faceBufferMemory;
    ObjData* objData;
}Particle;

/**
 * @brief initializes the particles system / configures internal data about particles based rendering
 * @param particles_max the maximum allowed simultaneous particleses supported at once.  Must be > 0
 * @note keep in mind that many models will be comprised of multiple sub particleses.  So this number may need to be very large
 */
void gf3d_particle_init(Uint32 particles_max);


void gf3d_particle_manager_close();


/**
 * @brief get a new empty model
 * @return NULL on error, or an empty model
 */
Particle *gf3d_particle_new();


/**
 * @brief load particles data from an obj filename.
 * @note: currently only supporting obj files
 * @note this free's the intermediate data loaded from the obj file, no longer needed for most applications
 * @param filename the name of the file to load
 * @return NULL on error or Particle data
 */
Particle *gf3d_particle_load_obj(const char *filename);

/*
* @brief draw a particles given the parameters
*
*/
void gf3d_particle_draw(Particle* particles, GFC_Matrix4 modelMat, GFC_Color mod, Texture* texture, GFC_Vector3D lightPos, GFC_Color lightColor);

/**
 * @brief queue up a render for the current draw frame
 * @param particles the particles to render
 * @param pipe the pipeline to use
 * @param uboData the data to use to draw the particles
 * @param texture texture data to use
 */
void gf3d_particle_queue_render(Particle* particles, Pipeline* pipe, void* uboData, Texture* texture);





/**
 * @brief get the input attribute descriptions for particles based rendering
 * @param count (optional, output) the number of attributes
 * @return a pointer to a vertex input attribute description array
 */
VkVertexInputAttributeDescription * gf3d_particle_get_attribute_descriptions(Uint32 *count);

/**
 * @brief get the binding description for particles based rendering
 * @return vertex input binding descriptions compatible with particles data
 */
VkVertexInputBindingDescription * gf3d_particle_get_bind_description();

/**
 * @brief free a particles that has been loaded from memory
 */
void gf3d_particle_free(Particle *particle);

void gf3d_particle_delete(Particle* particle);





/**
 * @brief create a particles's internal buffers based on vertices
 * @param primitive the particles primitive to populate
 * @note the primitive must have the objData set and it must have be organizes in buffer order
 */
void gf3d_particle_create_vertex_buffer(Particle* particle);

/**
 * @brief create a particles's internal buffers based on faces
 * @param primitive the particles primitive to populate
 * @note the primitive must have the objData set and it must have be organizes in buffer order
 */
void gf3d_particle_create_face_buffer(Particle* particle);

void gf3d_particle_move_vertices(Particle* in, GFC_Vector3D offset, GFC_Vector3D rotation);

/**
 * @brief get the pipeline that is used to render basic 3d particleses
 * @return NULL on error or the pipeline in question
 */
Pipeline *gf3d_particle_get_pipeline();

/**
 * @brief Draw the skybox based on the model provided
 * 
 */


/**
 * @brief given a model matrix and basic color, build the particlesUBO needed to render a model
 * @param modelMat the model Matrix
 * @param colorMod the color for the UBO
 */
ParticleUBO gf3d_particle_get_ubo(
    GFC_Matrix4 modelMat,
    GFC_Color colorMod);









#endif
