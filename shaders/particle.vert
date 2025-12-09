#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject
{
    mat4    model;
    mat4    view; //Cameras Orientation + position
    mat4    proj; //Projection matrix (Persepective or orthographic)
    vec4    viewportSize;   // add this to your UBO, what does it do?
    vec4    color;
    float   size;
    float   padding;
} ubo;

out gl_PerVertex
{
    vec4 gl_Position;
    float gl_PointSize;
};

layout(location = 0) in vec3 inPosition;

layout(location = 0) out vec4 colorMod;
layout(location = 1) out float outSize; //World-Space particle radius
layout(location = 2) out vec2 outCenter; //World-Space particle radius






void main()
{
//Convert "world radius" -> Pixel Radius: pixel_size = viewport_height_pixels * world_size / depth

    mat4 mpv = ubo.proj * ubo.view * ubo.model;
    gl_Position = mpv * vec4(inPosition, 1.0);

    outCenter = (0.5 * gl_Position.xy/gl_Position.w + 0.5) * ubo.viewportSize.xy;
    gl_PointSize = ubo.viewportSize.y * ubo.size / gl_Position.w;
    outSize = gl_PointSize/2.0;

    colorMod = ubo.color;

    //if (texColor.a < 0.01) discard;

}
