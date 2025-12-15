
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

	if (gfc_input_key_pressed("4"))
	{
		if (!player)
		{
			slog("No player");
			return;
		}

		cmd_check_player_health(player);
	}

	if (gfc_input_key_pressed("5"))
	{
		if (!player)
		{
			slog("No player");
			return;
		}

		cmd_check_player_energy(player);
	}

	if (gfc_input_key_pressed("6"))
	{
		if (!player)
		{
			slog("No player");
			return;
		}
		player->energy = 100;
		slog("Reset players energy");
	}

	if (gfc_input_key_pressed("7"))
	{
		if (!player)
		{
			slog("No player");
			return;
		}

		cmd_check_player_shards(player);
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

void cmd_check_player_health(Entity* player)
{
	slog("Player Health: %f", player->health);
	slog("Player Max Health: %f", player->maxHealth);

}


void cmd_check_player_energy(Entity* player)
{
	slog("Player Energy: %f", player->energy);
	slog("Player Max Energy: %f", player->maxEnergy);


}

void cmd_check_player_shards(Entity* player)
{
	Uint8 a = 0, b = 0, c = 0, d = 0;
	slog("Player Shards: %d", player_get_shards());

	if (player_get_shards() > 0) a = 1;
	if (player_get_shards() > 1) b = 1;
	if (player_get_shards() > 2) c = 1;
	if (player_get_shards() > 3) d = 1;

	slog("Glide: %d", a);
	slog("Missle: %d", b);
	slog("Dash: %d", c);
	slog("Teleport: %d", d);

}
