#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject
{
    mat4    view; //Cameras Orientation + position
    mat4    proj; //Projection matrix (Persepective or orthographic)
    vec2    viewportSize;   // add this to your UBO
} ubo;

out gl_PerVertex
{
    vec4 gl_Position;
    float gl_PointSize;
};

layout(location = 0) in vec3 inPosition;
layout(location = 1) in float inSize; //World-Space particle radius
layout(location = 2) in vec4 inColor;


layout(location = 0) out vec4 colorMod;


void main()
{
//Convert "world radius" -> Pixel Radius: pixel_size = viewport_height_pixels * world_size / depth

    mat4 pv = ubo.proj * ubo.view;
    gl_Position = pv * vec4(inPosition, 1.0);

    gl_PointSize = ubo.viewportSize.y * inSize / gl_Position.w;

    colorMod = inColor;

    //if (texColor.a < 0.01) discard;

}
