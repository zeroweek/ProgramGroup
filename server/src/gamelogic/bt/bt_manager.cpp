#include "bt_manager.h"
#include "lp_processerror.h"
#include "bt_action.h"
#include "bt_event.h"
#include "gamelogic.h"



SINGLETON_IMPLEMENT(CBTManager)

static char* s_szOwnerNameList[btotTotal] =
{
    "Invalid",
    "Invalid1",
    "Invalid2",
};


CBTManager::CBTManager()
{
    m_nActionIdGenerator = 0;
    m_pCurrentCtrl = NULL;
}

CBTManager::~CBTManager()
{
    UnInit();
}

LPINT32 CBTManager::_BTNodeFunc(BT_CTRL* pCtrl, BT_NODE* pNode)
{
    LPINT32 nResult = btrvError;
    LPINT32 nLuaResult = 0;
    LPINT32 nParam[BT_MAX_PARAM];
    char szFuncName[COMMON_NAME_LEN];
    char szNodeInfo[256];
    BT_CUSTOM_CTRL* pCustomCtrl = (BT_CUSTOM_CTRL*)pCtrl;

    LOG_PROCESS_ERROR(pCustomCtrl);
    LOG_PROCESS_ERROR(pNode);
    LOG_PROCESS_ERROR(pNode->type > btntInvalid && pNode->type < btntTotal);
    LOG_PROCESS_ERROR(Instance().m_ActionInfoList[pNode->type].eOwnerMask & (1 << pCustomCtrl->eOwnerType));

    // prepare params
    for(LPINT32 nParamIndex = 0; nParamIndex < BT_MAX_PARAM; ++nParamIndex)
    {
        if(pNode->luaParamMask & (1 << nParamIndex))
        {
            snprintf(szFuncName, COMMON_NAME_LEN, "ParamFunc%d", pNode->params[nParamIndex]);

            LOG_PROCESS_ERROR(pCustomCtrl->poScript);
            Instance().m_pCurrentCtrl = pCustomCtrl;
            nLuaResult = pCustomCtrl->poScript->CallFunction(szFuncName, "o:d",
                         pCustomCtrl->poOwner, s_szOwnerNameList[pCustomCtrl->eOwnerType], nParam + nParamIndex);
            Instance().m_pCurrentCtrl = NULL;
        }
        else
        {
            nParam[nParamIndex] = pNode->params[nParamIndex];
        }
    }

    // call action func
    if(Instance().m_ActionInfoList[pNode->type].bIsLuaFunc &&
            Instance().m_ActionInfoList[pNode->type].szName[0])
    {
        LOG_PROCESS_ERROR(pCustomCtrl->poScript);
        Instance().m_pCurrentCtrl = pCustomCtrl;
        nLuaResult = pCustomCtrl->poScript->CallFunction(Instance().m_ActionInfoList[pNode->type].szName, "odddd:d",
                     pCustomCtrl->poOwner, s_szOwnerNameList[pCustomCtrl->eOwnerType], nParam[0], nParam[1], nParam[2], nParam[3], &nResult);
        Instance().m_pCurrentCtrl = NULL;
        LOG_PROCESS_ERROR(nLuaResult);
    }
    else if(Instance().m_ActionInfoList[pNode->type].pfnCallback)
    {
        nResult = Instance().m_ActionInfoList[pNode->type].pfnCallback(pCustomCtrl, nParam);
    }

    if(pCustomCtrl->debugging)
    {
        memset(szNodeInfo, 0, sizeof(szNodeInfo));
        for(LPINT32 i = 0; i < pCustomCtrl->runStackNodeCount - 1; ++i)
        {
            szNodeInfo[i] = '\t';
        }
        if(Instance().GetBTActionName(pNode->type))
        {
            snprintf(szNodeInfo + pCustomCtrl->runStackNodeCount - 1, 200, "%s(%d, %d, %d, %d):%d",
                     Instance().GetBTActionName(pNode->type), nParam[0], nParam[1], nParam[2], nParam[3], nResult);
            INF(szNodeInfo);
        }
    }

Exit0:

    if(nResult == btrvError)
    {
        if(pNode && Instance().GetBTActionName(pNode->type) &&
                pCustomCtrl && pCustomCtrl->poOwner)
        {
            WRN("%s(%d, %d, %d, %d) error, event %d", Instance().GetBTActionName(pNode->type), pNode->params[0], pNode->params[1], pNode->params[2],
                pNode->params[3], pCustomCtrl->eEventType);
        }
        else if(pNode)
        {
            WRN("run unknown bt node %d(%d, %d, %d, %d) error", pNode->type, pNode->params[0], pNode->params[1], pNode->params[2], pNode->params[3]);
        }
        else
        {
            WRN("run unknown bt node error, node NULL");
        }
    }

    return nResult;
}

BOOL CBTManager::Init(void)
{
    std::pair<MAP_ACTION_NAME_2_INFO::iterator, bool> InsRet;

    m_nActionIdGenerator = 0;
    m_pCurrentCtrl = NULL;

    lpBTRegisterUserDefinedNodeFunc(_BTNodeFunc);
    m_nActionIdGenerator = btatEnd;

    for(LPINT32 nIndex = 0; nIndex < btatEnd; ++nIndex)
    {
        if(m_ActionInfoList[nIndex].szName[0])
        {
            InsRet = m_mapActionName2Info.insert(std::make_pair(
                    m_ActionInfoList[nIndex].szName, m_ActionInfoList + nIndex));
            LOG_PROCESS_ERROR(InsRet.second);
        }

        if(m_ActionInfoList[nIndex].nActionId != nIndex)
        {
            FTL("error bt action id (%d : %d : %s)", nIndex, m_ActionInfoList[nIndex].nActionId, m_ActionInfoList[nIndex].szName);
            LOG_PROCESS_ERROR(FALSE);
        }
    }

    LOG_PROCESS_ERROR(sizeof(g_BTEventName) / sizeof(g_BTEventName[0]) == btetTotal);

    return TRUE;
Exit0:
    return FALSE;
}

BOOL CBTManager::UnInit(void)
{
    MAP_ACTION_NAME_2_INFO::iterator it;

    for(it = m_mapActionName2Info.begin(); it != m_mapActionName2Info.end();)
    {
        m_mapActionName2Info.erase(it++);
    }

    return TRUE;
}

BOOL CBTManager::Reload(void)
{
    return FALSE;
}

BT_NODE* CBTManager::CreateBTNode(BT_NODE* pParent, LPINT32 nType, LPINT32 nTag, const char * pcszName, LPINT32 nParam0, LPINT32 nParam1, LPINT32 nParam2, LPINT32 nParam3, LPINT32 nFlag)
{
    LPINT32 nResult = 0;
    BT_NODE* pResult = NULL;
    const char* pcszActionName = NULL;

    pResult = CGameLogic::Instance().NewBTNode();
    LOG_PROCESS_ERROR(pResult);

    pResult->type = nType;
    pResult->nodeTag = nTag;

    pResult->params[0] = nParam0;
    pResult->params[1] = nParam1;
    pResult->params[2] = nParam2;
    pResult->params[3] = nParam3;
    pResult->flag = nFlag;
    pResult->childCount = 0;
    pResult->pChild = NULL;
    pResult->pSibling = NULL;

    if(pParent)
    {
        nResult = lpBTAddChild(pParent, pResult);
        LOG_PROCESS_ERROR(nResult);
    }

    return pResult;

Exit0:

    if(pResult)
    {
        CGameLogic::Instance().DelBTNode(pResult);
    }

    return NULL;
}

BOOL CBTManager::DestroyBTTree(BT_NODE* pRoot, LPINT32 nLayer)
{
    LPINT32 nResult = 0;

    LOG_PROCESS_ERROR(pRoot);
    LOG_PROCESS_ERROR(nLayer < BT_MAX_LAYER);

    if(pRoot->pChild)
    {
        nResult = DestroyBTTree(pRoot->pChild, nLayer + 1);
        LOG_CHECK_ERROR(nResult);
    }

    if(pRoot->pSibling)
    {
        nResult = DestroyBTTree(pRoot->pSibling, nLayer + 1);
        LOG_CHECK_ERROR(nResult);
    }

    nResult = CGameLogic::Instance().DelBTNode(pRoot);

    return TRUE;
Exit0:
    return FALSE;
}

BT_NODE* CBTManager::CopyBTTree(BT_NODE* pRoot)
{
    LPINT32 nResult = 0;
    BT_NODE* pResult = NULL;

    PROCESS_ERROR(pRoot);

    pResult = CGameLogic::Instance().NewBTNode();
    LOG_PROCESS_ERROR(pResult);

    pResult->type = pRoot->type;
    memcpy(pResult->params, pRoot->params, sizeof(pResult->params));
    pResult->flag = pRoot->flag;
    pResult->childCount = pResult->childCount;
    pResult->pChild = NULL;
    pResult->pSibling = NULL;

    if(pRoot->pChild)
    {
        pResult->pChild = CopyBTTree(pRoot->pChild);
        LOG_PROCESS_ERROR(pResult->pChild);
    }

    if(pRoot->pSibling)
    {
        pResult->pSibling = CopyBTTree(pRoot->pSibling);
        LOG_PROCESS_ERROR(pResult->pSibling);
    }

    return pResult;

Exit0:

    if(pResult)
    {
        DestroyBTTree(pResult);
    }

    return NULL;
}

void CBTManager::PrintBTTree(BT_NODE* pRoot, LPINT32 nLayer)
{
    LPINT32 nChildIndex = 0;
    BT_NODE* pNode = NULL;
    char szTab[MAX_PATH] = { 0 };
    const char* pcszName = NULL;

    LOG_PROCESS_ERROR(pRoot);
    LOG_PROCESS_ERROR(nLayer < BT_MAX_LAYER);

    memset(szTab, ' ', nLayer * 4);

    pcszName = GetBTActionName(pRoot->type);
    INF("%s %s (%d,%d,%d,%d)", szTab, pcszName, pRoot->params[0], pRoot->params[1], pRoot->params[2], pRoot->params[3]);

    if(pRoot->pChild)
    {
        PrintBTTree(pRoot->pChild, nLayer + 1);
    }

    if(pRoot->pSibling)
    {
        PrintBTTree(pRoot->pSibling, nLayer + 1);
    }

Exit0:
    return;
}

LPINT32 CBTManager::FindBTActionByName(const char * pcszName)
{
    MAP_ACTION_NAME_2_INFO::iterator fit;

    LOG_PROCESS_ERROR(pcszName);

    fit = m_mapActionName2Info.find(pcszName);
    PROCESS_ERROR(fit != m_mapActionName2Info.end());
    PROCESS_ERROR(fit->second);

    return fit->second->nActionId;
Exit0:
    return btatInvalid;
}

LPINT32 CBTManager::RegisterBTAction(const char * pcszName)
{
    std::pair<MAP_ACTION_NAME_2_INFO::iterator, bool> InsRet;

    LOG_PROCESS_ERROR(pcszName);

    m_ActionInfoList[m_nActionIdGenerator].nActionId = m_nActionIdGenerator;
    lpStrCpyN(m_ActionInfoList[m_nActionIdGenerator].szName, pcszName, MAX_FILE_NAME);
    m_ActionInfoList[m_nActionIdGenerator].bIsLuaFunc = TRUE;
    m_ActionInfoList[m_nActionIdGenerator].eOwnerMask = btotAll;

    InsRet = m_mapActionName2Info.insert(std::make_pair(
            m_ActionInfoList[m_nActionIdGenerator].szName, m_ActionInfoList + m_nActionIdGenerator));
    LOG_PROCESS_ERROR(InsRet.second);

    return m_nActionIdGenerator++;
Exit0:
    return btntInvalid;
}

const char * CBTManager::GetBTActionName(LPINT32 nActionId)
{
    ACTION_INFO* pActionInfo = NULL;

    LOG_PROCESS_ERROR(nActionId > 0 && nActionId < btntTotal);

    pActionInfo = &m_ActionInfoList[nActionId];
    LOG_PROCESS_ERROR(pActionInfo->nActionId > 0);
    LOG_PROCESS_ERROR(pActionInfo->szName[0] != '\0');

    return pActionInfo->szName;
Exit0:
    return NULL;
}

BOOL CBTManager::CallBTAction(LPINT32 nBTNodeType, LPINT32 nParam[])
{
    LPINT32 nResult = btrvError;

    LOG_PROCESS_ERROR(m_pCurrentCtrl);
    LOG_PROCESS_ERROR(nBTNodeType > btntInvalid && nBTNodeType < btntTotal);
    LOG_PROCESS_ERROR(m_ActionInfoList[nBTNodeType].eOwnerMask & (1 << m_pCurrentCtrl->eOwnerType));
    LOG_PROCESS_ERROR(!m_ActionInfoList[nBTNodeType].bIsLuaFunc);
    LOG_PROCESS_ERROR(m_ActionInfoList[nBTNodeType].pfnCallback);

    nResult = m_ActionInfoList[nBTNodeType].pfnCallback(m_pCurrentCtrl, nParam);

Exit0:
    return nResult;
}

BOOL CBTManager::TestBTAction(void * pObj, LPINT32 nBTNodeType, LPINT32 nParam[])
{
    LPINT32 nResult = btrvError;
    LPINT32 nMask = 0;

    BT_CUSTOM_CTRL stCtrl;
    BT_NODE* pRoot = NULL;

    LOG_PROCESS_ERROR(pObj);

    pRoot = Instance().CreateBTNode(NULL, nBTNodeType, 0, NULL, nParam[0], nParam[1], nParam[2], nParam[3], 0);
    LOG_PROCESS_ERROR(pRoot);

    memset(&stCtrl, 0, sizeof(stCtrl));
    stCtrl.poScript = NULL;
    stCtrl.eRunningType = btrtInvalid;

    nMask = Instance().m_ActionInfoList[nBTNodeType].eOwnerMask;

    if(nMask && btotSceneObjectBit)
    {
        //stCtrl.poOwner =
        stCtrl.eOwnerType = btotSceneObject;
    }
    else if(nMask & btotCharacterBit)
    {
        //stCtrl.poOwner =
        stCtrl.eOwnerType = btotCharacter;
    }
    else if(nMask & btotNpcBit)
    {
        //stCtrl.poOwner =
        stCtrl.eOwnerType = btotNpc;
    }
    else if(nMask & btotSceneBit)
    {
        //stCtrl.poOwner =
        stCtrl.eOwnerType = btotScene;
    }
    LOG_PROCESS_ERROR(stCtrl.poOwner);

    memset(stCtrl.nodeRunStack, 0, sizeof(stCtrl.nodeRunStack));
    stCtrl.nodeRunStack[0].pNode = pRoot;
    stCtrl.runStackNodeCount = 1;

    nResult = CBTManager::_BTNodeFunc(&stCtrl, pRoot);
    DBG("bt action %s(%s)'s run result = %d", GetBTActionName(nBTNodeType), nBTNodeType, nResult);

Exit0:

    if(pRoot)
    {
        m_oInstance.DestroyBTTree(pRoot);
    }
    return nResult;
}

LPUINT32 CBTManager::GetCtrlRoleId()
{
    LOG_PROCESS_ERROR(m_pCurrentCtrl);

    return m_pCurrentCtrl->dwRoleId;
Exit0:
    return 0;
}
