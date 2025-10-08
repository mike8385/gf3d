/*


Vulkan: API to



Queues:

Pipelines: Specific Dataflow through the system

2D Graphics Overlay layer pipeline:








Rendering Pipeline Notes:
	-Descriptor set: UBO, Index buffer, Face buffer, and textures and binding locations






	on cpu we are grabbing the obj data

	Make a 


	Things to Remove:
		-Move verticies
		-





IN BLENDER:
	-Select objects you want exported
	-File, export, (wavefront?).obj Also gltf embedded [.gltf]
	-Save a preset: gf3d
	-Must have a UV map
	-Must triangulate faces, write normals, and include UVs
	-Applied modifiers are optional
	-Y forward X left/right Z up and down
	-Save as .png with alpha channel (its the skin)


CODEBASE:
	-Meshload, follow codebase
	--Check if it already exists
	--Check if file exists
	--Make a new one if not
	--Make A new primitive







	I ended up just stealing the code from his repo, but now im getting errors that theres no popeline or whatever;

C:\Users\mrbar\Git Folders\gameframework3D\gf3d\src\game.c:53: gf3d begin

expected to read 6828 characters, but read 6526 instead
 for file config/input.cfg
expected to read 4237 characters, but read 4125 instead
 for file config/overlay_pipeline.cfg
C:\Users\mrbar\Git Folders\gameframework3D\gf3d\src\gf3d_pipeline.c:234: no free pipelines

C:\Users\mrbar\Git Folders\gameframework3D\gf3d\src\gf3d_pipeline.c:427: failed to get memory for a new pipeline

C:\Users\mrbar\Git Folders\gameframework3D\gf3d\src\gf3d_texture.c:77: no free texture space

expected to read 510 characters, but read 483 instead
 for file config/setup.cfg
expected to read 510 characters, but read 483 instead
 for file config/setup.cfg
expected to read 510 characters, but read 483 instead
 for file config/setup.cfg
expected to read 510 characters, but read 483 instead
 for file config/setup.cfg
expected to read 4237 characters, but read 4125 instead
 for file config/overlay_pipeline.cfg
C:\Users\mrbar\Git Folders\gameframework3D\gf3d\src\gf3d_swapchain.c:129: failed to setup frame buffers for pipeline, no pipeline specified

expected to read 1041 characters, but read 995 instead
 for file config/font.cfg
expected to read 472 characters, but read 451 instead
 for file actors/mouse.actor










*/