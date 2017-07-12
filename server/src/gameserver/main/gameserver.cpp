#include "gameserver.h"
#include "lp_processerror.h"
#include "gamelogic.h"
#include "mysql.h"
#include "lp_dbinit.h"
#include "nav_mesh_build.h"
#include "curl/curl.h"
#include "lp_filelist.h"
#include "lp_dump.h"
#include "lp_sharememory.h"

#ifdef _WIN32

#pragma comment(lib, "sapi.lib")
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Wldap32.lib")
#pragma comment(lib, "crypt32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dbghelp.lib")

#   ifdef _DEBUG

#   pragma comment(lib, "libmysql.lib")
#   pragma comment(lib, "zlibstat64d.lib")
#   pragma comment(lib, "lzpl64d.lib")
#   pragma comment(lib, "message64d.lib")
#   pragma comment(lib, "gamelogic64d.lib")
#   pragma comment(lib, "common64d.lib")
#   pragma comment(lib, "luastat64d.lib")
#   pragma comment(lib, "toluapp64d.lib")
#   pragma comment(lib, "ssleay32.lib")
#   pragma comment(lib, "libeay32.lib")
#   pragma comment(lib, "libcurld.lib")

#   else

#   pragma comment(lib, "libmysql.lib")
#   pragma comment(lib, "zlibstat64.lib")
#   pragma comment(lib, "lzpl64.lib")
#   pragma comment(lib, "message64.lib")
#   pragma comment(lib, "gamelogic64.lib")
#   pragma comment(lib, "common64.lib")
#   pragma comment(lib, "luastat64.lib")
#   pragma comment(lib, "toluapp64.lib")
#   pragma comment(lib, "ssleay32.lib")
#   pragma comment(lib, "libeay32.lib")
#   pragma comment(lib, "libcurl.lib")

#   endif

#endif



SINGLETON_IMPLEMENT(CGameServer)

CGameServer::CGameServer()
{
    m_pNet          = nullptr;
    m_pConnector    = nullptr;
    m_pListener     = NULL;
    m_dwServerState = eServerState_Invalid;
}

CGameServer::~CGameServer()
{
    //这里不能调用，该函数内部会调用其他模块的UnInit函数，而
    //其他模块可能在本模块释放前已经释放，再调用UnInit会出错
    //UnInit();
}

BOOL CGameServer::InitDB(void)
{
    BOOL nResult = 0;
    MYSQL* pMysql = NULL;
    MYSQL* pResMysql = NULL;

    pMysql = mysql_init(NULL);
    LOG_PROCESS_ERROR(pMysql);

    pResMysql = mysql_real_connect(
                    pMysql,
                    "127.0.0.1",
                    "root",
                    "88888888",
                    NULL,
                    3306,
                    NULL,
                    CLIENT_MULTI_STATEMENTS | CLIENT_MULTI_RESULTS);
    LOG_PROCESS_ERROR(pResMysql);

    if(!lpDBExist(pMysql, "lzpl_test001"))
    {
        nResult = lpDBCreate(pMysql, "lzpl_test001");
        LOG_PROCESS_ERROR(nResult);
    }

    nResult = lpDBUpdate(pMysql, "lzpl_test001", "", 1);
    LOG_PROCESS_ERROR(nResult);

    if(pMysql) mysql_close(pMysql);
    return TRUE;

Exit0:

    if(pMysql) mysql_close(pMysql);
    return FALSE;
}

BOOL LPAPI CGameServer::Init(void)
{
    LPINT32 nResult = 0;
    NET_CONFIG stNetConfig;

    //设置正在初始化状态
    SetServerState(eServerState_Initing);

    // 加载配置
    nResult = g_LoadGlobalConfig(g_szGlobalConfigFileName);
    LOG_PROCESS_ERROR(nResult);

    //初始化DB
    nResult = InitDB();
    LOG_PROCESS_ERROR(nResult);

    //创建内部消息分发处理对象
    m_pGSInternalMessageHandler = lp_make_shared<CGSMessageHandler>();
    LOG_PROCESS_ERROR(m_pGSInternalMessageHandler);
    nResult = m_pGSInternalMessageHandler->Init();
    LOG_PROCESS_ERROR(nResult);

    //创建内部消息解析对象
    m_pGSInternalPacketParser = lp_make_shared<CGSInternalPacketParser>();
    LOG_PROCESS_ERROR(m_pGSInternalPacketParser);

    //创建外部消息解析对象
    m_pGSExternalPacketParser = lp_make_shared<CGSExternalPacketParser>();
    LOG_PROCESS_ERROR(m_pGSExternalPacketParser);

    //网络模块初始化
    nResult = ILPNet::GlobalInit();
    LOG_PROCESS_ERROR(nResult);

    //创建Net对象
    stNetConfig = g_GlobalConfig.Server.Gs.Net;
    m_pNet = ILPNet::CreateNetModule(m_pGSInternalMessageHandler, &stNetConfig);
    LOG_PROCESS_ERROR(m_pNet != nullptr);

    //创建GT连接器对象
    m_pConnector = m_pNet->CreateConnectorCtrl(m_pGSInternalPacketParser);
    LOG_PROCESS_ERROR(m_pConnector != nullptr);

    nResult = m_pConnector->Start(g_GlobalConfig.Server.Gt.szListenIp, g_GlobalConfig.Server.Gt.dwListenPort, TRUE);
    LOG_PROCESS_ERROR(nResult);

    //创建监听器对象
    //m_pListener = m_pNet->CreateListenerCtrl(m_pGSInternalPacketParser);
    //LOG_PROCESS_ERROR(m_pListener);

    //nResult = m_pListener->Start(g_GlobalConfig.Server.Gs.szListenIp, g_GlobalConfig.Server.Gs.dwListenPort, TRUE);
    //LOG_PROCESS_ERROR(nResult);

    //游戏逻辑模块初始化
    nResult = CGameLogic::Instance().Init();
    LOG_PROCESS_ERROR(nResult);

    //设置初始化完毕状态
    SetServerState(eServerState_SelfReady);

    return TRUE;

Exit0:

    nResult = UnInit();
    LOG_CHECK_ERROR(nResult);

    return FALSE;
}

BOOL LPAPI CGameServer::UnInit(void)
{
    LPINT32 nResult = 0;

    SetServerState(eServerState_UnIniting);

    nResult = CGameLogic::Instance().UnInit();
    LOG_CHECK_ERROR(nResult);

    nResult = m_pGSInternalMessageHandler->UnInit();
    LOG_CHECK_ERROR(nResult);
    m_pGSInternalMessageHandler = nullptr;

    m_pGSInternalPacketParser = nullptr;
    m_pGSExternalPacketParser = nullptr;

    //删除释放net对象
    ILPNet::DeleteNetModule(m_pNet);

    SetServerState(eServerState_UnInited);

    return TRUE;
}

BOOL LPAPI CGameServer::MainLoop(void)
{
    LPINT32 nResult = 0;

    nResult = m_pNet->Run(30);
    if(nResult)
    {
        lpSleep(1);
    }

    return TRUE;
}

LPUINT32 LPAPI CGameServer::GetServerState(void)
{
    return m_dwServerState;
}

void CGameServer::SetServerState(LPUINT32 dwServerState)
{
    m_dwServerState = dwServerState;
}

void CGameServer::Close(void)
{
    LPUINT64 qwTickStart = 0;

    //停止所有监听器和连接器
    if(m_pListener != nullptr)
    {
        m_pListener->Close();
        m_pListener = nullptr;
    }

    if(m_pConnector != nullptr)
    {
        m_pConnector->Close();
        m_pConnector = nullptr;
    }

    //关闭所有链接
    m_pGSInternalMessageHandler->CloseAllSocker();

    //等待所有链接关闭
    while(m_pGSInternalMessageHandler->GetSockerCount() > 0)
    {
        MainLoop();
    }

    SetServerState(eServerState_Closed);
}

void _Tabfile_Test(void)
{
    LPINT32 nResult = 0;
    char szFilePath[MAX_PATH];

    lpStrCpyN(szFilePath, lpGetWorkingDir(), MAX_PATH);
    lpStrCatN(szFilePath, "\\config\\data\\tabtest.txt", MAX_PATH);

    LPTabFile oTabFile;

    nResult = oTabFile.Init(szFilePath, 0, TRUE);
    LOG_PROCESS_ERROR(nResult);

    LPINT32 nID = 0;
    char szName[128] = { 0 };
    char szBeginTime[128] = { 0 };
    char szEndTime[128] = { 0 };

    for(LPINT32 nRowIndex = 0; nRowIndex < oTabFile.GetRowCount(); nRowIndex++)
    {
        nResult = oTabFile.ReadData(0, nRowIndex, nID);
        LOG_PROCESS_ERROR(nResult);

        nResult = oTabFile.ReadData(1, nRowIndex, szName, 128);
        LOG_PROCESS_ERROR(nResult);

        nResult = oTabFile.ReadData(2, nRowIndex, szBeginTime, 128);
        LOG_PROCESS_ERROR(nResult);

        nResult = oTabFile.ReadData(3, nRowIndex, szEndTime, 128);
        LOG_PROCESS_ERROR(nResult);

        LPPRINTF("%d\t%s\t%s\t%s\n", nID, szName, szBeginTime, szEndTime);
    }
    LPPRINTF("\n\n");
    oTabFile.UnInit();

    nResult = oTabFile.Init(szFilePath, 0, FALSE);
    for(LPINT32 nRowIndex = 0; nRowIndex < oTabFile.GetRowCount(); nRowIndex++)
    {
        nResult = oTabFile.ReadData("id", nRowIndex, nID);
        LOG_PROCESS_ERROR(nResult);

        nResult = oTabFile.ReadData("name", nRowIndex, szName, 128);
        LOG_PROCESS_ERROR(nResult);

        nResult = oTabFile.ReadData("beginTime", nRowIndex, szBeginTime, 128);
        LOG_PROCESS_ERROR(nResult);

        nResult = oTabFile.ReadData("endTime", nRowIndex, szEndTime, 128);
        LOG_PROCESS_ERROR(nResult);

        LPPRINTF("%d\t%s\t%s\t%s\n", nID, szName, szBeginTime, szEndTime);
    }

Exit0:

    oTabFile.UnInit();
    return;
}

size_t _Test_CurlOnWrite(void *ptr, size_t size, size_t nmemb, void *stream)
{
    char szRecvData[4096];
    char* pWriteData = NULL;

    LOG_PROCESS_ERROR(ptr);
    LOG_PROCESS_ERROR(size * nmemb < sizeof(szRecvData));

    pWriteData = (char*)stream;

    memcpy_s(szRecvData, sizeof(szRecvData) - 1, ptr, size * nmemb);
    szRecvData[size * nmemb] = '\0';

    INF("%s: \n%s", __FUNCTION__, szRecvData);

Exit0:
    return size * nmemb;
}

size_t _Test_CurlOnRead(void *ptr, size_t size, size_t nmemb, void *stream)
{
    char szRecvData[4096];
    char* pReadData = NULL;

    LOG_PROCESS_ERROR(ptr);
    LOG_PROCESS_ERROR(size * nmemb < sizeof(szRecvData));

    pReadData = (char*)stream;

    memcpy_s(szRecvData, sizeof(szRecvData) - 1, ptr, size * nmemb);
    szRecvData[size * nmemb] = '\0';

    INF("%s: \n%s", __FUNCTION__, szRecvData);

Exit0:
    return size * nmemb;
}

BOOL _Test_Curl(void)
{
    LPINT32 nResult = 0;

    char* pVersion = NULL;
    CURL* pEasyCurl = NULL;
    char szRead[1024] = "read data";
    char szWrite[1024] = "write data";

    lpSleep(1500);

    nResult = curl_global_init(CURL_GLOBAL_ALL);
    LOG_PROCESS_ERROR(nResult == CURLE_OK);

    pVersion = curl_version();
    LOG_PROCESS_ERROR(pVersion);

    pEasyCurl = curl_easy_init();
    LOG_PROCESS_ERROR(pEasyCurl);

    //nResult = curl_easy_setopt(pEasyCurl, CURLOPT_URL, "http://127.0.0.1:6661/{\"id\":\"version\"}");
    nResult = curl_easy_setopt(pEasyCurl, CURLOPT_URL, "http://www.baidu.com");
    LOG_PROCESS_ERROR(nResult == CURLE_OK);

    curl_easy_setopt(pEasyCurl, CURLOPT_NOSIGNAL, 1);
    LOG_PROCESS_ERROR(nResult == CURLE_OK);

    curl_easy_setopt(pEasyCurl, CURLOPT_CONNECTTIMEOUT, 3);
    LOG_PROCESS_ERROR(nResult == CURLE_OK);

    //curl_easy_setopt(pEasyCurl, CURLOPT_TIMEOUT, 10);
    //LOG_PROCESS_ERROR(nResult == CURLE_OK);

    curl_easy_setopt(pEasyCurl, CURLOPT_READFUNCTION, NULL);
    LOG_PROCESS_ERROR(nResult == CURLE_OK);

    //curl_easy_setopt(pEasyCurl, CURLOPT_READFUNCTION, _Test_CurlOnRead);
    //LOG_PROCESS_ERROR(nResult == CURLE_OK);

    //curl_easy_setopt(pEasyCurl, CURLOPT_READDATA, (void *)szRead);
    //LOG_PROCESS_ERROR(nResult == CURLE_OK);

    curl_easy_setopt(pEasyCurl, CURLOPT_WRITEFUNCTION, _Test_CurlOnWrite);
    LOG_PROCESS_ERROR(nResult == CURLE_OK);

    curl_easy_setopt(pEasyCurl, CURLOPT_WRITEDATA, (void *)szWrite);
    LOG_PROCESS_ERROR(nResult == CURLE_OK);

    nResult = curl_easy_perform(pEasyCurl);
    LOG_PROCESS_ERROR(nResult == CURLE_OK);

    curl_easy_cleanup(pEasyCurl);

    curl_global_cleanup();

    return TRUE;
Exit0:
    return FALSE;
}

BOOL _Test_MeshObjLoad(void)
{
    LPINT32 nResult = 0;
    FILE_LIST fileList;
    CNavMeshObjLoader oNavMeshObjLoader;

    nResult = oNavMeshObjLoader.Init();
    LOG_PROCESS_ERROR(nResult);

    //FLOAT f1 = 12345674.9012345678f;
    //FLOAT f2 = f1;
    //FLOAT f3 = f2;
    //DOUBLE d1 = 1234567890123.456489012346;

    //LPPRINTF("f1 = %.10f\n", f1);
    //LPPRINTF("f2 = %.10f\n", f2);
    //LPPRINTF("f3 = %.10f\n", f3);

    //LPPRINTF("d1 = %.10lf\n", d1);

    nResult = fileList.ScanDirectory("config/scene", ".obj", TRUE);
    LOG_PROCESS_ERROR(nResult);

    nResult = oNavMeshObjLoader.Load("config/scene/01 Exported NavMesh.obj");
    LOG_PROCESS_ERROR(nResult);

    //for (LPINT32 i = 0; i < fileList.dwSize; ++i)
    //{
    //  nResult = oNavMeshObjLoader.Load(fileList.szFiles[i]);
    //  LOG_PROCESS_ERROR(nResult);
    //}

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

    if(oShareMemory.IsNew())
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

BOOL _Test(void)
{
    LPINT32 nResult = 0;

    __TRY__
    {
        nResult = _Test_ShareMem();
        LOG_PROCESS_ERROR(nResult);
    }
    __EXCEPT__
    {

    }

    LOG_PROCESS_ERROR(TRUE);

    return TRUE;
Exit0:
    return FALSE;
}

int main(int argc, char* argv[])
{
    LPINT32 nResult = 0;
    LOG_CONFIG stLogConfig;

    RedefinedConsole();

    // 使用LZPL，必须在程序运行时先初始化
    nResult = lpLoadLogConfig(lpGetExeDir(), "logconfig.xml", stLogConfig);
    PRINTF_PROCESS_ERROR(nResult);

#   ifdef _DEBUG
    {
        nResult = lpInitLzpl(stLogConfig);
    }
#   else
    {
        nResult = lpInitLzpl(stLogConfig);
    }
#   endif

    PRINTF_PROCESS_ERROR(nResult);

    nResult = CGameServer::Instance().Init();
    LOG_PROCESS_ERROR(nResult);

    nResult = LPDumpMgr::Instance().Init(NULL, "log");
    LOG_PROCESS_ERROR(nResult);

    LPPRINTF("gameserver start success !\n");
    IMP("gameserver start success !");

    nResult = _Test();
    LOG_PROCESS_ERROR(nResult);

    //CGameServer::Instance().SetServerState(eServerState_Closing);
    while(CGameServer::Instance().GetServerState() != eServerState_Closing)
    {
        __TRY__
        {
            nResult = CGameServer::Instance().MainLoop();
            LOG_CHECK_ERROR(nResult);
        }
        __EXCEPT__
        {

        }
    }

    CGameServer::Instance().Close();

Exit0:

    CGameServer::Instance().UnInit();

    LPPRINTF("gameserver exit success !\n");
    IMP("gameserver exit success !");

    LPPRINTF("\nEnter any key to exit !\n");
    getchar();

    return 0;
}
