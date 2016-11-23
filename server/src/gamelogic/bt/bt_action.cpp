#include "bt_action.h"
#include "lp_processerror.h"
#include "bt_manager.h"


using namespace LZPL;



INT_32 bfnTestAction(BT_CUSTOM_CTRL* pCtrl, INT_32 nParam[])
{
	LOG_PROCESS_ERROR(pCtrl);

	return btrvSuccess;
Exit0:
	return btrvError;
}

INT_32 bfnReturnSuccess(BT_CUSTOM_CTRL* pCtrl, INT_32 nParam[])
{
	return btrvSuccess;
}

INT_32 bfnReturnFail(BT_CUSTOM_CTRL* pCtrl, INT_32 nParam[])
{
	return btrvFail;
}


#define REGISTER_BT_FUNC(_type_, _func_, _owner_mask_) \
	{_type_, #_type_, _func_, (INT_16)_owner_mask_}

CBTManager::ACTION_INFO CBTManager::m_ActionInfoList[btntTotal] =
{
	// common
	REGISTER_BT_FUNC(btntInvalid,                          NULL,                         0),

	REGISTER_BT_FUNC(btatSelector,                         NULL,                         btotAll),
	REGISTER_BT_FUNC(btatSequencer,                        NULL,                         btotAll),
	REGISTER_BT_FUNC(btatParallel,                         NULL,                         btotAll),
	REGISTER_BT_FUNC(btatNot,                              NULL,                         btotAll),
	REGISTER_BT_FUNC(btatDebug,                            NULL,                         btotAll),
	REGISTER_BT_FUNC(btatRandom,                           NULL,                         btotAll),
	REGISTER_BT_FUNC(btatRepeat,                           NULL,                         btotAll),
	REGISTER_BT_FUNC(btatBegin,                            NULL,                         btotAll),
	
	REGISTER_BT_FUNC(btatReturnSuccess,                    bfnReturnSuccess,             btotAll),
	REGISTER_BT_FUNC(btatReturnFail,                       bfnReturnFail,                btotAll),
	REGISTER_BT_FUNC(btatTestAction,                       bfnTestAction,                btotAll),
	{0, 0},
};

