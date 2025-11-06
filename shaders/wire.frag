#version 450
#extension GL_ARB_separate_shader_objects : enable

//cd into shaders folder
//Run this: & 'C:\Users\mrbar\Git Folders\extraLibraries\VulkanSDK\1.4.321.1\Bin\glslang.exe' -V model.frag -o model_frag.spv

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec2 fragTexCoord;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec4 colorMod;
layout(location = 3) in vec4 worldPosition;


layout(location = 0) out vec4 outColor;


void main()
{

    vec4 texColor = texture(texSampler, fragTexCoord);

    texColor.xyz = texColor.xyz;


    outColor = texColor; 
}
