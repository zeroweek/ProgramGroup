#include "gateserver.h"
#include "lp_processerror.h"
#include "json/json.h"
#include <unordered_map>
#include <unordered_set>
#include "lp_profile.h"
#include <uchar.h>
#include <locale>
#include <string>
#include <codecvt>
#include <fstream>
#include "lp_urlcode.h"
#include "lp_base64.h"
#include "lp_md5.h"
#include "lp_codeconvert.h"
#include "lp_filelist.h"
#include "lp_dump.h"
#include "external_message_header.h"
#include "internal_message_header.h"
#include "lp_sharememory.h"

#include "testcase.h"


SINGLETON_IMPLEMENT(CGateServer)

CGateServer::CGateServer()
{
	m_pNet          = NULL;
	m_pListener     = NULL;
	m_dwServerState = eServerState_Invalid;
}

CGateServer::~CGateServer()
{
	//这里不能调用，该函数内部会调用其他模块的UnInit函数，而
	//其他模块可能在本模块释放前已经释放，再调用UnInit会出错
	//UnInit();
}

BOOL LPAPI CGateServer::Init(void)
{
	INT_32 nResult = 0;
	NET_CONFIG stNetConfig;

	SetServerState(eServerState_Initing);

	// 加载配置
	nResult = g_LoadGlobalConfig(g_szGlobalConfigFileName);
	LOG_PROCESS_ERROR(nResult);

	nResult = m_oGTMessageHandler.Init();
	LOG_PROCESS_ERROR(nResult);

	m_pGTInternalPacketParser = new CGTInternalPacketParser();
	LOG_PROCESS_ERROR(m_pGTInternalPacketParser);
	m_pGTInternalPacketParser->AddRef();

	m_pGTExternalPacketParser = new CGTExternalPacketParser();
	LOG_PROCESS_ERROR(m_pGTExternalPacketParser);
	m_pGTExternalPacketParser->AddRef();

	nResult = m_oGTHttpMessageHandler.Init();
	LOG_PROCESS_ERROR(nResult);

	stNetConfig = g_GlobalConfig.Server.Gt.Net;
	m_pNet = lpCreateNetModule(&m_oGTMessageHandler, &stNetConfig);
	LOG_PROCESS_ERROR(m_pNet);

	m_pListener = m_pNet->CreateListenerCtrl(eIoType_CompletionPort, m_pGTInternalPacketParser);
	LOG_PROCESS_ERROR(m_pListener);

	nResult = m_pListener->Start(g_GlobalConfig.Server.Gt.szListenIp, g_GlobalConfig.Server.Gt.dwListenPort, TRUE);
	LOG_PROCESS_ERROR(nResult);

	m_pClientListener = m_pNet->CreateListenerCtrl(eIoType_CompletionPort, m_pGTExternalPacketParser);
	LOG_PROCESS_ERROR(m_pListener);

	nResult = m_pClientListener->Start(g_GlobalConfig.Server.Gt.szClientListenIp, g_GlobalConfig.Server.Gt.dwClientListenPort, TRUE);
	LOG_PROCESS_ERROR(nResult);

	SetServerState(eServerState_SelfReady);

	return TRUE;

Exit0:

	nResult = UnInit();
	LOG_CHECK_ERROR(nResult);

	return FALSE;
}

BOOL LPAPI CGateServer::UnInit(void)
{
	INT_32 nResult = 0;

	SetServerState(eServerState_UnIniting);

	nResult = m_oGTMessageHandler.UnInit();
	LOG_CHECK_ERROR(nResult);
	
	if (m_pGTInternalPacketParser)
	{
		m_pGTInternalPacketParser->Release();
		m_pGTInternalPacketParser = NULL;
	}

	if (m_pGTExternalPacketParser)
	{
		m_pGTExternalPacketParser->Release();
		m_pGTExternalPacketParser = NULL;
	}

	if (NULL != m_pNet)
	{
		m_pNet->Release();
		m_pNet = NULL;
	}

	SetServerState(eServerState_UnInited);

	return TRUE;
}

BOOL LPAPI CGateServer::MainLoop(void)
{
	INT_32 nResult = 0;

	nResult = m_pNet->Run(30);
	if (nResult)
	{
		lpSleep(1);
	}

	return TRUE;
}

UINT_32 LPAPI CGateServer::GetServerState(void)
{
	return m_dwServerState;
}

void CGateServer::SetServerState(UINT_32 dwServerState)
{
	m_dwServerState = dwServerState;
}

void CGateServer::Close(void)
{
	SetServerState(eServerState_Closed);

	//停止所有监听器和连接器
	if (m_pListener)
	{
		m_pListener->Stop();
		m_pListener = NULL;
	}
	if (m_pClientListener)
	{
		m_pClientListener->Stop();
		m_pClientListener = NULL;
	}

	//关闭所有链接
	m_oGTMessageHandler.CloseAllSocker();
	m_oGTHttpMessageHandler.CloseAllHttpObject();

	//等待所有链接关闭
}

BOOL _Test_Jsoncpp(void)
{
	int nCount = 1024;
	int* pN = NULL;
	const char* str = "{\"uploadid\": \"UP000000\",\"code\": 100,\"msg\": \"\",\"files\": \"\"}";

	Json::Reader reader;
	Json::Value root;

	pN = new int[nCount];
	LOG_PROCESS_ERROR(pN);
	
	if (reader.parse("{\"id\":123}", root))  // reader将Json字符串解析到root，root将包含Json里所有子元素  
	{
		std::string upload_id = root["uploadid"].asString();  // 访问节点，upload_id = "UP000000"  
		int code = root["code"].asInt();    // 访问节点，code = 100 
	}

	if (pN)
	{
		SAFE_DELETE_SZ(pN);
	}

	return TRUE;
Exit0:

	if (pN)
	{
		SAFE_DELETE_SZ(pN);
	}

	return FALSE;
}

BOOL _Test_DumpCheckFunc(void)
{
	static UINT_32 s_dwLastCheckTime = 0;

	UINT_32 dwTime = (UINT_32)time(NULL);
	if (dwTime > s_dwLastCheckTime + 3)
	{
		s_dwLastCheckTime = dwTime;
		return TRUE;
	}

	return FALSE;
}

BOOL _Test_UrlCode(void)
{
	INT_32 nResult = 0;

	const char szStr[] = "1-_.~( )";
	char szBuf1[12] = { 0 };
	char szBuf2[8] = { 0 };

	UINT_32 dwBuf1Size = 0;
	UINT_32 dwBuf2Size = 0;

	dwBuf1Size = sizeof(szBuf1);
	nResult = lpUrlEncode(szStr, (UINT_32)strlen(szStr), szBuf1, dwBuf1Size, eUrlCode_Standard);
	LOG_PROCESS_ERROR(nResult);

	dwBuf2Size = sizeof(szBuf2);
	nResult = lpUrlDecode(szBuf1, dwBuf1Size, szBuf2, dwBuf2Size, eUrlCode_Standard);
	LOG_PROCESS_ERROR(nResult);

	return TRUE;
Exit0:
	return FALSE;
}

BOOL _Test_Base64(void)
{
	INT_32 nResult = 0;

	const char szStr[] = "4F60597D";
	char szBuf1[100] = { 0 };
	char szBuf2[100] = { 0 };

	UINT_32 dwBuf1Size = 0;
	UINT_32 dwBuf2Size = 0;

	dwBuf1Size = sizeof(szBuf1);
	nResult = lpBase64Encode(szStr, (UINT_32)strlen(szStr), szBuf1, dwBuf1Size, eBase64Code_Standard);
	LOG_PROCESS_ERROR(nResult);

	dwBuf2Size = sizeof(szBuf2);
	nResult = lpBase64Decode(szBuf1, dwBuf1Size, szBuf2, dwBuf2Size, eBase64Code_Standard);
	LOG_PROCESS_ERROR(nResult);

	return TRUE;
Exit0:
	return FALSE;
}

static char gs_szHttpMessage[] = "GET /{\"id\":1} HTTP/1.1\r\n";
BOOL LPAPI _Test_ParseHttpMessage(const char * pcszBuf, UINT_32 dwSize)
{
	INT_32 nResult = 0;
	INT_32 nIndex = 0;
	INT_32 nIndexTemp = 0;
	BOOL bGetMethod = FALSE;
	BOOL bPostMethod = FALSE;
	LPHttpObject* pHttpObject = NULL;

	Json::Reader jsonReader;
	Json::Value jsonRoot;
	Json::Reader* pReader = NULL;
	Json::Value* pRoot = NULL;

	char method[8];
	char* pTempBuf = NULL;
	INT_32 nTempBufSize = 0;
	const char* pFindStr = NULL;

	LOG_PROCESS_ERROR(pcszBuf);

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

	nTempBufSize = (INT_32)(pFindStr - pcszBuf - nIndex + 1 + 1);
	pTempBuf = new char[nTempBufSize];
	LOG_PROCESS_ERROR(pTempBuf);
	memset(pTempBuf, 0, nTempBufSize);

	memcpy_s(pTempBuf, nTempBufSize - 1, pcszBuf + nIndex, nTempBufSize - 1);

	IMP("msg: %s", pTempBuf);

	nResult = jsonReader.parse("{\"id\":1}", jsonRoot);
	LOG_CHECK_ERROR(nResult);

	if (pTempBuf)
	{
		SAFE_DELETE_SZ(pTempBuf);
	}

Exit1:
	return TRUE;

Exit0:

	if (pTempBuf)
	{
		SAFE_DELETE_SZ(pTempBuf);
	}
	return FALSE;
}

BOOL LPAPI _Test_MD5(void)
{
	string str = "1234567890";
	MD5 md5(str);
	string result = md5.md5();
	//cout << "1234567890 md5 is " << result << endl;

	return TRUE;
}

#define _TEST_MAP_COUNT_             (1000000)
BOOL LPAPI _Test_Map(void)
{
	UINT_32 dwFindValue = 0;
	std::map<UINT_32, UINT_32> mapID2ID;
	std::unordered_map<UINT_32, UINT_32> mapUnorderID2ID;
	std::set<UINT_32> setID;
	std::unordered_set<UINT_32> setUnorderID;

	cout << "profile start ..." << endl;

	PROFILE_START();

	PROFILE_POINT(1);
	for (UINT_32 i = 0; i < _TEST_MAP_COUNT_; ++i)
	{
		mapID2ID.insert(std::make_pair(i + 1, i + 1));
	}
	PROFILE_POINT(1);

	PROFILE_POINT(2);
	for (UINT_32 i = 0; i < _TEST_MAP_COUNT_; ++i)
	{
		mapUnorderID2ID.insert(std::pair<UINT_32, UINT_32>(i + 1, i + 1));
	}
	PROFILE_POINT(2);

	PROFILE_POINT(3);
	for (UINT_32 i = 0; i < _TEST_MAP_COUNT_; ++i)
	{
		setID.insert(i + 1);
	}
	PROFILE_POINT(3);

	PROFILE_POINT(4);
	for (UINT_32 i = 0; i < _TEST_MAP_COUNT_; ++i)
	{
		setUnorderID.insert(i + 1);
	}
	PROFILE_POINT(4);

	PROFILE_POINT(6);
	for (UINT_32 i = 0; i < _TEST_MAP_COUNT_; ++i)
	{
		auto fit = mapID2ID.find(i + 1);
		if (fit != mapID2ID.end())
		{
			dwFindValue = fit->second;
		}
	}
	PROFILE_POINT(6);


	PROFILE_POINT(7);
	for (UINT_32 i = 0; i < _TEST_MAP_COUNT_; ++i)
	{
		auto fit = mapUnorderID2ID.find(i + 1);
		if (fit != mapUnorderID2ID.end())
		{
			dwFindValue = fit->second;
		}
	}
	PROFILE_POINT(7);


	PROFILE_POINT(8);
	for (UINT_32 i = 0; i < _TEST_MAP_COUNT_; ++i)
	{
		auto fit = setID.find(i + 1);
		if (fit != setID.end())
		{
			dwFindValue = (*fit);
		}
	}
	PROFILE_POINT(8);


	PROFILE_POINT(9);
	for (UINT_32 i = 0; i < _TEST_MAP_COUNT_; ++i)
	{
		auto fit = setUnorderID.find(i + 1);
		if (fit != setUnorderID.end())
		{
			dwFindValue = (*fit);
		}
	}
	PROFILE_POINT(9);

	PROFILE_END();

	cout << "profile end." << endl;

	return TRUE;
}

BOOL _Test_ConvertCode(void)
{
	INT_32 nResult = 0;
	string sUtf8;
	string sGB2312;
	char szGB2312[1024] = "你";
	char szUtf[1024] = "\xe4\xbd\xa0";

	nResult = lpGB2312ToUTF8(szGB2312, 2, sUtf8);
	LOG_PROCESS_ERROR(nResult);

	nResult = lpUTF8ToGB2312(szUtf, 3, sGB2312);
	LOG_PROCESS_ERROR(nResult);

	return TRUE;
Exit0:
	return FALSE;
}

BOOL _Test_FileList(void)
{
	INT_32 nResult = 0;

	FILE_LIST fileList;

	nResult = fileList.ScanDirectory(".", ".dll", TRUE);
	nResult = fileList.ScanDirectory("log", ".log", TRUE);
	LOG_PROCESS_ERROR(nResult);

	return TRUE;
Exit0:
	return FALSE;
}

BOOL _Test_ShareMem(void)
{
	INT_32 nResult = 0;

	LPShareMemory oShareMemory;

	nResult = oShareMemory.Init("hello", 1024 * 1024);
	LOG_PROCESS_ERROR(nResult);

	if (oShareMemory.IsNew())
	{
		lpStrCpyN(oShareMemory.Mem(), "hello", oShareMemory.Size());
		LPPRINTF("write memory : %s\n", oShareMemory.Mem());
	}
	else
	{
		LPPRINTF("read memory : %s\n", oShareMemory.Mem());
	}

	return TRUE;
Exit0:
	return FALSE;
}

BOOL _Test_Time(void)
{
	INT_32 nResult = 0;

	LPTime::GlobalInit(8);
	LPTime nowTime = LPTime::GetNowTime();
	UINT_64 qwNowTimestamp = LPTime::GetNowTimetamp();
	LPTime nowUTCTime = LPTime::GetNowUTCTime();
	UINT_64 qwNowUTCTimestamp = LPTime::GetNowUTCTimestamp();

	LPTime tempTime = nowTime;
	nResult = tempTime.SecEqualTo(nowTime);
	nResult = tempTime.UsecEqualTo(nowTime);

	nResult = tempTime.SecEqualTo(nowUTCTime);
	nResult = tempTime.UsecEqualTo(nowUTCTime);

	PROCESS_ERROR(TRUE);
	return TRUE;
Exit0:
	return FALSE;
}

BOOL _Test_WhileWriteLog(void)
{
	INT_32 nResult = 0;

	INT_32 nIndex = 0;
	while (true)
	{
		lpSleep(100);
		INF("Hello, welcome to %d !", ++nIndex);
	}

	PROCESS_ERROR(TRUE);
	return TRUE;
Exit0:
	return FALSE;
}

BOOL _Test_ConsoleColor(void)
{
	INT_32 nResult = 0;

	DBG("This is a INF message.");
	WRN("This is a WRN message.");
	INF("This is a INF message.");
	IMP("This is a IMP message.");
	ERR("This is a ERR message.");
	FTL("This is a FTL message.");
	LUA("This is a LUA message.");

	PROCESS_ERROR(TRUE);
	return TRUE;
Exit0:
	return FALSE;
}

BOOL _Test_String(void)
{
	INT_32 nResult = 0;
	LPString str = LPString(2, true);

	nResult = str.Append("abcdefghijklmnopqrstuvwxyz", 0, 3);
	LOG_CHECK_ERROR(nResult);
	nResult = str.Insert(2, "abcdefghijklmnopqrstuvwxyz", 0, 6);
	LOG_CHECK_ERROR(nResult);
	nResult = str.Append("abcdefghijklmnopqrstuvwxyz", 0, 10);
	LOG_CHECK_ERROR(nResult);
	nResult = str.Append("abcdefghijklmnopqrstuvwxyz", 0, 10);
	LOG_CHECK_ERROR(nResult);

	PROCESS_ERROR(TRUE);
	return TRUE;
Exit0:
	return FALSE;
}

BOOL _Test_VarData(void)
{
	INT_32 nResult = 0;
	LPString str = LPString(2, true);
	std::string strTemp = NULL_STR;

	LPDataInt64 data64;
	LPDataFloat dataFloat;
	LPDataDouble dataDouble;
	LPDataString dataString;

	//strTemp = data64.ToString();
	//nResult = data64.SetInt64(1234567890);
	//strTemp = data64.ToString();
	//nResult = data64.SetFloat(1234567890.123456789f);
	//nResult = data64.SetDouble(1234567890.123456789);
	//nResult = data64.SetString("hello1234567890");

	//strTemp = dataFloat.ToString();
	//nResult = dataFloat.SetFloat(1234567890.123456789f);
	//strTemp = dataFloat.ToString();
	//nResult = dataFloat.SetInt64(1234567890);
	//nResult = dataFloat.SetDouble(1234567890.123456789);
	//nResult = dataFloat.SetString("hello1234567890");

	//strTemp = dataDouble.ToString();
	//nResult = dataDouble.SetDouble(1234567890.123456789);
	//strTemp = dataDouble.ToString();
	//nResult = dataDouble.SetInt64(1234567890);
	//nResult = dataDouble.SetFloat(1234567890.123456789f);
	//nResult = dataDouble.SetString("hello1234567890");

	//strTemp = dataString.ToString();
	//nResult = dataString.SetString("hello1234567890");
	//strTemp = dataString.ToString();
	//nResult = dataString.SetInt64(1234567890);
	//nResult = dataString.SetFloat(1234567890.123456789f);
	//nResult = dataString.SetDouble(1234567890.123456789);

	int n1 = sizeof(data64);
	int n2 = sizeof(dataFloat);
	int n3 = sizeof(dataDouble);
	int n4 = sizeof(dataString);

	SIMPLE_LIST_NODE node;
	int n5 = sizeof(node);
	int n6 = sizeof(SIMPLE_LIST_NODE);

	PROCESS_ERROR(TRUE);
	return TRUE;
Exit0:
	return FALSE;
}

BOOL _Test_DataList(void)
{
	INT_32 nResult = 0;

	LPDATALIST oDataList;
	std::string strTemp = "tempstring";

	oDataList << 3 << 4.4f << 5.5 << "hello6" << strTemp;
	
	INT_64 lData = oDataList.Int64(0);
	FLOAT fData = oDataList.Float(1);
	DOUBLE dData = oDataList.Double(2);
	std::string strData = oDataList.String(3);

	LOG_PROCESS_ERROR(TRUE);
	return TRUE;
Exit0:
	return FALSE;
}

BOOL _Test(void)
{
	INT_32 nResult = 0;

	nResult = TC_TestSimpleList();
	LOG_PROCESS_ERROR(nResult);

	LOG_PROCESS_ERROR(TRUE);

	return TRUE;
Exit0:
	return FALSE;
}

int main(int argc, char* argv[])
{
	INT_32 nResult = 0;
	LOG_CONFIG stLogConfig;
	LPLuaScript* pScript = NULL;
	LPLuaScriptMgr* pScriptMgr = NULL;

	RedefinedConsole();

	// 使用LZPL，必须在程序运行时先初始化
	nResult = lpLoadLogConfig(lpGetExeDir(), "logconfig.xml", stLogConfig);
	PRINTF_PROCESS_ERROR(nResult);

#   ifdef _DEBUG
		nResult = lpInitLzpl(stLogConfig);
#   else
		nResult = lpInitLzpl(stLogConfig);
#   endif

	PRINTF_PROCESS_ERROR(nResult);

	nResult = CGateServer::Instance().Init();
	LOG_PROCESS_ERROR(nResult);

	nResult = LPDumpMgr::Instance().Init(NULL, "log");
	LOG_PROCESS_ERROR(nResult);

	IMP("gateserver start success !");

	nResult = _Test();
	LOG_PROCESS_ERROR(nResult);

	while (CGateServer::Instance().GetServerState() != eServerState_Closing)
	{
		__TRY__
		{
			nResult = CGateServer::Instance().MainLoop();
			LOG_CHECK_ERROR(nResult);
		}
		__EXCEPT__
		{

		}
	}

	CGateServer::Instance().Close();

Exit0:

	CGateServer::Instance().UnInit();

	LPPRINTF("\nEnter any key to exit !\n");
	getchar();

	LPDumpMgr::Instance().UnInit();

	return 0;
}
