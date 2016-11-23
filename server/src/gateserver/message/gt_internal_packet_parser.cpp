#include "gt_internal_packet_parser.h"
#include "lp_processerror.h"



INT_32  g_MessageSize[INTERNAL_MESSAGE::max_internal_message_count];

CGTInternalPacketParser::CGTInternalPacketParser()
{
	m_dwRef = 0;
}

CGTInternalPacketParser::~CGTInternalPacketParser()
{
}

void LPAPI CGTInternalPacketParser::AddRef(void)
{
	++m_dwRef;
}

UINT_32 LPAPI CGTInternalPacketParser::QueryRef(void)
{
	return m_dwRef;
}

void LPAPI CGTInternalPacketParser::Release(void)
{
	if (m_dwRef > 0)
	{
		--m_dwRef;
	}
	if (m_dwRef == 0)
	{
		delete this;
	}
}

INT_32 LPAPI CGTInternalPacketParser::Parse(ILPLoopBuf * pLoopBuf)
{
	INT_32 nResult = 0;
	UINT_16 wMsgId = 0;
	INT_32 nParseCount = 0;

	LOG_PROCESS_ERROR(pLoopBuf);

	if (pLoopBuf->GetTotalReadableLen() < sizeof(wMsgId))
	{
		PROCESS_SUCCESS(TRUE);
	}

	nResult = pLoopBuf->Read((char*)(&wMsgId), sizeof(wMsgId), FALSE, FALSE);
	LOG_PROCESS_ERROR(nResult);

	CONVERT_MSG_ID_ENDIAN(wMsgId);

	LOG_PROCESS_ERROR(wMsgId > min_internal_message && wMsgId < max_internal_message);

	if (pLoopBuf->GetTotalReadableLen() >= g_MessageSize[wMsgId])
	{
		nParseCount = g_MessageSize[wMsgId];
	}

	PROCESS_SUCCESS(TRUE);

Exit0:
	nParseCount = -1;

Exit1:
	return nParseCount;
}