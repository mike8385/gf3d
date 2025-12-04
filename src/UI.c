#include "simple_logger.h"

#include "UI.h"

#include "gfc_input.h"
#include "gf3d_draw.h"
#include "gfc_shape.h"
#include "gfc_vector.h"
#include "player.h"


typedef struct
{
	EnergyBar* energy_list;
}EnergySystem;

static EnergySystem energy_system = { 0 };




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
