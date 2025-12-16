#include "simple_logger.h"

#include "UI.h"

#include "gfc_input.h"
#include "gf3d_draw.h"
#include "gfc_shape.h"
#include "gfc_vector.h"
#include "player.h"


typedef struct
{
	UIBar* energy_list;
}EnergySystem;

static EnergySystem energy_system = { 0 };


static Sprite* ui_bar_sprite = NULL;


//Energy Bar Code:

UIBar* energy_bar_init()
{
	energy_system.energy_list = gfc_allocate_array(sizeof(UIBar), 1);
	if (!energy_system.energy_list)
	{
		slog("Failed to allocate energybar windows for the system");
		return NULL;
	}
	//atexit(energy_system_close);
	slog("Window system initialized");	
}

//void energy_system_close()
//{
//	int i;
//	if (energy_system.energy_list)
//	{
//		if (energy_system.energy_list->_inuse)
//		{
//				//window_free(&window_system.window_list[i]);
//		}
//		free(energy_system.energy_list);
//	}
//	memset(&energy_system, 0, sizeof(EnergySystem));
//}

//UIBar* energy_new()
//{
//	int i;
//	if (energy_system.energy_list)
//	{
//		if (!energy_system.energy_list[i]._inuse)
//		{
//			memset(&energy_system.energy_list[i], 0, sizeof(UIBar));   //clears ALL garbage memory
//			energy_system.energy_list[i]._inuse = 1;
//			return &energy_system.energy_list[i];
//		}
//		
//	}
//	return NULL;
//}

UIBar* energy_bar()
{
	Sprite* sprite;
	Entity* player;

	player = player_get_player();
	if (!player) return NULL;
	//UIBar* energyBar;

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

	if (!ui_bar_sprite)
	{
		ui_bar_sprite = gf2d_sprite_load_image("images/ui/healthbar.png");
	}

	gf2d_sprite_draw(ui_bar_sprite, gfc_vector2d(0, 35), NULL, NULL, NULL, NULL, &barColor, &crop, NULL);
	gf2d_sprite_draw(ui_bar_sprite, gfc_vector2d(0, 35), NULL, NULL, NULL, NULL, NULL, &colorCrop, NULL);



	
}

UIBar* health_bar()
{
	Sprite* sprite;
	Entity* player;

	player = player_get_player();
	if (!player) return NULL;
	//UIBar* energyBar;

	//energyBar = energy_new();
	//if (!energyBar) return;
	//energyBar->_inuse = 1;
	//energyBar->box = gfc_rect(0, 5, 50, 50);
	GFC_Vector4D crop;
	GFC_Vector4D colorCrop;
	GFC_Color	barColor;

	crop = gfc_vector4d(0, 0, 0, 70);
	barColor = GFC_COLOR_RED;
	colorCrop = gfc_vector4d(player->health * 2, 0, 0, 70);

	if (!ui_bar_sprite)
	{
		ui_bar_sprite = gf2d_sprite_load_image("images/ui/healthbar.png");
	}
	gf2d_sprite_draw(ui_bar_sprite, gfc_vector2d(0, 55), NULL, NULL, NULL, NULL, &barColor, &crop, NULL);
	gf2d_sprite_draw(ui_bar_sprite, gfc_vector2d(0, 55), NULL, NULL, NULL, NULL, NULL, &colorCrop, NULL);




}
