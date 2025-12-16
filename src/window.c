#include "simple_logger.h"

#include "window.h"

#include "gfc_input.h"
#include "gf3d_draw.h"
#include "gfc_shape.h"
#include "gfc_vector.h"
#include "player.h"

typedef struct
{
	Window* window_list;
	Uint32	window_max;
}WindowSystem;

static WindowSystem window_system = { 0 };


GameState game_state = GS_MainMenu;

void window_system_init(Uint32 max_windows)
{
	if (!max_windows)
	{
		slog("Cannot init window system with zero windows");
		return;
	}
	window_system.window_list = gfc_allocate_array(sizeof(Window), max_windows);
	if (!window_system.window_list)
	{
		slog("Failed to allocate %i windows for the system", max_windows);
		return;
	}
	window_system.window_max = max_windows;
	atexit(window_system_close);
	slog("Window system initialized");
}

void window_system_close()
{
	int i;
	if (window_system.window_list)
	{
		for (i = 0; i < window_system.window_max; i++)
		{
			if (window_system.window_list[i]._inuse)
			{
				//window_free(&window_system.window_list[i]);
			}
		}
		free(window_system.window_list);
	}
	memset(&window_system, 0, sizeof(WindowSystem));
}

Window* window_new()
{
	int i;
	if (window_system.window_list)
	{
		for (i = 0; i < window_system.window_max; i++)
		{
			if (!window_system.window_list[i]._inuse)
			{
				memset(&window_system.window_list[i], 0, sizeof(Window));   //clears ALL garbage memory
				window_system.window_list[i]._inuse = 1;
				window_system.window_list[i].color = GFC_COLOR_WHITE;
				window_system.window_list[i].buttonList = gfc_list_new();
				slog("window_new() called: slot %d address=%p", i, (void*)&window_system.window_list[i]);

				return &window_system.window_list[i];
			}
		}
	}
	return NULL;
}


void window_free(Window* win)
{
	if (!win) return;
	if (win->free)win->free(win);
	//gf3d_mesh_free(win->mesh);
	gf3d_texture_free(win->texture);
	//NOTE: Free the list
	memset(win, 0, sizeof(Window));
}


void window_draw(Window* win)
{
	if (!win) return;


	GFC_Vector3D drawPos;
	GFC_Matrix4 modelMat;
	//gfc_vector3d_add(drawPos, win->drawOffset, win->position);
	if (!win)
	{
		slog("Couldnt draw window");
		return;
	}

	gf2d_sprite_draw_image(
		win->texture,
		win->position
	);



	if (win->draw) win->draw(win);


}

void window_system_draw_all()
{
	int i;
	for (i = 0; i < window_system.window_max; i++)
	{
		if (window_system.window_list[i]._visible)
		{
			window_draw(&window_system.window_list[i]);
		}
	}
}

void window_think(Window* win)
{
	if (!win) return;

	if (win->think) win->think(win);
}


void window_system_think_all()
{
	//same as draw all but think
	int i;
	for (i = 0; i < window_system.window_max; i++)
	{
		if (window_system.window_list[i]._visible)
		{
			window_think(&window_system.window_list[i]);
		}
	}
}


void window_system_update_all()
{
	int i;
	for (i = 0; i < window_system.window_max; i++)
	{
		if (window_system.window_list[i]._inuse)
		{
			window_update(&window_system.window_list[i]);
		}
	}
}

void window_update(Window* win)
{
	if (!win) return;

	if (win->update) win->update(win);
}


GameState game_state_get()
{
	return game_state;
}

GameState game_state_start()
{
	game_state = GS_Play;
}

GameState game_state_pause()
{
	game_state = GS_Pause;
}

GameState game_state_main_menu()
{
	game_state = GS_MainMenu;
}

GameState game_state_quit()
{
	game_state = GS_Quit;
}

