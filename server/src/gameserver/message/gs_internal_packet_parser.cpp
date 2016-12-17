#include "gs_internal_packet_parser.h"
#include "lp_processerror.h"



LPINT32  g_MessageSize[INTERNAL_MESSAGE::max_internal_message_count];

CGSInternalPacketParser::CGSInternalPacketParser()
{
	m_dwRef = 0;
}

CGSInternalPacketParser::~CGSInternalPacketParser()
{
}

void LPAPI CGSInternalPacketParser::AddRef(void)
{
	++m_dwRef;
}

LPUINT32 LPAPI CGSInternalPacketParser::QueryRef(void)
{
	return m_dwRef;
}

void LPAPI CGSInternalPacketParser::Release(void)
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

LPINT32 LPAPI CGSInternalPacketParser::Parse(ILPLoopBuf * pLoopBuf)
{
	LPINT32 nResult = 0;
	LPUINT16 wMsgId = 0;
	LPINT32 nParseCount = 0;

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
