#include "emu.h"

#include "drivenum.h"

GAME_EXTERN(___empty);
GAME_EXTERN(x68000);
GAME_EXTERN(x68030);
GAME_EXTERN(x68ksupr);
GAME_EXTERN(x68kxvi);

game_driver const *const driver_list::s_drivers_sorted[5] =
{
	&GAME_NAME(___empty),
	&GAME_NAME(x68000),
	&GAME_NAME(x68030),
	&GAME_NAME(x68ksupr),
	&GAME_NAME(x68kxvi),
};

std::size_t const driver_list::s_driver_count = 5;
