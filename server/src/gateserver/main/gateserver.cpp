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
	m_pNet                   = nullptr;
	m_pListener              = nullptr;
	m_pClientListener        = nullptr;
	m_dwServerState          = eServerState_Invalid;
}

CGateServer::~CGateServer()
{
	//这里不能调用，该函数内部会调用其他模块的UnInit函数，而
	//其他模块可能在本模块释放前已经释放，再调用UnInit会出错
	//UnInit();
}

BOOL LPAPI CGateServer::Init(void)
{
	LPINT32 nResult = 0;
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

	nResult = ILPNet::GlobalInit();
	LOG_PROCESS_ERROR(nResult);

	stNetConfig = g_GlobalConfig.Server.Gt.Net;
	m_pNet = ILPNet::CreateNetModule(&m_oGTMessageHandler, &stNetConfig);
	LOG_PROCESS_ERROR(m_pNet != nullptr);

	m_pListener = m_pNet->CreateListenerCtrl(m_pGTInternalPacketParser);
	LOG_PROCESS_ERROR(m_pListener != nullptr);

	nResult = m_pListener->Start(g_GlobalConfig.Server.Gt.szListenIp, g_GlobalConfig.Server.Gt.dwListenPort, TRUE);
	LOG_PROCESS_ERROR(nResult);

	m_pClientListener = m_pNet->CreateListenerCtrl(m_pGTExternalPacketParser);
	LOG_PROCESS_ERROR(m_pClientListener != nullptr);

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
	LPINT32 nResult = 0;

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

	//删除释放net对象
	ILPNet::DeleteNetModule(m_pNet);

	SetServerState(eServerState_UnInited);

	return TRUE;
}

BOOL LPAPI CGateServer::MainLoop(void)
{
	LPINT32 nResult = 0;

	nResult = m_pNet->Run(30);
	if (nResult)
	{
		lpSleep(1);
	}

	return TRUE;
}

LPUINT32 LPAPI CGateServer::GetServerState(void)
{
	return m_dwServerState;
}

void CGateServer::SetServerState(LPUINT32 dwServerState)
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
		m_pListener = nullptr;
	}

	if (m_pClientListener)
	{
		m_pClientListener->Stop();
		m_pClientListener = nullptr;
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
	static LPUINT32 s_dwLastCheckTime = 0;

	LPUINT32 dwTime = (LPUINT32)time(NULL);
	if (dwTime > s_dwLastCheckTime + 3)
	{
		s_dwLastCheckTime = dwTime;
		return TRUE;
	}

	return FALSE;
}

BOOL _Test_UrlCode(void)
{
	LPINT32 nResult = 0;

	const char szStr[] = "1-_.~( )";
	char szBuf1[12] = { 0 };
	char szBuf2[8] = { 0 };

	LPUINT32 dwBuf1Size = 0;
	LPUINT32 dwBuf2Size = 0;

	dwBuf1Size = sizeof(szBuf1);
	nResult = lpUrlEncode(szStr, (LPUINT32)strlen(szStr), szBuf1, dwBuf1Size, eUrlCode_Standard);
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
	LPINT32 nResult = 0;

	const char szStr[] = "4F60597D";
	char szBuf1[100] = { 0 };
	char szBuf2[100] = { 0 };

	LPUINT32 dwBuf1Size = 0;
	LPUINT32 dwBuf2Size = 0;

	dwBuf1Size = sizeof(szBuf1);
	nResult = lpBase64Encode(szStr, (LPUINT32)strlen(szStr), szBuf1, dwBuf1Size, eBase64Code_Standard);
	LOG_PROCESS_ERROR(nResult);

	dwBuf2Size = sizeof(szBuf2);
	nResult = lpBase64Decode(szBuf1, dwBuf1Size, szBuf2, dwBuf2Size, eBase64Code_Standard);
	LOG_PROCESS_ERROR(nResult);

	return TRUE;
Exit0:
	return FALSE;
}

static char gs_szHttpMessage[] = "GET /{\"id\":1} HTTP/1.1\r\n";
BOOL LPAPI _Test_ParseHttpMessage(const char * pcszBuf, LPUINT32 dwSize)
{
	LPINT32 nResult = 0;
	LPINT32 nIndex = 0;
	LPINT32 nIndexTemp = 0;
	BOOL bGetMethod = FALSE;
	BOOL bPostMethod = FALSE;
	LPHttpObject* pHttpObject = NULL;

	Json::Reader jsonReader;
	Json::Value jsonRoot;
	Json::Reader* pReader = NULL;
	Json::Value* pRoot = NULL;

	char method[8];
	char* pTempBuf = NULL;
	LPINT32 nTempBufSize = 0;
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

	nTempBufSize = (LPINT32)(pFindStr - pcszBuf - nIndex + 1 + 1);
	pTempBuf = new char[nTempBufSize];
	LOG_PROCESS_ERROR(pTempBuf);
	memset(pTempBuf, 0, nTempBufSize);

#	ifdef _WIN32
	{
		memcpy_s(pTempBuf, nTempBufSize - 1, pcszBuf + nIndex, nTempBufSize - 1);
	}
#	else
	{
		memcpy(pTempBuf, pcszBuf + nIndex, nTempBufSize - 1);
	}
#	endif

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
	LPUINT32 dwFindValue = 0;
	std::map<LPUINT32, LPUINT32> mapID2ID;
	std::unordered_map<LPUINT32, LPUINT32> mapUnorderID2ID;
	std::set<LPUINT32> setID;
	std::unordered_set<LPUINT32> setUnorderID;

	cout << "profile start ..." << endl;

	PROFILE_START();

	PROFILE_POINT(1);
	for (LPUINT32 i = 0; i < _TEST_MAP_COUNT_; ++i)
	{
		mapID2ID.insert(std::make_pair(i + 1, i + 1));
	}
	PROFILE_POINT(1);

	PROFILE_POINT(2);
	for (LPUINT32 i = 0; i < _TEST_MAP_COUNT_; ++i)
	{
		mapUnorderID2ID.insert(std::pair<LPUINT32, LPUINT32>(i + 1, i + 1));
	}
	PROFILE_POINT(2);

	PROFILE_POINT(3);
	for (LPUINT32 i = 0; i < _TEST_MAP_COUNT_; ++i)
	{
		setID.insert(i + 1);
	}
	PROFILE_POINT(3);

	PROFILE_POINT(4);
	for (LPUINT32 i = 0; i < _TEST_MAP_COUNT_; ++i)
	{
		setUnorderID.insert(i + 1);
	}
	PROFILE_POINT(4);

	PROFILE_POINT(6);
	for (LPUINT32 i = 0; i < _TEST_MAP_COUNT_; ++i)
	{
		auto fit = mapID2ID.find(i + 1);
		if (fit != mapID2ID.end())
		{
			dwFindValue = fit->second;
		}
	}
	PROFILE_POINT(6);


	PROFILE_POINT(7);
	for (LPUINT32 i = 0; i < _TEST_MAP_COUNT_; ++i)
	{
		auto fit = mapUnorderID2ID.find(i + 1);
		if (fit != mapUnorderID2ID.end())
		{
			dwFindValue = fit->second;
		}
	}
	PROFILE_POINT(7);


	PROFILE_POINT(8);
	for (LPUINT32 i = 0; i < _TEST_MAP_COUNT_; ++i)
	{
		auto fit = setID.find(i + 1);
		if (fit != setID.end())
		{
			dwFindValue = (*fit);
		}
	}
	PROFILE_POINT(8);


	PROFILE_POINT(9);
	for (LPUINT32 i = 0; i < _TEST_MAP_COUNT_; ++i)
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
	LPINT32 nResult = 0;
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
	LPINT32 nResult = 0;

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
	LPINT32 nResult = 0;

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
	LPINT32 nResult = 0;

	LPTime::GlobalInit(8);
	LPTime nowTime = LPTime::GetNowTime();
	LPUINT64 qwNowTimestamp = LPTime::GetNowTimetamp();
	LPTime nowUTCTime = LPTime::GetNowUTCTime();
	LPUINT64 qwNowUTCTimestamp = LPTime::GetNowUTCTimestamp();

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
	LPINT32 nResult = 0;

	LPINT32 nIndex = 0;
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
	LPINT32 nResult = 0;

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
	LPINT32 nResult = 0;
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
	LPINT32 nResult = 0;
	LPString str = LPString(2, true);
	std::string strTemp = NULL_STR;

	ILPData* poDataInt64 = ILPData::NewData(eDataType_Int64);
	ILPData* poDataFloat = ILPData::NewData(eDataType_Float);
	ILPData* poDataDouble = ILPData::NewData(eDataType_Double);
	ILPData* poDataString = ILPData::NewData(eDataType_String);

	//strTemp = poDataInt64->ToString();
	//nResult = poDataInt64->SetInt64(1234567890);
	//strTemp = poDataInt64->ToString();
	//nResult = poDataInt64->SetFloat(1234567890.123456789f);
	//nResult = poDataInt64->SetDouble(1234567890.123456789);
	//nResult = poDataInt64->SetString("hello1234567890");

	//strTemp = poDataFloat->ToString();
	//nResult = poDataFloat->SetFloat(1234567890.123456789f);
	//strTemp = poDataFloat->ToString();
	//nResult = poDataFloat->SetInt64(1234567890);
	//nResult = poDataFloat->SetDouble(1234567890.123456789);
	//nResult = poDataFloat->SetString("hello1234567890");

	//strTemp = poDataDouble->ToString();
	//nResult = poDataDouble->SetDouble(1234567890.123456789);
	//strTemp = poDataDouble->ToString();
	//nResult = poDataDouble->SetInt64(1234567890);
	//nResult = poDataDouble->SetFloat(1234567890.123456789f);
	//nResult = poDataDouble->SetString("hello1234567890");

	//strTemp = poDataString->ToString();
	//nResult = poDataString->SetString("hello1234567890");
	//strTemp = poDataString->ToString();
	//nResult = poDataString->SetInt64(1234567890);
	//nResult = poDataString->SetFloat(1234567890.123456789f);
	//nResult = poDataString->SetDouble(1234567890.123456789);

	int n1 = sizeof(*poDataInt64);
	int n2 = sizeof(*poDataFloat);
	int n3 = sizeof(*poDataDouble);
	int n4 = sizeof(*poDataString);

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
	LPINT32 nResult = 0;

	//LPDataList oDataList;
	LPDataList oDataList2;
	//LPDataList oDataList3;
	LPDataList oDataList4;
	//std::string strTemp = "tempstring";

	//oDataList << 1 << 1.1f << 1.1 << "hello1" << strTemp;
	oDataList2 << 1 << 2.2f << 3.3 << "4.4";
	//oDataList3 << 5 << 6.6f << 7.7 << "8.8" << "9.9";

	ILPDataList& oDataList22 = oDataList2;
	ILPDataList& oDataList44 = oDataList4;
	oDataList4 = oDataList22;
	PRINTF_DATA_LIST(IMP, oDataList44);

	//oDataList4.Concat(oDataList3);
	//PRINTF_DATA_LIST(IMP, oDataList4);

	//
	//LPINT64 lData = oDataList.Int64(0);
	//FLOAT fData = oDataList.Float(1);
	//DOUBLE dData = oDataList.Double(2);
	//std::string strData = oDataList.String(3);

	LOG_PROCESS_ERROR(TRUE);
	return TRUE;
Exit0:
	return FALSE;
}

class DECLARE TestClass
{
public:

private:
	LPINT32 m_nData;
};

BOOL _Test_Temp(void)
{
	LPINT32 nResult = 0;
	
	LOG_PROCESS_ERROR(TRUE);
	return TRUE;
Exit0:
	return FALSE;
}

BOOL _Test(void)
{
	LPINT32 nResult = 0;

	nResult = _Test_String();
	LOG_PROCESS_ERROR(nResult);

	LOG_PROCESS_ERROR(TRUE);

	return TRUE;
Exit0:
	return FALSE;
}

int main(int argc, char* argv[])
{
	LPINT32 nResult = 0;
	LOG_CONFIG stLogConfig;
	LPLuaScript* pScript = NULL;
	LPLuaScriptMgr* pScriptMgr = NULL;

	RedefinedConsole();

	// 使用LZPL，必须在程序运行时先初始化
	nResult = lpLoadLogConfig(lpGetExeDir(), "logconfig.xml", stLogConfig);
	PRINTF_PROCESS_ERROR(nResult);

#	ifdef _DEBUG
	{
		nResult = lpInitLzpl(stLogConfig);
	}
#	else
	{
		nResult = lpInitLzpl(stLogConfig);
	}
#	endif

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
