#include "simple_logger.h"

#include "simple_json.h"

#include "gf3d_vgraphics.h"
#include "gf3d_obj_load.h"
#include "gfc_color.h"
#include "gf3d_camera.h"



#include "gf3d_particle.h"

#define MESH_ATTRIBUTE_COUNT 3

extern int __DEBUG;


typedef struct
{
    Mesh* particle_list;
    Uint32                                  particle_count;
    Uint32                                  chain_length;     /**<length of swap chain*/
    VkDevice                                device;           /**<logical vulkan device*/
    Pipeline* pipe;             /**<the pipeline associated with particle rendering*/
    Pipeline* sky_pipe;
    Pipeline* wire_pipe;
    VkBuffer                                faceBuffer;
    VkDeviceMemory                          faceBufferMemory;
    VkVertexInputAttributeDescription       attributeDescriptions[MESH_ATTRIBUTE_COUNT];
    VkVertexInputBindingDescription         bindingDescription;
    float                                   drawOrder;
    Texture* defaultTexture;
} MeshManager;

static MeshManager particle_manager = { 0 };





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

    memset(&particle_manager, 0, sizeof(MeshManager));
    if (__DEBUG)slog("Mesh manager closed");
}


void gf3d_particle_init(Uint32 particle_max)
{
    Uint32 count = 0;
    if (particle_max == 0)
    {
        slog("cannot intilizat particle manager for 0 particles");
        return;
    }
    particle_manager.chain_length = gf3d_swapchain_get_chain_length();
    particle_manager.particle_list = (Mesh*)gfc_allocate_array(sizeof(Mesh), particle_max);
    particle_manager.particle_count = particle_max;
    particle_manager.device = gf3d_vgraphics_get_default_logical_device();


    gf3d_particle_get_attribute_descriptions(&count);
    particle_manager.sky_pipe = gf3d_pipeline_create_from_config(
        gf3d_vgraphics_get_default_logical_device(),
        "config/sky_pipeline.cfg",
        gf3d_vgraphics_get_view_extent(),
        particle_max,
        gf3d_particle_get_bind_description(),
        gf3d_particle_get_attribute_descriptions(NULL),
        count,
        sizeof(SkyUBO),
        VK_INDEX_TYPE_UINT16
    );

    particle_manager.pipe = gf3d_pipeline_create_from_config(
        gf3d_vgraphics_get_default_logical_device(),
        "config/model_pipeline.cfg",
        gf3d_vgraphics_get_view_extent(),
        particle_max,
        gf3d_particle_get_bind_description(),
        gf3d_particle_get_attribute_descriptions(NULL),
        count,
        sizeof(MeshUBO),
        VK_INDEX_TYPE_UINT16
    );
    particle_manager.wire_pipe = gf3d_pipeline_create_from_config(
        gf3d_vgraphics_get_default_logical_device(),
        "config/wire_pipeline.cfg",
        gf3d_vgraphics_get_view_extent(),
        particle_max,
        gf3d_particle_get_bind_description(),
        gf3d_particle_get_attribute_descriptions(NULL),
        count,
        sizeof(WireUBO),
        VK_INDEX_TYPE_UINT16
    );

    particle_manager.defaultTexture = gf3d_texture_load("images/default.png");
    if (__DEBUG)slog("particle manager initiliazed");
    atexit(gf3d_particle_manager_close);
}

void gf3d_particle_delete(Mesh* particle)
{
    int i, c;
    MeshPrimitive* prim;
    if (!particle)return;
    c = gfc_list_count(particle->primitives);
    for (i = 0; i < c; i++)
    {
        prim = gfc_list_get_nth(particle->primitives, i);
        if (!prim) continue;
        gf3d_particle_primitive_free(prim);
    }
    if (particle->primitives) gfc_list_delete(particle->primitives);

    //if (particle->buffer != VK_NULL_HANDLE)
    //{
    //    vkDestroyBuffer(gf2d_sprite.device, sprite->buffer, NULL);
    //}
    //if (sprite->bufferMemory != VK_NULL_HANDLE)
    //{
    //    vkFreeMemory(gf2d_sprite.device, sprite->bufferMemory, NULL);
    //}

   // gf3d_texture_free(particle->texture);
    memset(particle, 0, sizeof(Mesh));
}


Mesh* gf3d_particle_new()
{
    int i;
    for (i = 0; i < particle_manager.particle_count; i++)
    {
        if (particle_manager.particle_list[i]._refCount)continue;
        particle_manager.particle_list[i]._refCount = 1;
        return &particle_manager.particle_list[i];
    }
    slog("gf3d_particle_new: no free slots for new particlees");
    return NULL;
}

Mesh* gf3d_particle_get_by_filename(const char* filename)
{
    int i;
    if (!filename)return NULL;
    for (i = 0; i < particle_manager.particle_count; i++)
    {
        if (!particle_manager.particle_list[i]._refCount)continue;
        if (gfc_line_cmp(particle_manager.particle_list[i].filename, filename) == 0)
        {
            return &particle_manager.particle_list[i];
        }
    }
    return NULL;
}


Mesh* gf3d_particle_load_obj(const char* filename)
{
    MeshPrimitive* primitive;
    Mesh* particle;
    ObjData* obj;

    if (!filename) return NULL;
    //particle = gf3d_particle_get_by_filename(filename);
    //if (particle)
    //{
    //    particle->_refCount++;  //If particle currently exists, grab it and end call
    //    return particle;
    //}
    obj = gf3d_obj_load_from_file(filename); //Parse data from file
    if (!obj)
    {
        slog("Failed to parse obj file %s", filename);
        return NULL;
    }
    particle = gf3d_particle_new();
    if (!particle)
    {
        gf3d_obj_free(obj);
        slog("Failed to make new particle");
        return NULL;
    }

    primitive = gf3d_particle_primitive_new();

    if (!primitive)
    {
        gf3d_obj_free(obj);
        gf3d_particle_free(particle);
        slog("Failed to make new particlePrim");
        return NULL;
    }

    particle->primitives = gfc_list_new();
    gfc_list_append(particle->primitives, primitive);

    primitive->objData = obj;

    gf3d_particle_primitive_create_vertex_buffer(primitive);
    gf3d_particle_primitive_create_face_buffer(primitive);
    gfc_line_cpy(particle->filename, filename);
    //gf3d_particle_create_vertex_buffer(particle);
    return particle;
}


void gf3d_particle_primitive_free(MeshPrimitive* prim)
{

}



MeshPrimitive* gf3d_particle_primitive_new()
{
    MeshPrimitive* prim = NULL;
    prim = (MeshPrimitive*)gfc_allocate_array(sizeof(MeshPrimitive), 1);

    if (!prim)
    {
        slog("Couldnt make a new particle primitive memory");
        return NULL;
    }


    return prim;

}


VkVertexInputAttributeDescription* gf3d_particle_get_attribute_descriptions(Uint32* count)
{
    particle_manager.attributeDescriptions[0].binding = 0;
    particle_manager.attributeDescriptions[0].location = 0;
    particle_manager.attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    particle_manager.attributeDescriptions[0].offset = offsetof(Vertex, vertex);

    particle_manager.attributeDescriptions[1].binding = 0;
    particle_manager.attributeDescriptions[1].location = 1;
    particle_manager.attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    particle_manager.attributeDescriptions[1].offset = offsetof(Vertex, normal);

    particle_manager.attributeDescriptions[2].binding = 0;
    particle_manager.attributeDescriptions[2].location = 2;
    particle_manager.attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
    particle_manager.attributeDescriptions[2].offset = offsetof(Vertex, texel);
    if (count)*count = MESH_ATTRIBUTE_COUNT;
    return particle_manager.attributeDescriptions;
}

VkVertexInputBindingDescription* gf3d_particle_get_bind_description()
{
    particle_manager.bindingDescription.binding = 0;
    particle_manager.bindingDescription.stride = sizeof(Vertex);
    particle_manager.bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return &particle_manager.bindingDescription;
}

void gf3d_particle_free(Mesh* particle)
{
    if (!particle)return;
    particle->_refCount--;
    if (particle->_refCount <= 0)gf3d_particle_delete(particle);
}


void gf3d_particle_primitive_create_vertex_buffer(MeshPrimitive* prim)
{
    void* data = NULL;
    VkDevice device = gf3d_vgraphics_get_default_logical_device();
    Vertex* verticies;
    Uint32 vcount;
    size_t bufferSize;
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory = VK_NULL_HANDLE;

    if (!prim)
    {
        slog("No particle primitize provided");
        return;
    }

    verticies = prim->objData->faceVertices;
    vcount = prim->objData->face_vert_count;
    //faces = prim->objData->outFace;
    //fcount = prim->objData->face_count;
    bufferSize = sizeof(Vertex) * vcount;
    gf3d_buffer_create(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
        VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &stagingBuffer, &stagingBufferMemory);

    vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, verticies, (size_t)bufferSize);
    vkUnmapMemory(device, stagingBufferMemory);

    gf3d_buffer_create(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &prim->vertexBuffer, &prim->vertexBufferMemory);

    gf3d_buffer_copy(stagingBuffer, prim->vertexBuffer, bufferSize);

    vkDestroyBuffer(device, stagingBuffer, NULL);
    vkFreeMemory(device, stagingBufferMemory, NULL);

    prim->vertexCount = vcount;

}


void gf3d_particle_primitive_create_face_buffer(MeshPrimitive* prim)
{
    void* data = NULL;
    VkDevice device = gf3d_vgraphics_get_default_logical_device();
    Face* faces;
    Uint32 fcount;
    size_t bufferSize;
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory = VK_NULL_HANDLE;

    if (!prim)
    {
        slog("No particle primitize provided");
        return;
    }

    faces = prim->objData->outFace;
    fcount = prim->objData->face_count;
    //faces = prim->objData->outFace;
    //fcount = prim->objData->face_count;
    bufferSize = sizeof(Face) * fcount;
    gf3d_buffer_create(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
        VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &stagingBuffer, &stagingBufferMemory);

    vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, faces, (size_t)bufferSize);
    vkUnmapMemory(device, stagingBufferMemory);

    gf3d_buffer_create(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &prim->faceBuffer, &prim->faceBufferMemory);

    gf3d_buffer_copy(stagingBuffer, prim->faceBuffer, bufferSize);

    prim->faceCount = fcount;

    vkDestroyBuffer(device, stagingBuffer, NULL);
    vkFreeMemory(device, stagingBufferMemory, NULL);

}


//Only one to keep
void gf3d_particle_queue_render(Mesh* particle, Pipeline* pipe, void* uboData, Texture* texture)
{
    int i, c;
    MeshPrimitive* prim;
    if ((!particle) || (!pipe) || (!uboData)) return;
    c = gfc_list_count(particle->primitives);
    for (i = 0; i < c; i++)
    {
        prim = gfc_list_nth(particle->primitives, i);
        if (!prim) continue;
        gf3d_particle_primitive_queue_render(prim, pipe, uboData, texture);
    }


}


void gf3d_particle_primitive_queue_render(MeshPrimitive* prim, Pipeline* pipe, void* uboData, Texture* texture)
{
    if ((!prim) || (!pipe) || (!uboData)) return;
    if (!texture) texture = particle_manager.defaultTexture;
    gf3d_pipeline_queue_render(
        pipe,
        prim->vertexBuffer,
        prim->vertexCount,
        prim->faceBuffer,
        uboData,
        texture);
}




void gf3d_particle_draw(Mesh* particle, GFC_Matrix4 modelMat, GFC_Color mod, Texture* texture, GFC_Vector3D lightPos, GFC_Color lightColor)
{
    MeshUBO ubo = { 0 };
    //slog("In draw");
    if (!particle) return;
    gfc_matrix4_copy(ubo.model, modelMat);
    gf3d_vgraphics_get_view(&ubo.view);


    gf3d_vgraphics_get_projection_matrix(&ubo.proj);

    ubo.color = gfc_color_to_vector4f(mod);
    ubo.lightColor = gfc_color_to_vector4f(lightColor);
    ubo.lightPos = gfc_vector3dw(lightPos, 1.0);


    ubo.camera = gfc_vector3dw(gf3d_camera_get_position(), 1.0);

    gf3d_particle_queue_render(particle, particle_manager.pipe, &ubo, texture);
}

void gf3d_particle_sky_draw(Mesh* particle, GFC_Matrix4 modelMat, GFC_Color mod, Texture* texture)
{
    SkyUBO ubo = { 0 };
    //slog("In draw");
    if (!particle) return;
    gfc_matrix4_copy(ubo.model, modelMat);
    gf3d_vgraphics_get_view(&ubo.view);

    ubo.view[0][3] = 0;
    ubo.view[1][3] = 0;
    ubo.view[2][3] = 0;
    ubo.view[3][0] = 0;
    ubo.view[3][1] = 0;
    ubo.view[3][2] = 0;

    gf3d_vgraphics_get_projection_matrix(&ubo.proj);

    ubo.color = gfc_color_to_vector4f(mod);

    gf3d_particle_queue_render(particle, particle_manager.sky_pipe, &ubo, texture);
}

void gf3d_wire_draw(Mesh* particle, GFC_Matrix4 modelMat, GFC_Color mod, Texture* texture)
{
    WireUBO ubo = { 0 };
    //slog("In draw");
    if (!particle) return;
    gfc_matrix4_copy(ubo.model, modelMat);
    gf3d_vgraphics_get_view(&ubo.view);


    gf3d_vgraphics_get_projection_matrix(&ubo.proj);

    ubo.color = gfc_color_to_vector4f(mod);


    gf3d_particle_queue_render(particle, particle_manager.wire_pipe, &ubo, texture);
}


Pipeline* gf3d_particle_get_pipeline()
{
    return particle_manager.pipe;
}

