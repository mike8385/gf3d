#ifndef __LEVEL_PROCESS_H__
#define __LEVEL_PROCESS_H__

#include "gfc_vector.h"


typedef enum {
	MAIN_MENU = 1,
	LEVEL_LOAD = 2,
	LEVEL_CLEANUP = 3,
	LEVEL_LEVEL = 4
} ProcessNumber;


typedef struct
{
	ProcessNumber*		processNumber;

} Process;


void level_process_main_menu();


#endif

