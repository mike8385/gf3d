#include "simple_logger.h"

#include "simple_json.h"

#include "gf3d_vgraphics.h"
#include "gf3d_obj_load.h"
#include "gfc_color.h"
#include "gf3d_camera.h"



#include "particles.h"

//Null for face and stuff, and 1 for vector i think, and only use 1. Something like that use sprite too.

#define PARTICLE_ATTRIBUTE_COUNT 1

extern int __DEBUG;


typedef struct
{
	Particle*                               particle_list;
	Uint32                                  particle_count;
    Uint32                                  chain_length;     /**<length of swap chain*/
    VkDevice                                device;           /**<logical vulkan device*/
    Pipeline*                               pipe;             /**<the pipeline associated with particle rendering*/
    VkBuffer                                faceBuffer;
    VkDeviceMemory                          faceBufferMemory;
    VkVertexInputAttributeDescription       attributeDescriptions[PARTICLE_ATTRIBUTE_COUNT];
    VkVertexInputBindingDescription         bindingDescription;
    float                                   drawOrder;
    Texture*                                defaultTexture;
} ParticleManager;

static ParticleManager particle_manager = { 0 };

//Null for 



void gf3d_particle_manager_close()
{
    int i;
    for (i = 0; i < particle_manager.particle_count; i++)
    {
        gf3d_particle_delete(&particle_manager.particle_list[i]);
    }
    if (particle_manager.particle_list)
    {
        free(particle_manager.particle_list);
    }

    memset(&particle_manager, 0, sizeof(ParticleManager));
    if (__DEBUG)slog("Particle manager closed");
}


void gf3d_particle_init(Uint32 particle_max)
{
    Uint32 count = 0;
    if (particle_max == 0)
    {
        slog("cannot intilizat particle manager for 0 particle");
        return;
    }
    particle_manager.chain_length = gf3d_swapchain_get_chain_length();
    particle_manager.particle_list = (Particle*)gfc_allocate_array(sizeof(Particle), particle_max);
    particle_manager.particle_count = particle_max;
    particle_manager.device = gf3d_vgraphics_get_default_logical_device();


    particle_manager.pipe = gf3d_pipeline_create_from_config(
        gf3d_vgraphics_get_default_logical_device(),
        "config/particle_pipeline.cfg",
        gf3d_vgraphics_get_view_extent(),
        particle_max,
        gf3d_particle_get_bind_description(),
        gf3d_particle_get_attribute_descriptions(NULL),
        count,
        sizeof(ParticleUBO),
        VK_INDEX_TYPE_UINT16
    );


    particle_manager.defaultTexture = gf3d_texture_load("images/default.png");
    if (__DEBUG)slog("particle manager initiliazed");
    atexit(gf3d_particle_manager_close);
}

void gf3d_particle_delete(Particle* particle)
{
    int i, c;
    if (!particle)return;

    //if (particle->buffer != VK_NULL_HANDLE)
    //{
    //    vkDestroyBuffer(gf2d_sprite.device, sprite->buffer, NULL);
    //}
    //if (sprite->bufferMemory != VK_NULL_HANDLE)
    //{
    //    vkFreeMemory(gf2d_sprite.device, sprite->bufferMemory, NULL);
    //}

   // gf3d_texture_free(particle->texture);
    memset(particle, 0, sizeof(Particle));
}


Particle* gf3d_particle_new()
{
    int i;
    for (i = 0; i < particle_manager.particle_count; i++)
    {
        if (particle_manager.particle_list[i]._inuse)continue;
        particle_manager.particle_list[i]._inuse = 1;
        return &particle_manager.particle_list[i];
    }
    slog("gf3d_particle_new: no free slots for new particlees");
    return NULL;
}



Particle* gf3d_particle_get_by_filename(const char* filename)
{
    int i;
    if (!filename)return NULL;
    for (i = 0; i < particle_manager.particle_count; i++)
    {
        if (!particle_manager.particle_list[i]._inuse)continue;
        if (gfc_line_cmp(particle_manager.particle_list[i].name, filename) == 0)
        {
            return &particle_manager.particle_list[i];
        }
    }
    return NULL;
}


Particle* gf3d_particle_load(GFC_Color color, GFC_Vector3D position)
{
    Particle* particle;
    //ObjData* obj;

   particle = gf3d_particle_new();
    if (!particle)
    {
        //gf3d_obj_free(obj);
        slog("Failed to make new particle");
        return NULL;
    }
    //Probably doesnt work because I dont initialize anything
    particle->color = color;
    particle->position = position;

    //particle->objData = obj;


    gf3d_particle_create_vertex_buffer(particle);
    return particle;
}

VkVertexInputAttributeDescription* gf3d_particle_get_attribute_descriptions(Uint32* count)
{
    particle_manager.attributeDescriptions[0].binding = 0;
    particle_manager.attributeDescriptions[0].location = 0;
    particle_manager.attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    particle_manager.attributeDescriptions[0].offset = 0;
    if (count)*count = PARTICLE_ATTRIBUTE_COUNT;
    return particle_manager.attributeDescriptions;
}

VkVertexInputBindingDescription* gf3d_particle_get_bind_description()
{
    particle_manager.bindingDescription.binding = 0;
    particle_manager.bindingDescription.stride = sizeof(GFC_Vector3D);
    particle_manager.bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return &particle_manager.bindingDescription;
}

void gf3d_particle_create_vertex_buffer(Particle* particle)
{
    void* data = NULL;
    VkDevice device = particle_manager.device;
    size_t bufferSize;
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    GFC_Vector3D vertices[] = {gfc_vector3d(0, 0, 0) };
    
    bufferSize = sizeof(GFC_Vector3D);

    gf3d_buffer_create(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &stagingBuffer, &stagingBufferMemory);

    vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices, bufferSize);
    vkUnmapMemory(device, stagingBufferMemory);

    gf3d_buffer_create(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &particle->vertexBuffer, &particle->vertexBufferMemory);

    gf3d_buffer_copy(stagingBuffer, particle->vertexBuffer, bufferSize);

    vkDestroyBuffer(device, stagingBuffer, NULL);
    vkFreeMemory(device, stagingBufferMemory, NULL);
}


void gf3d_particle_free(Particle* particle)
{
    if (!particle)return;
    particle->_refCount--;
    if (particle->_refCount <= 0)gf3d_particle_delete(particle);
}






//Only one to keep
void gf3d_particle_queue_render(Particle* particle, Pipeline* pipe, void* uboData, Texture* texture)
{
    int i, c;
    if ((!particle) || (!pipe) || (!uboData)) return;
    if (!texture) texture = particle_manager.defaultTexture;
    gf3d_pipeline_queue_render(
        pipe,
        particle->vertexBuffer,
        1,
        NULL,
        uboData,
        texture);



}


void gf3d_particle_draw(Particle* particle, GFC_Matrix4 modelMat, GFC_Color mod, Texture* texture)
{

    ParticleUBO ubo = { 0 };
    //slog("In draw");
    if (!particle) return;
    gfc_matrix4_copy(ubo.model, modelMat);
    gf3d_vgraphics_get_view(&ubo.view);


    gf3d_vgraphics_get_projection_matrix(&ubo.proj);

    gf3d_vgraphics_get_view_extent_as_vector2d(&ubo.viewportSize);


    gf3d_particle_queue_render(particle, particle_manager.pipe, &ubo, texture);
}


Pipeline* gf3d_particle_get_pipeline()
{
    return particle_manager.pipe;
}




