#ifndef __UI_H__
#define __UI_H__
#include "gfc_vector.h"

#include "gf2d_sprite.h"
#include "gfc_color.h"
#include "gfc_shape.h"
#include "gfc_text.h"




typedef struct
{
	Uint8			_inuse;

	Sprite*			texture;
	GFC_Vector2D	position;
	GFC_TextLine	text;
	GFC_Color		color;
} Window;

typedef struct
{
	GFC_Rect	box;
	Sprite*		health;
} HealthBar;

typedef struct
{
	Uint8		_inuse;
	GFC_Rect	box;
	Sprite*		health;
	Uint32		total;
	Uint32		MAX;
} EnergyBar;




void window_system_init(Uint32 max_windows);


void window_system_close();

Window* window_new();

Window* window_main_menu(GFC_Vector2D position, GFC_Color color, const char* filename);

EnergyBar* energy_bar();

#endif

