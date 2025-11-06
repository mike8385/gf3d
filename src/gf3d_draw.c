#include "simple_logger.h"

#include "simple_json.h"

#include "gf3d_vgraphics.h"
#include "gf3d_obj_load.h"
#include "gfc_color.h"
#include "gf3d_draw.h"

#define DRAW_ATTRIBUTE_COUNT 3

extern int __DEBUG;

typedef struct
{
    Mesh* draw_list;
    Uint32                                  draw_count;
    Uint32                                  chain_length;     /**<length of swap chain*/
    VkDevice                                device;           /**<logical vulkan device*/
    Pipeline* pipe;                         /**<the pipeline associated with draw rendering*/
    VkBuffer                                faceBuffer;
    VkDeviceMemory                          faceBufferMemory;
    VkVertexInputAttributeDescription       attributeDescriptions[DRAW_ATTRIBUTE_COUNT];
    VkVertexInputBindingDescription         bindingDescription;
    float                                   drawOrder;
    Texture* defaultTexture;
} DrawManager;

static DrawManager draw_manager = { 0 };


void gf3d_draw_manager_close()
{
    int i;
    for (i = 0; i < draw_manager.draw_count; i++)
    {
        gf3d_draw_delete(&draw_manager.draw_list[i]);
    }
    if (draw_manager.draw_list)
    {
        free(draw_manager.draw_list);
    }

    memset(&draw_manager, 0, sizeof(DrawManager));
    if (__DEBUG)slog("Draw manager closed");
}


void gf3d_draw_init(Uint32 draw_max)
{
    Uint32 count = 0;
    if (draw_max == 0)
    {
        slog("cannot intilizat draw manager for 0 draws");
        return;
    }
    draw_manager.chain_length = gf3d_swapchain_get_chain_length();
    draw_manager.draw_list = (Mesh*)gfc_allocate_array(sizeof(Mesh), draw_max);
    draw_manager.draw_count = draw_max;
    draw_manager.device = gf3d_vgraphics_get_default_logical_device();


    gf3d_draw_get_attribute_descriptions(&count);
    draw_manager.pipe = gf3d_pipeline_create_from_config(
        gf3d_vgraphics_get_default_logical_device(),
        "config/wire_pipeline.cfg",
        gf3d_vgraphics_get_view_extent(),
        draw_max,
        gf3d_draw_get_bind_description(),
        gf3d_draw_get_attribute_descriptions(NULL),
        count,
        sizeof(WireUBO),
        VK_INDEX_TYPE_UINT16
    );

    draw_manager.defaultTexture = gf3d_texture_load("images/default.png");
    if (__DEBUG)slog("draw manager initiliazed");
    atexit(gf3d_draw_manager_close);
}

void gf3d_draw_delete(Mesh* draw)
{
    int i, c;
    MeshPrimitive* prim;
    if (!draw)return;
    c = gfc_list_count(draw->primitives);
    for (i = 0; i < c; i++)
    {
        prim = gfc_list_get_nth(draw->primitives, i);
        if (!prim) continue;
        gf3d_draw_primitive_free(prim);
    }
    if (draw->primitives) gfc_list_delete(draw->primitives);

    //if (draw->buffer != VK_NULL_HANDLE)
    //{
    //    vkDestroyBuffer(gf2d_sprite.device, sprite->buffer, NULL);
    //}
    //if (sprite->bufferMemory != VK_NULL_HANDLE)
    //{
    //    vkFreeMemory(gf2d_sprite.device, sprite->bufferMemory, NULL);
    //}

   // gf3d_texture_free(draw->texture);
    memset(draw, 0, sizeof(Mesh));
}


Mesh* gf3d_draw_new()
{
    int i;
    for (i = 0; i < draw_manager.draw_count; i++)
    {
        if (draw_manager.draw_list[i]._refCount)continue;
        draw_manager.draw_list[i]._refCount = 1;
        return &draw_manager.draw_list[i];
    }
    slog("gf3d_draw_new: no free slots for new drawes");
    return NULL;
}

Mesh* gf3d_draw_get_by_filename(const char* filename)
{
    int i;
    if (!filename)return NULL;
    for (i = 0; i < draw_manager.draw_count; i++)
    {
        if (!draw_manager.draw_list[i]._refCount)continue;
        if (gfc_line_cmp(draw_manager.draw_list[i].filename, filename) == 0)
        {
            return &draw_manager.draw_list[i];
        }
    }
    return NULL;
}


Mesh* gf3d_draw_load_obj(const char* filename)
{
    MeshPrimitive* primitive;
    Mesh* draw;
    ObjData* obj;

    if (!filename) return NULL;
    //draw = gf3d_draw_get_by_filename(filename);
    //if (draw)
    //{
    //    draw->_refCount++;  //If draw currently exists, grab it and end call
    //    return draw;
    //}
    obj = gf3d_obj_load_from_file(filename); //Parse data from file
    if (!obj)
    {
        slog("Failed to parse obj file %s", filename);
        return NULL;
    }
    draw = gf3d_draw_new();
    if (!draw)
    {
        gf3d_obj_free(obj);
        slog("Failed to make new draw");
        return NULL;
    }

    primitive = gf3d_draw_primitive_new();

    if (!primitive)
    {
        gf3d_obj_free(obj);
        gf3d_draw_free(draw);
        slog("Failed to make new drawPrim");
        return NULL;
    }

    draw->primitives = gfc_list_new();
    gfc_list_append(draw->primitives, primitive);

    primitive->objData = obj;

    gf3d_draw_primitive_create_vertex_buffer(primitive);
    gf3d_draw_primitive_create_face_buffer(primitive);
    gfc_line_cpy(draw->filename, filename);
    //gf3d_draw_create_vertex_buffer(draw);
    return draw;
}


void gf3d_draw_primitive_free(MeshPrimitive* prim)
{

}



MeshPrimitive* gf3d_draw_primitive_new()
{
    MeshPrimitive* prim = NULL;
    prim = (MeshPrimitive*)gfc_allocate_array(sizeof(MeshPrimitive), 1);

    if (!prim)
    {
        slog("Couldnt make a new draw primitive memory");
        return NULL;
    }


    return prim;

}


VkVertexInputAttributeDescription* gf3d_draw_get_attribute_descriptions(Uint32* count)
{
    draw_manager.attributeDescriptions[0].binding = 0;
    draw_manager.attributeDescriptions[0].location = 0;
    draw_manager.attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    draw_manager.attributeDescriptions[0].offset = offsetof(Vertex, vertex);

    draw_manager.attributeDescriptions[1].binding = 0;
    draw_manager.attributeDescriptions[1].location = 1;
    draw_manager.attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    draw_manager.attributeDescriptions[1].offset = offsetof(Vertex, normal);

    draw_manager.attributeDescriptions[2].binding = 0;
    draw_manager.attributeDescriptions[2].location = 2;
    draw_manager.attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
    draw_manager.attributeDescriptions[2].offset = offsetof(Vertex, texel);
    if (count)*count = DRAW_ATTRIBUTE_COUNT;
    return draw_manager.attributeDescriptions;
}

VkVertexInputBindingDescription* gf3d_draw_get_bind_description()
{
    draw_manager.bindingDescription.binding = 0;
    draw_manager.bindingDescription.stride = sizeof(Vertex);
    draw_manager.bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return &draw_manager.bindingDescription;
}

void gf3d_draw_free(Mesh* draw)
{
    if (!draw)return;
    draw->_refCount--;
    if (draw->_refCount <= 0)gf3d_draw_delete(draw);
}


void gf3d_draw_primitive_create_vertex_buffer(MeshPrimitive* prim)
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
        slog("No draw primitize provided");
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


void gf3d_draw_primitive_create_face_buffer(MeshPrimitive* prim)
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
        slog("No draw primitize provided");
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
void gf3d_draw_queue_render(Mesh* draw, Pipeline* pipe, void* uboData, Texture* texture)
{
    int i, c;
    MeshPrimitive* prim;
    if ((!draw) || (!pipe) || (!uboData)) return;
    c = gfc_list_count(draw->primitives);
    for (i = 0; i < c; i++)
    {
        prim = gfc_list_nth(draw->primitives, i);
        if (!prim) continue;
        gf3d_draw_primitive_queue_render(prim, pipe, uboData, texture);
    }


}


void gf3d_draw_primitive_queue_render(MeshPrimitive* prim, Pipeline* pipe, void* uboData, Texture* texture)
{
    if ((!prim) || (!pipe) || (!uboData)) return;
    if (!texture) texture = draw_manager.defaultTexture;
    gf3d_pipeline_queue_render(
        pipe,
        prim->vertexBuffer,
        prim->vertexCount,
        prim->faceBuffer,
        uboData,
        texture);
}




void gf3d_draw_draw(Mesh* draw, GFC_Matrix4 modelMat, GFC_Color mod, Texture* texture, GFC_Vector3D lightPos, GFC_Color lightColor)
{
    WireUBO ubo = { 0 };
    //slog("In draw");
    if (!draw) return;
    gfc_matrix4_copy(ubo.model, modelMat);
    gf3d_vgraphics_get_view(&ubo.view);


    gf3d_vgraphics_get_projection_matrix(&ubo.proj);

    ubo.color = gfc_color_to_vector4f(mod);
    gf3d_draw_queue_render(draw, draw_manager.pipe, &ubo, texture);
}

Pipeline* gf3d_draw_get_pipeline()
{
    return draw_manager.pipe;
}