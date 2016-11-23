#include "http_message_handler.h"
#include "lp_processerror.h"
#include "json/json.h"
#include "lp_dump.h"
#include "curl/curl.h"
#include "lp_urlcode.h"



static char g_szHeaders[] =
"HTTP/1.0 200 OK\r\n"\
"Server: jdbhttpd/0.1.0\r\n"\
"Content-Type: text/html\r\n"\
"\r\n"
"<HTML><TITLE>index.html</TITLE>\r\n"\
"<BODY><P>Welcome to here !</P>\r\n"\
"</BODY></HTML>\r\n";
static UINT_32 g_dwHeadersLen = (UINT_32)strlen(g_szHeaders);

static char g_szNotFound[] =
"HTTP/1.0 404 NOT FOUND\r\n"\
"Server: jdbhttpd/0.1.0\r\n"\
"Content-Type: text/html\r\n"\
"\r\n"\
"<HTML><TITLE>Not Found</TITLE>\r\n"\
"<BODY><P>The server could not fulfill\r\n"\
"your request because the resource specified\r\n"\
"is unavailable or nonexistent.</P>\r\n"\
"</BODY></HTML>\r\n";
static UINT_32 g_dwNotFoundLen = (UINT_32)strlen(g_szNotFound);

static char g_szBadRequest[] =
"HTTP/1.0 400 BAD REQUEST\r\n"\
"Content-type: text/html\r\n"\
"\r\n"\
"<P>Your browser sent a bad request, "\
"such as a POST without a Content-Length.</P>\r\n";
static UINT_32 g_dwBadRequestLen = (UINT_32)strlen(g_szBadRequest);

static char g_szCanNotExcute[] =
"HTTP/1.0 500 Internal Server Error\r\n"\
"Content-type: text/html\r\n"\
"\r\n"\
"<P>Error prohibited CGI execution.</P>\r\n";
static UINT_32 g_dwCanNotExcuteLen = (UINT_32)strlen(g_szCanNotExcute);

static char g_szUnImplemented[] =
"HTTP/1.0 501 Method Not Implemented\r\n"\
"Server: jdbhttpd/0.1.0\r\n"\
"Content-Type: text/html\r\n"\
"\r\n"\
"<HTML><HEAD><TITLE>Method Not Implemented\r\n"\
"</TITLE></HEAD>\r\n"\
"<BODY><P>HTTP request method not supported.</P>\r\n"\
"</BODY></HTML>\r\n";
static UINT_32 g_dwUnImplementedLen = (UINT_32)strlen(g_szUnImplemented);



LPHttpObject::LPHttpObject()
{
	m_pSocker = NULL;
	SetHaveUnResponseRequest(FALSE);
}

LPHttpObject::~LPHttpObject()
{
	UnInit();
}

BOOL LPAPI LPHttpObject::Init(ILPSocker * pSocker)
{
	LOG_PROCESS_ERROR(pSocker);
	m_pSocker = pSocker;

	return TRUE;
Exit0:
	return FALSE;
}

BOOL LPAPI LPHttpObject::UnInit(void)
{
	m_pSocker = NULL;

	return TRUE;
}

void LPAPI LPHttpObject::Close(void)
{
	LOG_PROCESS_ERROR(m_pSocker);
	m_pSocker->Close(SOCK_ERR_CODE(eSockErrCode_Success, 1, 0), FALSE);

Exit0:
	return;
}

ILPSocker *LPAPI LPHttpObject::GetSocker(void)
{
	return m_pSocker;
}

BOOL LPAPI LPHttpObject::HaveUnResponseRequest(void)
{
	return m_bHaveUnResponseRequest;
}

void LPAPI LPHttpObject::SetHaveUnResponseRequest(BOOL bHave)
{
	m_bHaveUnResponseRequest = bHave;
}

void LPAPI LPHttpObject::ParseRequest(const char * szInputBuf, UINT_32 dwInputLen, char * szOutputBuf, UINT_32 & szOutputLen)
{
	return;
}

void LPAPI LPHttpObject::DoResponse_NotFound(void)
{
	INT_32 nResult = 0;

	LOG_PROCESS_ERROR(m_pSocker);
	nResult = m_pSocker->Send(g_szNotFound, g_dwNotFoundLen);
	LOG_PROCESS_ERROR(nResult);

Exit0:
	return;
}

void LPAPI LPHttpObject::DoResponse_BadRequest(void)
{
	INT_32 nResult = 0;

	LOG_PROCESS_ERROR(m_pSocker);
	nResult = m_pSocker->Send(g_szBadRequest, g_dwBadRequestLen);
	LOG_PROCESS_ERROR(nResult);

Exit0:
	return;
}

void LPAPI LPHttpObject::DoResponse_CanNotExcute(void)
{
	INT_32 nResult = 0;

	LOG_PROCESS_ERROR(m_pSocker);
	nResult = m_pSocker->Send(g_szCanNotExcute, g_dwCanNotExcuteLen);
	LOG_PROCESS_ERROR(nResult);

Exit0:
	return;
}

void LPAPI LPHttpObject::DoResponse_Headers(void)
{
	INT_32 nResult = 0;

	LOG_PROCESS_ERROR(m_pSocker);
	nResult = m_pSocker->Send(g_szHeaders, g_dwHeadersLen);
	LOG_PROCESS_ERROR(nResult);

Exit0:
	return;
}

void LPAPI LPHttpObject::DoResponse_UnImplemented(void)
{
	INT_32 nResult = 0;

	LOG_PROCESS_ERROR(m_pSocker);
	nResult = m_pSocker->Send(g_szUnImplemented, g_dwUnImplementedLen);
	LOG_PROCESS_ERROR(nResult);

Exit0:
	return;
}

void LPAPI LPHttpObject::DoResponse_RecvSuccess(const char * pcszBuf, UINT_32 dwSize)
{
	INT_32 nResult = 0;
	
	char szMsg[1024] = { 0 };
	char szStr1[] = "HTTP/1.0 200 OK\r\nServer: jdbhttpd/0.1.0\r\nContent-Type: text/html\r\n\r\n<HTML><TITLE>index.html</TITLE>\r\n<BODY><h1>";
	char szStr2[] = "</h1>\r\n</BODY></HTML>\r\n";

	INT_32 nStr1Count = 0;
	INT_32 nStr2Count = 0;
	INT_32 nMsgCount = 0;
	INT_32 nMaxMsgCount = 0;

	nStr1Count = sizeof(szStr1) - 1;
	nStr2Count = sizeof(szStr2) - 1;
	nMaxMsgCount = sizeof(szMsg) - 1;

	LOG_PROCESS_ERROR(pcszBuf);
	LOG_PROCESS_ERROR(m_pSocker);
	LOG_PROCESS_ERROR(dwSize + nStr1Count + nStr2Count <= nMaxMsgCount);

	memcpy(szMsg, szStr1, nStr1Count);
	memcpy(szMsg + nStr1Count, pcszBuf, dwSize);
	memcpy(szMsg + nStr1Count + dwSize, szStr2, nStr2Count);
	
	nResult = m_pSocker->Send(szMsg, nStr1Count + dwSize + nStr2Count);
	LOG_PROCESS_ERROR(nResult);

Exit0:
	return;
}

CGTHttpMessageHandler::CGTHttpMessageHandler()
{
}

CGTHttpMessageHandler::~CGTHttpMessageHandler()
{
	UnInit();
}

BOOL LPAPI CGTHttpMessageHandler::Init(void)
{
	INT_32 nResult = 0;

	nResult = m_oHttpObjectPool.Init(100, TRUE);
	LOG_PROCESS_ERROR(nResult);

	return TRUE;
Exit0:
	return FALSE;
}

BOOL LPAPI CGTHttpMessageHandler::UnInit(void)
{
	return TRUE;
}

void LPAPI CGTHttpMessageHandler::AddRef(void)
{
	++m_dwRef;
}

UINT_32 LPAPI CGTHttpMessageHandler::QueryRef(void)
{
	return m_dwRef;
}

void LPAPI CGTHttpMessageHandler::Release(void)
{
	LOG_CHECK_ERROR(m_dwRef > 0);

	if (m_dwRef > 0)
	{
		--m_dwRef;
	}
}

void LPAPI CGTHttpMessageHandler::OnAccepted(ILPSocker * pSocker)
{
	INT_32 nResult = 0;
	LPHttpObject* pHttpObject = NULL;
	std::pair<MAP_HTTP_OBJECT::iterator, bool> InsRet;

	LOG_PROCESS_ERROR(pSocker);

	pHttpObject = _NewHttpObject();
	LOG_PROCESS_ERROR(pHttpObject);

	nResult = pHttpObject->Init(pSocker);
	LOG_PROCESS_ERROR(nResult);

	InsRet = m_mapHttpObject.insert(std::make_pair(pSocker, pHttpObject));
	LOG_PROCESS_ERROR(InsRet.second);

	return;

Exit0:

	return;
}

void LPAPI CGTHttpMessageHandler::OnConnected(ILPSocker * pSocker)
{
	LPHttpObject* pHttpObject = NULL;
	std::pair<MAP_HTTP_OBJECT::iterator, bool> InsRet;

	LOG_PROCESS_ERROR(pSocker);

	pHttpObject = _NewHttpObject();
	LOG_PROCESS_ERROR(pHttpObject);

	InsRet = m_mapHttpObject.insert(std::make_pair(pSocker, pHttpObject));
	LOG_PROCESS_ERROR(InsRet.second);

	return;

Exit0:

	return;
}

void LPAPI CGTHttpMessageHandler::OnConnectError(ILPConnector * pConnector, UINT_32 dwErrorNo)
{
	LOG_PROCESS_ERROR(pConnector);

	ERR("connect fail, connector id %d, errno %d", pConnector->GetId(), dwErrorNo);

Exit0:
	return;
}

void LPAPI CGTHttpMessageHandler::OnMessage(ILPSocker * pSocker, const char * pcszBuf, UINT_32 dwSize)
{
	INT_32 nResult = 0;
	
	__TRY__
	{
		nResult = _ParseHttpMessage(pSocker, pcszBuf, dwSize);
		LOG_PROCESS_ERROR(nResult);
	}
	__EXCEPT__
	{

	}

Exit0:
	return;
}

void LPAPI CGTHttpMessageHandler::OnDisconnected(ILPSocker * pSocker)
{
	LOG_PROCESS_ERROR(pSocker);

	if (pSocker->IsPassiveClose())
	{
		IMP("%s peer close the socker : (%d:%d)", __FUNCTION__, pSocker->GetSockerId(), pSocker->GetSock());
	}
	else
	{
		IMP("%s local close the socker : (%d:%d)", __FUNCTION__, pSocker->GetSockerId(), pSocker->GetSock());
	}

	m_iterHttpObject = m_mapHttpObject.find(pSocker);
	LOG_CHECK_ERROR(m_iterHttpObject != m_mapHttpObject.end());
	if (m_iterHttpObject != m_mapHttpObject.end())
	{
		_DelHttpObject(m_iterHttpObject->second);
	}
	m_mapHttpObject.erase(pSocker);

	return;

Exit0:

	return;
}

void LPAPI CGTHttpMessageHandler::OnConnectDisconnected(ILPSocker * pSocker, ILPConnector * pConnector)
{
	LOG_PROCESS_ERROR(pSocker);
	LOG_PROCESS_ERROR(pConnector);

	if (pSocker->IsPassiveClose())
	{
		IMP("%s peer close the connector socker : (%d:%d)", __FUNCTION__, pSocker->GetSockerId(), pSocker->GetSock());
	}
	else
	{
		IMP("%s local close the connector socker : (%d:%d)", __FUNCTION__, pSocker->GetSockerId(), pSocker->GetSock());
	}

	m_mapHttpObject.erase(pSocker);

	return;

Exit0:

	return;
}

INT_32 LPAPI CGTHttpMessageHandler::Parse(ILPLoopBuf * pLoopBuf)
{
	INT_32 nResult = 0;
	INT_32 nParseCount = 0;
	UINT_32 dwOnceReadableLen = 0;
	UINT_32 dwTotalReadableLen = 0;
	const char* pszReadPtr = NULL;
	const char* pszWritePtr = NULL;

	LOG_PROCESS_ERROR(pLoopBuf);
	dwOnceReadableLen = pLoopBuf->GetOnceReadableLen();
	dwTotalReadableLen = pLoopBuf->GetTotalReadableLen();

	nParseCount = 0;
	pszReadPtr = pLoopBuf->ReadPtr();
	pszWritePtr = pLoopBuf->WritePtr();
	LOG_PROCESS_ERROR(pszReadPtr);
	LOG_PROCESS_ERROR(pszWritePtr);
	if (dwTotalReadableLen <= dwOnceReadableLen)
	{
		if (pszReadPtr[dwOnceReadableLen - 2] == '\r' && pszReadPtr[dwOnceReadableLen - 1] == '\n')
		{
			nParseCount = dwTotalReadableLen;
		}
	}
	else if(dwTotalReadableLen == dwOnceReadableLen + 1)
	{
		if (pszReadPtr[dwOnceReadableLen - 1] == '\r' && *(pszWritePtr - 1) == '\n')
		{
			nParseCount = dwTotalReadableLen;
		}
	}
	else
	{
		if (*(pszWritePtr - 2) == '\r' && *(pszWritePtr - 1) == '\n')
		{
			nParseCount = dwTotalReadableLen;
		}
	}

	PROCESS_SUCCESS(TRUE);

Exit0:

	nParseCount = -1;

Exit1:

	return nParseCount;
}

void CGTHttpMessageHandler::CloseAllHttpObject(void)
{
	LPHttpObject* pHttpObject = NULL;

	for (m_iterHttpObject = m_mapHttpObject.begin(); m_iterHttpObject != m_mapHttpObject.end(); ++m_iterHttpObject)
	{
		pHttpObject = m_iterHttpObject->second;
		LOG_CHECK_ERROR(pHttpObject);
		IF_NULL_CONTINUE(pHttpObject);

		pHttpObject->Close();
	}
}

LPHttpObject *LPAPI CGTHttpMessageHandler::_NewHttpObject(void)
{
	return m_oHttpObjectPool.Malloc();
}

BOOL LPAPI CGTHttpMessageHandler::_DelHttpObject(LPHttpObject * pHttpObject)
{
	INT_32 nResult = 0;

	m_oHttpObjectPool.Free(pHttpObject);

	return TRUE;
}

BOOL LPAPI CGTHttpMessageHandler::_ParseHttpMessage(ILPSocker * pSocker, const char * pcszBuf, UINT_32 dwSize)
{
	INT_32 nResult = 0;
	INT_32 nIndex = 0;
	INT_32 nIndexTemp = 0;
	BOOL bGetMethod = FALSE;
	BOOL bPostMethod = FALSE;
	LPHttpObject* pHttpObject = NULL;

	Json::Reader jsonReader;
	Json::Value jsonRoot;

	static char method[8];
	static char szMsgBuf[2048];
	static UINT_32 dwMsgBufSize = 2048;

	INT_32 nMsgSize = 0;
	UINT_32 dwRealMsgSize = 0;
	const char* pFindStr = NULL;

	LOG_PROCESS_ERROR(pSocker);
	LOG_PROCESS_ERROR(pcszBuf);

	lpFastZeroCharArray(szMsgBuf);

	//INF("socker_id=%d, pcszBuf=%s", pSocker->GetSockerId(), pcszBuf);

	nIndexTemp = 0;
	while (nIndex < dwSize && !ISSPACE(pcszBuf[nIndex]) && nIndex < sizeof(method) - 1)
	{
		method[nIndexTemp] = pcszBuf[nIndex];
		++nIndexTemp;
		++nIndex;
	}
	method[nIndexTemp] = '\0';

	if (strcmp(method, "GET") == 0)
	{
		bGetMethod = TRUE;
	}
	else if (strcmp(method, "POST") == 0)
	{
		bPostMethod = TRUE;
	}

	if (!bGetMethod && !bPostMethod)
	{
		PROCESS_SUCCESS(TRUE);
	}

	while (nIndex < dwSize && ISSPACE(pcszBuf[nIndex]))
	{
		++nIndex;
	}

	LOG_PROCESS_ERROR(nIndex < dwSize);
	LOG_PROCESS_ERROR(pcszBuf[nIndex] == '/');
	++nIndex;

	LOG_PROCESS_ERROR(nIndex < dwSize);
	pFindStr = strstr(pcszBuf, "HTTP/");
	LOG_PROCESS_ERROR(pFindStr);
	LOG_PROCESS_ERROR(pFindStr > pcszBuf + nIndex);

	--pFindStr;
	while (pFindStr > pcszBuf + nIndex && ISSPACE(*pFindStr))
	{
		--pFindStr;
	}
	LOG_PROCESS_ERROR(pFindStr > pcszBuf + nIndex);

	nMsgSize = (INT_32)(pFindStr - pcszBuf - nIndex + 1);
	LOG_PROCESS_ERROR(nMsgSize < dwMsgBufSize);

	dwRealMsgSize = dwMsgBufSize - 1;
	nResult = lpUrlDecode(pcszBuf + nIndex, nMsgSize, szMsgBuf, dwRealMsgSize, eUrlCode_Standard);
	LOG_PROCESS_ERROR(nResult);
	szMsgBuf[dwRealMsgSize] = '\0';

	IMP("### recv http msg: %s", szMsgBuf);

	nResult = jsonReader.parse(szMsgBuf, jsonRoot);
	LOG_CHECK_ERROR(nResult);

	m_iterHttpObject = m_mapHttpObject.find(pSocker);
	LOG_PROCESS_ERROR(m_iterHttpObject != m_mapHttpObject.end());
	pHttpObject = m_iterHttpObject->second;
	LOG_PROCESS_ERROR(pHttpObject);

	pHttpObject->DoResponse_RecvSuccess(szMsgBuf, dwRealMsgSize);

Exit1:
	return TRUE;
Exit0:
	return FALSE;
}
