#ifndef __COMMANDS_H__
#define __COMMANDS_H__

#include <SDL.h>
#include "simple_logger.h"
#include "gfc_config_def.h"
#include "gfc_input.h"
#include "player.h"

//Global data variables for the functions
Entity* player;




/*
* @brief Function in main game loop that allows the commands to be used. Think of it as a 'think' function.
*/
void cmds();

/*
* @brief Gets the players location and sends it to the console
* @param The player entity itself
*/
void cmd_get_player_location(Entity* player);

/*
* @brief Gets the players list of inventory items and sends it to the console
* @param The player entity itself
*/
void cmd_check_player_stopped(Entity* player);

void cmd_check_player_grounded(Entity* player);



#endif


