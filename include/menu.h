#ifndef __MENU_H__
#define __MENU_H__
#include "gfc_vector.h"

#include "gf2d_sprite.h"
#include "gfc_color.h"
#include "gfc_shape.h"
#include "gfc_text.h"
#include "window.h"




typedef struct
{
	GFC_TextLine name;
	GFC_TextLine text;
	GFC_TextLine command; //Maybe name it action
	GFC_Vector2D positon;
	GFC_Color	buttonColor;
	GFC_Color	hoverColor;
	GFC_Color	color;
} WindowButtons;



Window* menu_new();

Window* menu_main_menu(GFC_Vector2D position, GFC_Color color, const char* filename);

Window* menu_pause_menu(GFC_Vector2D position, GFC_Color color, const char* filename);


void menu_free(Window* win);

void menu_think(Window* win);

void menu_update(Window* win);

void menu_draw(Window* self);

void menu_command(Window* self);


#endif

