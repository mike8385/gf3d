#include "simple_logger.h"

#include "UI.h"

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

typedef struct
{
	EnergyBar* energy_list;
}EnergySystem;

static EnergySystem energy_system = { 0 };


void window_system_init(Uint32 max_windows)
{
	if (!max_windows)
	{
		slog("Cannot init window system with zero windows");
		return ;
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

Window* window_main_menu(GFC_Vector2D position, GFC_Color color, const char* filename)
{
	Window* self;
	Sprite* texture;
	self = window_new();
	if (!self) return NULL;
	texture = gf2d_sprite_load_image(filename);
	if (!texture)
	{
		slog("Couldnt load texture");
		return NULL;
	}
	self->position = position;
	self->color = color;
	self->texture = texture;

	//gf2d_sprite_draw(texture , position, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

	return self;
}

//Energy Bar Code:

EnergyBar* energy_bar_init()
{
	energy_system.energy_list = gfc_allocate_array(sizeof(EnergyBar), 1);
	if (!window_system.window_list)
	{
		slog("Failed to allocate energybar windows for the system");
		return NULL;
	}
	//atexit(energy_system_close);
	slog("Window system initialized");	
}

void energy_system_close()
{
	int i;
	if (energy_system.energy_list)
	{
		if (energy_system.energy_list->_inuse)
		{
				//window_free(&window_system.window_list[i]);
		}
		free(energy_system.energy_list);
	}
	memset(&energy_system, 0, sizeof(EnergySystem));
}

EnergyBar* energy_new()
{
	int i;
	if (energy_system.energy_list)
	{
		if (!energy_system.energy_list[i]._inuse)
		{
			memset(&energy_system.energy_list[i], 0, sizeof(EnergyBar));   //clears ALL garbage memory
			energy_system.energy_list[i]._inuse = 1;
			return &energy_system.energy_list[i];
		}
		
	}
	return NULL;
}

EnergyBar* energy_bar()
{
	Sprite* sprite;
	Entity* player;

	player = player_get_player();
	if (!player) return NULL;
	//EnergyBar* energyBar;

	//energyBar = energy_new();
	//if (!energyBar) return;
	//energyBar->_inuse = 1;
	//energyBar->box = gfc_rect(0, 5, 50, 50);
	GFC_Vector4D crop;
	GFC_Vector4D colorCrop;
	GFC_Color	barColor;

	crop = gfc_vector4d(0, 0, 0, 70);
	barColor = GFC_COLOR_BLUE;
	colorCrop = gfc_vector4d(player->energy*2, 0, 0, 70);

	sprite = gf2d_sprite_load_image("images/ui/healthbar.png");
	gf2d_sprite_draw(sprite, gfc_vector2d(0, 35), NULL, NULL, NULL, NULL, &barColor, &crop, NULL);
	gf2d_sprite_draw(sprite, gfc_vector2d(0, 35), NULL, NULL, NULL, NULL, NULL, &colorCrop, NULL);



	
}

/*
* void UI_health_bar(GFC_Rect box)
{

	Entity* player;
	float playerHealth = get_player_health();
	Sprite* sprite;
	sprite = gf2d_sprite_load_image("images/UI/fullhealth.png");
	gf2d_sprite_draw_image(sprite, gfc_vector2d(box.x, box.y));
	gf2d_draw_rect(box, GFC_COLOR_BLACK);
*/
