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
#include "window.h"
#include "menu.h"

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
    GameState game_state;


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

    //2D init
    window_system_init(100);

    //game init
    srand(SDL_GetTicks());
    slog_sync();
    
    gf2d_mouse_load("actors/mouse.actor");


    //Windows
    Window* main_menu = menu_main_menu(gfc_vector2d(0, 0), GFC_COLOR_RED, "images/ui/main_menu.png");
    Window* pause_menu = menu_pause_menu(gfc_vector2d(450, 200), GFC_COLOR_RED, "images/ui/window_background.png");


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





    while(!_done)
    {
        cmds();
        gfc_input_update();
        gf2d_mouse_update();
        gf2d_font_update();
        theta += .1;

        gfc_matrix4_rotate_z(dinoM, id, theta);

        game_state = game_state_get();

        if (game_state == GS_Play && gfc_input_key_pressed("p"))
        {
            game_state_pause();   // sets GS_Pause
            pause_menu->_visible = 1;
        }

        else if (game_state == GS_Pause && gfc_input_key_pressed("p"))
        {
            game_state_start();  // sets GS_Play
            pause_menu->_visible = 0;
        }



        switch (game_state)
        {
        case GS_MainMenu:
            main_menu->_visible = 1;
            window_system_think_all();
            window_system_update_all();
            gf3d_vgraphics_render_start();
            window_system_draw_all();
            gf2d_font_draw_line_tag("ALT+F4 to exit", FT_H1, GFC_COLOR_WHITE, gfc_vector2d(10, 10));
            gf2d_mouse_draw();
            gf3d_vgraphics_render_end();
            break;

        case GS_Play:
            main_menu->_visible = 0;
            entity_system_think_all();
            power_system_think_all();
            window_system_think_all();


            entity_system_move_all();
            power_system_move_all();

            entity_check_collisions();//, World* world);
            shard_check_collisions();

            entity_system_update_all();

            window_system_update_all();


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

            //2D draws
            energy_bar();
            health_bar();
            window_system_draw_all();
            gf2d_mouse_draw();
            gf3d_vgraphics_render_end();
            break;

        case GS_Pause:
            window_system_think_all();
            window_system_update_all();
            gf3d_vgraphics_render_start();
            //3D draws
            gf3d_mesh_sky_draw(mesh, modelMat, GFC_COLOR_WHITE, texture);
            world_draw(world);
            entity_system_draw_all(lightPos, GFC_COLOR_RED); //Change id to dinoM
            power_system_draw_all(lightPos, GFC_COLOR_BLUE);
            shard_system_draw_all(lightPos, GFC_COLOR_GREEN);

            //2D draws
            energy_bar();
            health_bar();
            window_system_draw_all();
            gf2d_mouse_draw();
            gf3d_vgraphics_render_end();
            break;
        case GS_Quit:
            _done = 1;
            break;
            
        }

        

               
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
