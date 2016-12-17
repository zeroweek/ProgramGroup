#include "gt_external_packet_parser.h"
#include "lp_processerror.h"
#include "external_message.h"
#include "lp_messageserializer.h"


using namespace EXTERNAL_MESSAGE;



CGTExternalPacketParser::CGTExternalPacketParser()
{
	m_dwRef = 0;
}

CGTExternalPacketParser::~CGTExternalPacketParser()
{
}

void LPAPI CGTExternalPacketParser::AddRef(void)
{
	++m_dwRef;
}

LPUINT32 LPAPI CGTExternalPacketParser::QueryRef(void)
{
	return m_dwRef;
}

void LPAPI CGTExternalPacketParser::Release(void)
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

LPINT32 LPAPI CGTExternalPacketParser::Parse(ILPLoopBuf * pLoopBuf)
{
	LPINT32 nResult = 0;
	LPINT32 nParseCount = 0;
	LPUINT32 dwHeadSize = 4;
	LPExternalMessageSerializer oMessageSerializer;
	char szPacketBuf[128];

	LOG_PROCESS_ERROR(pLoopBuf);

	if (pLoopBuf->GetTotalReadableLen() < dwHeadSize)
	{
		PROCESS_SUCCESS(TRUE);
	}

	nResult = pLoopBuf->Read(szPacketBuf, dwHeadSize, FALSE, FALSE);
	LOG_PROCESS_ERROR(nResult);

	nResult = oMessageSerializer.Init(NULL, 0, szPacketBuf, dwHeadSize);
	LOG_PROCESS_ERROR(nResult);

	nResult = m_stMessageHead.UnSerialize(&oMessageSerializer);
	LOG_PROCESS_ERROR(nResult);

	LOG_PROCESS_ERROR(m_stMessageHead.wMsgId > min_external_message && m_stMessageHead.wMsgId < max_external_message);

	if (pLoopBuf->GetTotalReadableLen() >= m_stMessageHead.wMsgSize)
	{
		nParseCount = m_stMessageHead.wMsgSize;
	}

	PROCESS_SUCCESS(TRUE);

Exit0:

	nParseCount = -1;

Exit1:

	nResult = oMessageSerializer.UnInit();
	LOG_CHECK_ERROR(nResult);

	return nParseCount;
}
