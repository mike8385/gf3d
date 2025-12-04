#ifndef __UI_H__
#define __UI_H__
#include "gfc_vector.h"

#include "gf2d_sprite.h"
#include "gfc_color.h"
#include "gfc_shape.h"
#include "gfc_text.h"



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




EnergyBar* energy_bar();

#endif

