#include "simple_logger.h"

#include "menu.h"

#include "gfc_input.h"
#include "gf3d_draw.h"
#include "gfc_shape.h"
#include "gfc_vector.h"
#include "player.h"
#include "gfc_text.h"
#include "gf2d_font.h"


Window* menu_main_menu(GFC_Vector2D position, GFC_Color color, const char* filename)
{
	Window* menu;
	WindowButtons* start_button = gfc_allocate_array(sizeof(WindowButtons), 1);
	WindowButtons* quit_button = gfc_allocate_array(sizeof(WindowButtons), 1);
	//WindowButtons* start_button = gfc_allocate_array(sizeof(WindowButtons), 1);


	menu = window_new();
	if (!menu)
	{
		slog("Cannot make a new menu");
		return NULL;
	}
	if (!filename)
	{
		slog("No filename, assigning default texture/sprite");
		menu->texture = gf2d_sprite_load_image("images/bg_flat.png");
	}
	else
	{
		menu->texture = gf2d_sprite_load_image(filename);
	}

	if (!menu->texture)
	{
		slog("Failed to load menu texture, using fallback");
		menu->texture = gf2d_sprite_load_image("images/bg_flat.png");
	}

	menu->draw = menu_draw;
	menu->update = menu_update;
	menu->think = menu_think;

	menu->position = position;
	menu->_visible = 1;
	gfc_line_cpy(menu->text, "Electric Guy: The Game");
	menu->textPosition = gfc_vector2d(400, 25);
	
	//Buttons:
	gfc_line_cpy(start_button->text, "Start Game");
	gfc_line_cpy(start_button->command, "start");
	start_button->positon = gfc_vector2d(600, 250);
	start_button->buttonColor = GFC_COLOR_BLACK;
	start_button->hoverColor = GFC_COLOR_YELLOW;
	start_button->color = start_button->buttonColor;
	gfc_list_append(menu->buttonList, start_button);

	gfc_line_cpy(quit_button->text, "Quit Game");
	gfc_line_cpy(quit_button->command, "exit");
	quit_button->positon = gfc_vector2d(600, 300);
	quit_button->buttonColor = GFC_COLOR_BLACK;
	quit_button->hoverColor = GFC_COLOR_YELLOW;
	quit_button->color = quit_button->buttonColor;
	gfc_list_append(menu->buttonList, quit_button);

	menu->buttonMax = gfc_list_get_count(menu->buttonList);
	menu->buttonSelected = gfc_list_get_item_index(menu->buttonList, start_button);


	return menu;
}

Window* menu_pause_menu(GFC_Vector2D position, GFC_Color color, const char* filename)
{
	Window* menu;
	WindowButtons* continue_button = gfc_allocate_array(sizeof(WindowButtons), 1);
	WindowButtons* back_button = gfc_allocate_array(sizeof(WindowButtons), 1);
	//WindowButtons* continue_button = gfc_allocate_array(sizeof(WindowButtons), 1);


	menu = window_new();
	if (!menu)
	{
		slog("Cannot make a new menu");
		return NULL;
	}
	if (!filename)
	{
		slog("No filename, assigning default texture/sprite");
		menu->texture = gf2d_sprite_load_image("images/bg_flat.png");
	}
	else
	{
		menu->texture = gf2d_sprite_load_image(filename);
	}

	if (!menu->texture)
	{
		slog("Failed to load menu texture, using fallback");
		menu->texture = gf2d_sprite_load_image("images/bg_flat.png");
	}

	menu->draw = menu_draw;
	menu->update = menu_update;
	menu->think = menu_think;

	menu->position = position;
	menu->_visible = 0;
	gfc_line_cpy(menu->text, "Paused");
	menu->textPosition = gfc_vector2d(530, 240);

	//Buttons:
	gfc_line_cpy(continue_button->text, "Resume");
	gfc_line_cpy(continue_button->command, "resume");
	continue_button->positon = gfc_vector2d(530, 300);
	continue_button->buttonColor = GFC_COLOR_BLACK;
	continue_button->hoverColor = GFC_COLOR_YELLOW;
	continue_button->color = continue_button->buttonColor;
	gfc_list_append(menu->buttonList, continue_button);

	gfc_line_cpy(back_button->text, "Quit Game");
	gfc_line_cpy(back_button->command, "quit");
	back_button->positon = gfc_vector2d(520, 380);
	back_button->buttonColor = GFC_COLOR_BLACK;
	back_button->hoverColor = GFC_COLOR_YELLOW;
	back_button->color = back_button->buttonColor;
	gfc_list_append(menu->buttonList, back_button);

	menu->buttonMax = gfc_list_get_count(menu->buttonList);
	menu->buttonSelected = gfc_list_get_item_index(menu->buttonList, continue_button);


	return menu;
}

void menu_free(Window* self)
{
	if (!self) return;


}


void menu_think(Window* self)
{
	if (!self) return;

	//for (int i = 0; i < self->buttonMax; i++)
	//{

	//}
	if (self->_visible)
	{
		if (gfc_input_key_pressed("w"))
		{
			self->buttonSelected--;
		}
		else if (gfc_input_key_pressed("s"))
		{
			self->buttonSelected++;
		}

		if (self->buttonSelected >= self->buttonMax)
		{
			self->buttonSelected = 0;
		}
		if (self->buttonSelected < 0)
		{
			self->buttonSelected = self->buttonMax - 1;
		}

		if (gfc_input_command_pressed("enter"))
		{
			menu_command(self);
		}
	}



}


void menu_update(Window* self)
{
	if (!self) return;

	for (int i = 0; i < self->buttonMax; i++)
	{
		WindowButtons* btn = gfc_list_get_nth(self->buttonList, i);
		if (!btn) return;
		if (i == self->buttonSelected)
		{
			btn->color = btn->hoverColor;
		}
		else
		{
			btn->color = btn->buttonColor;
		}
	}

}

void menu_draw(Window* self)
{
	if (!self) return;

	for (int i = 0; i < self->buttonMax; i++)
	{
		WindowButtons* btn;
		btn = gfc_list_get_nth(self->buttonList, i);
		if (btn->text != NULL)
		{
			gf2d_font_draw_line_tag(btn->text, FT_H1, btn->color, btn->positon);
		}

	}

	if (self->text)
	{
		gf2d_font_draw_line_tag(self->text, FT_H1, GFC_COLOR_BLACK, self->textPosition);

	}



}


void menu_command(Window* self)
{
	if (!self) return;
	int i;

	if (self->buttonSelected < 0 || self->buttonSelected >= self->buttonMax)
	{
		slog("menu_command: invalid button index %d", self->buttonSelected);
		return;
	}

	WindowButtons* btn = gfc_list_get_nth(self->buttonList, self->buttonSelected);

	if (strcmp(btn->command, "start") == 0)
	{
		game_state_start();
		self->_visible = 0;
	}

	if (strcmp(btn->command, "resume") == 0)
	{
		game_state_start();
		self->_visible = 0;

	}

	if (strcmp(btn->command, "quit") == 0)
	{
		game_state_main_menu();
		self->_visible = 0;
	}

	if (strcmp(btn->command, "exit") == 0)
	{
		game_state_quit();
		self->_visible = 0;
	}

	/*for (i = 0; i < self->buttonMax; i++)
	{
		
	}*/

	
}

