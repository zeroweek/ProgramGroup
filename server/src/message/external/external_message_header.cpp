#include "external_message_header.h"
#include "lp_processerror.h"


//begin�������������ֿռ�
NS_EXTERNAL_MESSAGE_BEGIN



MESSAGE_SERIALIZE_BEGIN(EXTERNAL_MESSAGE_HEADER)
MESSAGE_WRITE_NUM_VALUE(Uint16, wMsgId)
MESSAGE_WRITE_NUM_VALUE(Uint16, 4)
MESSAGE_SERIALIZE_END


MESSAGE_UNSERIALIZE_BEGIN(EXTERNAL_MESSAGE_HEADER)
MESSAGE_READ_NUM_VALUE(Uint16, wMsgId)
MESSAGE_READ_NUM_VALUE(Uint16, wMsgSize)
MESSAGE_UNSERIALIZE_END








//end�������������ֿռ�
NS_EXTERNAL_MESSAGE_END