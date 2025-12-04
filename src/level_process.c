#include "simple_logger.h"

#include "level_process.h"


#include "gfc_input.h"


#include "window.h"


typedef struct
{
	Process* process_data;
}ProcessSystem;



static ProcessSystem process_system = { 0 }; /**<Initalize a LOCAL global entity manager*/

void level_process_main_menu()
{
	window_main_menu();


}