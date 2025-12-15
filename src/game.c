#include <SDL.h>    //Precompiled     
#include <SDL_net.h>

#include "simple_json.h"
#include "simple_logger.h"

#include "gfc_input.h"
#include "gfc_config_def.h"
#include "gfc_vector.h"
#include "gfc_matrix.h"
#include "gfc_audio.h"
#include "gfc_string.h"
#include "gfc_actions.h"

#include "gf2d_sprite.h"
#include "gf2d_font.h"
#include "gf2d_actor.h"
#include "gf2d_mouse.h"

#include "gf3d_vgraphics.h"
#include "gf3d_pipeline.h"
#include "gf3d_swapchain.h"

#include "gf3d_mesh.h"
#include "entity.h"
#include "monster.h"
#include "world.h"
#include "player.h"
#include "space.h"
#include "particles.h"
#include "power.h"
#include "shards.h"

#include "camera_entity.h"
#include "gf3d_camera.h"
#include "UI.h"

#include "commands.h"

extern int __DEBUG;

static int _done = 0;
static Uint32 frame_delay = 33;
static float fps = 0;

void parse_arguments(int argc,char *argv[]);
void game_frame_delay();

void exitGame()
{
    _done = 1;
}


int main(int argc,char *argv[])
{     
    //local variables
    Mesh* mesh;
    //Mesh* cube;
    Texture* texture;
    Entity* monster;
    Entity* player;
    Power* power;

    Particle* particle;

    //World Variables
    Entity* entityCam;
    World* world;
    Sprite* bg;

    //Space variables
    Space* space;

    float theta = 0;
    GFC_Vector3D lightPos = { 5,5,50 };
    GFC_Vector3D cam = { 0,75,25 };
    GFC_Matrix4 id, dinoM;
    GFC_Matrix4 modelMat;
    GFC_Matrix4 modelMatCube;

    //initializtion    
    parse_arguments(argc,argv);
    init_logger("gf3d.log",0); //1 wont delete log file at end
    slog("gf3d begin");

    //gfc init
    gfc_input_init("config/input.cfg");
    gfc_config_def_init();
    gfc_action_init(1024);

    //gf3d init
    gf3d_vgraphics_init("config/setup.cfg");
    gf2d_font_init("config/font.cfg");
    gf2d_actor_init(1000);
    
    entity_system_init(8000);
    power_system_init(1024);
    shard_system_init(1024);
    //game init
    srand(SDL_GetTicks());
    slog_sync();
    bg = gf2d_sprite_load_image("images/bg_flat.png");
    gf2d_mouse_load("actors/mouse.actor");




    // main game loop    
    gfc_matrix4_identity(modelMat);
    //modelMat[3][2] = 5;
    gfc_matrix4_identity(id);
    //gf3d_camera_look_at(gfc_vector3d(0, 0, 0), &cam);
    mesh = gf3d_mesh_load_obj("models/sky/sky.obj");
    //slog("%f, %f, %f", gf3d_camera_get_position().x, gf3d_camera_get_position().y, gf3d_camera_get_position().z);


    texture = gf3d_texture_load("models/sky/sky.png");

    space = space_load();


    world = world_load("def/cityTerrain.json");
    monster = monster_spawn(gfc_vector3d(5, -15, 10), GFC_COLOR_WHITE);
    player = player_spawn(gfc_vector3d(0, 0, 10), GFC_COLOR_PINK);
    //world_entity_building_spawn(gfc_vector3d(50, 50, 0), GFC_COLOR_RED);
    //cube = gf3d_mesh_load_obj("models/box.obj");
    //power = power_spawn(gfc_vector3d(0, 75, 25), GFC_COLOR_PINK);

    Window* window;
    window = window_main_menu(gfc_vector2d(0,0),GFC_COLOR_RED, "images/ui/window_background.png");

   // particle = gf3d_particle_load2(gfc_vector3d(0, 75, 25));
    //Particle* particle2 = gf3d_particle_load2(gfc_vector3d(12, 4, 10));
    //Particle* particle3 = gf3d_particle_load2(gfc_vector3d(111, 23, 0));


    //printf("sizeof(GFC_Matrix4) = %zu\n", sizeof(GFC_Matrix4));
    //printf("sizeof(GFC_Vector4D) = %zu\n", sizeof(GFC_Vector4D));
    //printf("sizeof(MeshUBO) = %zu\n", sizeof(MeshUBO));
    //particle = gf3d_particle_new();
    camera_entity_spawn(cam, player);

    //


    while(!_done)
    {
        cmds();
        gfc_input_update();
        gf2d_mouse_update();
        gf2d_font_update();
        theta += .1;

        gfc_matrix4_rotate_z(dinoM, id, theta);
        entity_system_think_all();
        power_system_think_all();
        //shard_system_think_all();
        //space_run(space);

        entity_system_move_all();
        power_system_move_all();

        entity_check_collisions();//, World* world);
        shard_check_collisions();

        entity_system_update_all();
        //power_system_update_all();
        //shard_system_update_all();

        //update_space(world)
        
        //camera updates
        gf3d_camera_update_view();

        gf3d_vgraphics_render_start();

                //3D draws
                gf3d_mesh_sky_draw(mesh, modelMat, GFC_COLOR_WHITE, texture);
                world_draw(world);
                ////gf3d_wire_draw(cube,);
                entity_system_draw_all(lightPos, GFC_COLOR_RED); //Change id to dinoM
                power_system_draw_all(lightPos, GFC_COLOR_BLUE);
                shard_system_draw_all(lightPos, GFC_COLOR_GREEN);

                //Particle Draw
                //gf3d_particle_draw(particle, modelMat, GFC_COLOR_BLUE, NULL);
                //gf3d_particle_draw(particle2, modelMat, GFC_COLOR_RED, NULL);
                //gf3d_particle_draw(particle3, modelMat, GFC_COLOR_GREEN, NULL);

                //slog("particle location: %f,%f,%f", particle->position.x, particle->position.y, particle->position.z);

                //2D draws
                energy_bar();
                health_bar();

                //gf2d_sprite_draw_image(window, gfc_vector2d(0,0));
                gf2d_font_draw_line_tag("ALT+F4 to exit",FT_H1,GFC_COLOR_WHITE, gfc_vector2d(10,10));
                gf2d_mouse_draw();
        gf3d_vgraphics_render_end();
        if (gfc_input_command_down("exit"))_done = 1; // exit condition
        game_frame_delay();
    }    
    vkDeviceWaitIdle(gf3d_vgraphics_get_default_logical_device());    
    //cleanup
    slog("gf3d program end");
    SDLNet_Quit();
    exit(0);
    slog_sync();
    return 0;
}

void parse_arguments(int argc,char *argv[])
{
    int a;

    for (a = 1; a < argc;a++)
    {
        if (strcmp(argv[a],"--debug") == 0)
        {
            __DEBUG = 1;
        }
    }    
}


void game_frame_delay()
{
    Uint32 diff;
    static Uint32 now;
    static Uint32 then;
    then = now;
    slog_sync();// make sure logs get written when we have time to write it
    now = SDL_GetTicks();
    diff = (now - then);
    if (diff < frame_delay)
    {
        SDL_Delay(frame_delay - diff);
    }
    fps = 1000.0/MAX(SDL_GetTicks() - then,0.001);
//     slog("fps: %f",fps);
}
/*eol@eof*/
