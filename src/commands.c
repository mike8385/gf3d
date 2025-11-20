
#include "simple_logger.h"

#include "gfc_vector.h"
#include "gfc_input.h"
#include "commands.h"
#include "player.h"


Entity* player;

void cmds()
{
	player = player_get_player();
	
	
	if (gfc_input_key_pressed("1"))
	{
		if (!player)
		{
			slog("No player");
			return;
		}

		cmd_get_player_location(player);
	}

	if (gfc_input_key_pressed("2"))
	{
		if (!player)
		{
			slog("No player");
			return;
		}

		cmd_check_player_stopped(player);
	}

	if (gfc_input_key_pressed("3"))
	{
		if (!player)
		{
			slog("No player");
			return;
		}

		cmd_check_player_grounded(player);
	}


}


void cmd_get_player_location(Entity* player)
{
	slog("Player position: [%f, %f, %f]  X: %f Y: %f Z: %f", player->position.x, player->position.y, player->position.z, player->position.x, player->position.y, player->position.z);
}

void cmd_check_player_stopped(Entity* player)
{
	slog("Player stopped: %d", player->stopped);
}

void cmd_check_player_grounded(Entity* player)
{
	slog("Player grounded: %d", player->onGround);
}
