#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec4 colorMod;

layout(location = 0) out vec4 outColor;


void main()
{

    vec2 uv = gl_PointCoord;

    vec4 texColor = texture(texSampler, uv);

    outColor = texColor * colorMod;
}
