#include "simple_logger.h"

#include "window.h"


typedef struct
{
	Window* window_list;
	Uint32	window_max;
}WindowSystem;

static WindowSystem window_system = { 0 };



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
				return &window_system.window_list[i];
			}
		}
	}
	return NULL;
}

Window* window_main_menu()
{
	Window* self;
	Sprite* texture;
	GFC_Vector2D scale = gfc_vector2d(100, 100);
	self = window_new();
	if (!self) return NULL;

	
	texture = gf2d_sprite_load_image("images/ui/window_background.png");
	if (!texture)
	{
		slog("Couldnt load texture");
		return NULL;
	}

	self->texture = texture;


	gf2d_sprite_draw(texture, gfc_vector2d(0,0), &scale, NULL, NULL, NULL, NULL, NULL, NULL);

	return self;
}
