#ifndef __WINDOW_H__
#define __WINDOW_H__
#include "gfc_vector.h"

#include "gf2d_sprite.h"
#include "gfc_color.h"
#include "gfc_shape.h"
#include "gfc_text.h"


typedef enum
{
	GS_MainMenu = 0,
	GS_Play = 1,
	GS_Pause = 2,
	GS_Quit = 3
} GameState;


typedef struct Window_S
{
	Uint8			_inuse;
	Uint8			_visible;

	Uint32			buttonSelected;
	Uint32			buttonMax;

	Sprite* texture;
	GFC_Vector2D	position;
	GFC_TextLine	text;
	GFC_Vector2D	textPosition;

	GFC_Color		color;
	GFC_Color		hoverColor;

	GFC_List* buttonList;

	void							(*draw)(struct Window_S* self);
	void							(*think)(struct Window_S* self);
	void							(*update)(struct Window_S* self);
	void							(*move)(struct Window_S* self);
	void							(*free)(struct Window_S* self);

} Window;



void window_system_init(Uint32 max_windows);


void window_system_close();

Window* window_new();

void window_system_init(Uint32 max_windows);

void window_free(Window* win);

void window_system_close();

void window_draw(Window* win);

void window_system_draw_all();

void window_think(Window* win);

void window_system_think_all();

void window_system_update_all();

void window_update(Window* win);

GameState game_state_get();

GameState game_state_start();

GameState game_state_pause();

GameState game_state_main_menu();

GameState game_state_quit();


#endif

