#include "bt_event.h"
#include "lp_processerror.h"



char const* const g_BTEventName[btetTotal] =
{
	"btetInvalid",

	"btetAIInit",
	"btetAIIdle",
	"btetAIFight",
	"btetAIDie",
	"btetAIRevive",
	"btetAIInteract",
};

