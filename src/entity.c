//#include "simple_logger.h"
//
//#include "entity.h"
//
//
//typedef struct Entity_S
//{
//	Entity* entity_list;
//	Uint32	entity_max;
//}EntitySystem;
//
//
//
//Entity* entity_new()
//{
//
//}
//
//
//void entity_free(Entity* ent);
//
//
//void entity_system_close()
//{
//	int i;
//	if (entity_system.entity_list)
//	{
//		for (i = 0; i < entity_system.entity_max; i++)
//		{
//			//entity_free()
//		}
//	}
//}
//
//void entity_system_init(Uint32 max_ents)
//{
//	if (!max_ents)
//	{
//		slog("Cannot init entity system with zero ents");
//		return;
//	}
//	entity_system_entity_list = gfc_allocate_array(sizeof(Entity), max_ents);
//	if (!entity_system_entity_list)
//	{
//		slog("");
//		return;
//	}
//}
//
//
//void entity_system_draw_all();
//
//
//void entity_system_think_all();
//
//
//void entity_system_update_all();
//
//#endif