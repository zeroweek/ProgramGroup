#include "lp_bt.h"
#include "lp_processerror.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN

static pfunBTUserDefinedNodeFunc gs_pfUserDefinedNodeFunc = NULL;


DECLARE BOOL LPAPI lpBTAddChild(BT_NODE* pRootNode, BT_NODE* pChild)
{
	LPINT32 nChildIndex = 0;
	BT_NODE* pNode = NULL;

	LOG_PROCESS_ERROR(pRootNode);
	LOG_PROCESS_ERROR(pChild);

	if (NULL == pRootNode->pChild)
	{
		LOG_PROCESS_ERROR(0 == pRootNode->childCount);
		pRootNode->pChild = pChild;
	} 
	else
	{
		pNode = pRootNode->pChild;

		nChildIndex = 0;
		while (pNode)
		{
			if (NULL == pNode->pSibling)
			{
				LOG_PROCESS_ERROR(nChildIndex + 1 == pRootNode->childCount);

				pNode->pSibling = pChild;
				break;
			}

			++nChildIndex;
			LOG_PROCESS_ERROR(nChildIndex < BT_MAX_CHILD_COUNT);

			pNode = pNode->pSibling;
		}
	}

	pRootNode->childCount++;

	return TRUE;
Exit0:
	return FALSE;
}

DECLARE BOOL LPAPI lpBTRemoveChild(BT_NODE* pRootNode, BT_NODE* pChild)
{
	LPINT32 nChildIndex = 0;
	BT_NODE* pNode = NULL;

	LOG_PROCESS_ERROR(pRootNode);
	LOG_PROCESS_ERROR(pChild);

	LOG_PROCESS_ERROR(pRootNode->pChild);

	if (pChild == pRootNode->pChild)
	{
		pRootNode->pChild = pChild->pSibling;
	}
	else
	{
		pNode = pRootNode->pChild;

		nChildIndex = 0;
		while (pNode)
		{
			if (pChild == pNode->pSibling)
			{
				LOG_PROCESS_ERROR(nChildIndex + 1 < pRootNode->childCount);

				pNode->pSibling = pNode->pSibling->pSibling;
				break;
			}

			++nChildIndex;
			LOG_PROCESS_ERROR(nChildIndex < BT_MAX_CHILD_COUNT);

			pNode = pNode->pSibling;
		}
	}

	pRootNode->childCount--;

	return TRUE;
Exit0:
	return FALSE;
}

BT_NODE* LPAPI lpBTFindChildAtTag(BT_NODE* pRootNode, LPINT32 nTag, BOOL bDepthFirst = FALSE)
{
	BT_NODE* pResult = NULL;
	BT_NODE* pNode = NULL;
	BT_NODE* pChild = NULL;

	BT_NODE* aChildArray[BT_MAX_CHILD_COUNT] = { NULL };
	LPINT32 nChildCount = 0;
	LPINT32 nChildIndex = 0;

	LOG_PROCESS_ERROR(pRootNode);
	LOG_PROCESS_ERROR(nTag > 0);

	pNode = pRootNode->pChild;
	if (!bDepthFirst)
	{
		while (pNode)
		{
			if (pNode->nodeTag == nTag)
			{
				pResult = pNode;
				break;
			}

			aChildArray[nChildCount++] = pNode;

			pNode = pNode->pSibling;
			LOG_PROCESS_ERROR(nChildCount < BT_MAX_CHILD_COUNT);
		}

		for (nChildIndex = 0; nChildIndex < nChildCount; ++nChildIndex)
		{
			pChild = lpBTFindChildAtTag(aChildArray[nChildIndex], nTag);
			if (pChild && pChild->nodeTag == nTag)
			{
				pResult = pChild;
				break;
			}
		}
	}
	else
	{
		while (pNode)
		{
			if (pNode->nodeTag == nTag)
			{
				pResult = pNode;
				break;
			}

			pChild = lpBTFindChildAtTag(pNode, nTag);
			if (pNode && pNode->nodeTag == nTag)
			{
				pResult = pChild;
				break;
			}

			pNode = pNode->pSibling;
		}
	}

	return pResult;
Exit0:
	return NULL;
}

DECLARE BOOL LPAPI lpBTInsertChildAtTag(BT_NODE* pParent, LPINT32 nTag, BT_NODE* pNewChild)
{
	BT_NODE* pNode = NULL;
	LPINT32 nChildIndex = 0;

	LOG_PROCESS_ERROR(pParent);
	LOG_PROCESS_ERROR(pNewChild);
	LOG_PROCESS_ERROR(NULL == pNewChild->pSibling);

	if (NULL == pParent->pChild)
	{
		pParent->pChild = pNewChild;
		++pParent->childCount;

		PROCESS_SUCCESS(TRUE);
	}

	if (pParent->pChild->nodeTag >= nTag)
	{
		pNewChild->pSibling = pParent->pChild;

		pParent->pChild = pNewChild;
		++pParent->childCount;
	}
	else
	{
		pNode = pParent->pChild;

		nChildIndex = 0;
		while (pNode)
		{
			LOG_PROCESS_ERROR(pNode != pNewChild);

			if (NULL == pNode->pSibling)
			{
				LOG_PROCESS_ERROR(nChildIndex + 1 == pParent->childCount);
				pNode->pSibling = pNewChild;
				++pParent->childCount;
				break;
			}
			else if(pNode->pSibling->nodeTag >= nTag)
			{
				LOG_PROCESS_ERROR(nChildIndex + 1 < pParent->childCount);
				pNewChild->pSibling = pNode->pSibling;

				pNode->pSibling = pNewChild;
				++pParent->childCount;
				break;

			}

			++nChildIndex;
			LOG_PROCESS_ERROR(nChildIndex < BT_MAX_CHILD_COUNT);
			
			LOG_PROCESS_ERROR(pNode != pNode->pSibling);
			pNode = pNode->pSibling;
		}
	}

Exit1:
	return TRUE;
Exit0:
	return FALSE;
}

DECLARE BT_NODE* LPAPI lpBTGetChild(BT_NODE* pRootNode, LPINT32 nIndex)
{
	BT_NODE* pNode = NULL;
	BT_NODE* pResult = NULL;

	LOG_PROCESS_ERROR(pRootNode);
	LOG_PROCESS_ERROR(nIndex >= 0 && nIndex < pRootNode->childCount);

	LOG_PROCESS_ERROR(pRootNode->pChild);

	pNode = pRootNode->pChild;
	while (pNode && nIndex--)
	{
		pNode = pNode->pSibling;
	}

	pResult = pNode;

	return pResult;
Exit0:
	return NULL;
}

inline BOOL LPAPI lpBTCtrlPushNode(BT_CTRL& ctrl, BT_NODE* pNode)
{
	LOG_PROCESS_ERROR(ctrl.runStackNodeCount < BT_MAX_CALL_STACK_NODE);

	ctrl.nodeRunStack[ctrl.runStackNodeCount].pNode = pNode;
	ctrl.nodeRunStack[ctrl.runStackNodeCount].pChild = NULL;
	++ctrl.runStackNodeCount;

	return TRUE;
Exit0:
	return FALSE;
}

inline BOOL LPAPI lpBTCtrlPopNode(BT_CTRL& ctrl)
{
	LOG_PROCESS_ERROR(ctrl.runStackNodeCount > 0);
	ctrl.runStackNodeCount--;

	return TRUE;
Exit0:
	return FALSE;
}

DECLARE void LPAPI lpBTRunBtCtrl(BT_CTRL & ctrl)
{
	LPINT32 nResult = 0;
	LPINT32 nRepeatTimes = 0;
	LPINT32 nTotalWeight = 0;
	LPINT32 nCounter = 0;
	LPINT32 nRandom = 0;
	BT_NODE* pNode = NULL;

	while (ctrl.runStackNodeCount > 0)
	{
		char szNodeInfo[256];
		BT_RUN_NODE& currentRunNode = ctrl.nodeRunStack[ctrl.runStackNodeCount - 1];

		LOG_PROCESS_ERROR(currentRunNode.pNode);

		if (ctrl.debugging)
		{
			for (LPINT32 i = 0; i < ctrl.runStackNodeCount - 1; ++i)
			{
				szNodeInfo[i] = '\t';
			}
		}

		switch (currentRunNode.pNode->type)
		{
			case btntSelector:
				{
					if (currentRunNode.pChild == NULL)
					{
						if (ctrl.debugging)
						{
							sprintf_s(szNodeInfo + ctrl.runStackNodeCount - 1, 200, "btntSelector child %d", currentRunNode.pNode->childCount);
							INF(szNodeInfo);
						}

						if (currentRunNode.pNode->pChild)
						{
							currentRunNode.pChild = currentRunNode.pNode->pChild;
							ctrl.returnValue = btrvError;

							nResult = lpBTCtrlPushNode(ctrl, currentRunNode.pNode->pChild);
							LOG_PROCESS_ERROR(nResult);
						}
						else
						{
							ctrl.returnValue = btrvFail;

							if (ctrl.debugging)
							{
								sprintf_s(szNodeInfo + ctrl.runStackNodeCount - 1, 200, "btntSelector return %d", ctrl.returnValue);
								INF(szNodeInfo);
							}

							nResult = lpBTCtrlPopNode(ctrl);
							LOG_PROCESS_ERROR(nResult);
						}
					}
					else if(currentRunNode.pChild->pSibling && ctrl.returnValue == btrvFail)
					{
						currentRunNode.pChild = currentRunNode.pChild->pSibling;

						nResult = lpBTCtrlPushNode(ctrl, currentRunNode.pChild);
						LOG_PROCESS_ERROR(nResult);
					}
					else
					{
						if (ctrl.debugging)
						{
							sprintf_s(szNodeInfo + ctrl.runStackNodeCount - 1, 200, "btntSelector return %d", ctrl.returnValue);
							INF(szNodeInfo);
						}

						nResult = lpBTCtrlPopNode(ctrl);
						LOG_PROCESS_ERROR(nResult);
					}
				}
				break;
			case btntSequencer:
				{
					if (currentRunNode.pChild == NULL)
					{
						if (ctrl.debugging)
						{
							sprintf_s(szNodeInfo + ctrl.runStackNodeCount - 1, 200, "btntSequencer child %d", currentRunNode.pNode->childCount);
							INF(szNodeInfo);
						}

						if (currentRunNode.pNode->pChild)
						{
							currentRunNode.pChild = currentRunNode.pNode->pChild;
							ctrl.returnValue = btrvError;

							nResult = lpBTCtrlPushNode(ctrl, currentRunNode.pNode->pChild);
							LOG_PROCESS_ERROR(nResult);
						}
						else
						{
							ctrl.returnValue = btrvSuccess;

							if (ctrl.debugging)
							{
								sprintf_s(szNodeInfo + ctrl.runStackNodeCount - 1, 200, "btntSequencer return %d", ctrl.returnValue);
								INF(szNodeInfo);
							}

							nResult = lpBTCtrlPopNode(ctrl);
							LOG_PROCESS_ERROR(nResult);
						}
					}
					else if (currentRunNode.pChild->pSibling && ctrl.returnValue == btrvSuccess)
					{
						currentRunNode.pChild = currentRunNode.pChild->pSibling;

						nResult = lpBTCtrlPushNode(ctrl, currentRunNode.pChild);
						LOG_PROCESS_ERROR(nResult);
					}
					else
					{
						if (ctrl.debugging)
						{
							sprintf_s(szNodeInfo + ctrl.runStackNodeCount - 1, 200, "btntSequencer return %d", ctrl.returnValue);
							INF(szNodeInfo);
						}

						nResult = lpBTCtrlPopNode(ctrl);
						LOG_PROCESS_ERROR(nResult);
					}
				}
				break;
			case btntParallel:
				{
					if (currentRunNode.pChild == NULL)
					{
						if (ctrl.debugging)
						{
							sprintf_s(szNodeInfo + ctrl.runStackNodeCount - 1, 200, "btntParallel child %d", currentRunNode.pNode->childCount);
							INF(szNodeInfo);
						}

						if (currentRunNode.pNode->pChild)
						{
							currentRunNode.pChild = currentRunNode.pNode->pChild;
							ctrl.returnValue = btrvError;

							nResult = lpBTCtrlPushNode(ctrl, currentRunNode.pNode->pChild);
							LOG_PROCESS_ERROR(nResult);
						}
						else
						{
							ctrl.returnValue = btrvSuccess;

							if (ctrl.debugging)
							{
								sprintf_s(szNodeInfo + ctrl.runStackNodeCount - 1, 200, "btntParallel return %d", ctrl.returnValue);
								INF(szNodeInfo);
							}

							nResult = lpBTCtrlPopNode(ctrl);
							LOG_PROCESS_ERROR(nResult);
						}
					}
					else if (currentRunNode.pChild->pSibling)
					{
						currentRunNode.pChild = currentRunNode.pChild->pSibling;

						nResult = lpBTCtrlPushNode(ctrl, currentRunNode.pChild);
						LOG_PROCESS_ERROR(nResult);
					}
					else
					{
						ctrl.returnValue = btrvSuccess;

						if (ctrl.debugging)
						{
							sprintf_s(szNodeInfo + ctrl.runStackNodeCount - 1, 200, "btntParallel return %d", ctrl.returnValue);
							INF(szNodeInfo);
						}

						nResult = lpBTCtrlPopNode(ctrl);
						LOG_PROCESS_ERROR(nResult);
					}
				}
				break;
			case btntDebug:
				{
					if (currentRunNode.pChild == NULL)
					{
						LOG_PROCESS_ERROR(currentRunNode.pNode->childCount == 1 && currentRunNode.pNode->pChild);
						currentRunNode.pChild = currentRunNode.pNode->pChild;
						ctrl.returnValue = btrvError;
						ctrl.debugging = TRUE;

						nResult = lpBTCtrlPushNode(ctrl, currentRunNode.pNode->pChild);
						LOG_PROCESS_ERROR(nResult);
					}
					else
					{
						ctrl.debugging = FALSE;
						nResult = lpBTCtrlPopNode(ctrl);
						LOG_PROCESS_ERROR(nResult);
					}
				}
				break;
			case btntNot:
				{
					if (currentRunNode.pChild == NULL)
					{
						if (ctrl.debugging)
						{
							sprintf_s(szNodeInfo + ctrl.runStackNodeCount - 1, 200, "btntNot child %d", currentRunNode.pNode->childCount);
							INF(szNodeInfo);
						}

						currentRunNode.pChild = currentRunNode.pNode->pChild;
						ctrl.returnValue = btrvError;

						nResult = lpBTCtrlPushNode(ctrl, currentRunNode.pNode->pChild);
						LOG_PROCESS_ERROR(nResult);
					}
					else
					{
						ctrl.returnValue = !ctrl.returnValue;

						if (ctrl.debugging)
						{
							sprintf_s(szNodeInfo + ctrl.runStackNodeCount - 1, 200, "btntNot return %d", ctrl.returnValue);
							INF(szNodeInfo);
						}

						nResult = lpBTCtrlPopNode(ctrl);
						LOG_PROCESS_ERROR(nResult);
					}
				}
				break;
			case btntRepeat:
				{
					if (currentRunNode.pChild == NULL)
					{
						if (currentRunNode.pNode->childCount > 0 && currentRunNode.pNode->pChild)
						{
							nRepeatTimes = currentRunNode.pNode->params[0];
							LOG_PROCESS_ERROR(nRepeatTimes > 0 && nRepeatTimes < BT_MAX_REPEAT_TIMES);
							LOG_PROCESS_ERROR(currentRunNode.pNode->childCount == 1);

							currentRunNode.pNode->repeatCounter = nRepeatTimes;

							if (ctrl.debugging)
							{
								sprintf_s(szNodeInfo + ctrl.runStackNodeCount - 1, 200, "btntRepeat times %d", currentRunNode.pNode->params[0]);
								INF(szNodeInfo);
							}

							currentRunNode.pNode = currentRunNode.pNode->pChild;
							ctrl.returnValue = btrvError;

							nResult = lpBTCtrlPushNode(ctrl, currentRunNode.pNode->pChild);
							LOG_PROCESS_ERROR(nResult);
						}
						else // if(pNode->childCount == 0 || NULL == nodeRunStack.pNode->pChild) // error ?
						{
							ctrl.returnValue = btrvSuccess;

							if (ctrl.debugging)
							{
								sprintf_s(szNodeInfo + ctrl.runStackNodeCount - 1, 200, "btntRepeat times %d do nothing return %d", 
									currentRunNode.pNode->params[0], ctrl.returnValue);
								INF(szNodeInfo);
							}

							nResult = lpBTCtrlPopNode(ctrl);
							LOG_PROCESS_ERROR(nResult);
						}
					}
					else
					{
						--currentRunNode.pNode->repeatCounter;
						if (ctrl.debugging)
						{
							sprintf_s(szNodeInfo + ctrl.runStackNodeCount - 1, 200, "btntRepeat times(%d/%d) return %d",
								currentRunNode.pNode->params[0] - currentRunNode.pNode->repeatCounter, currentRunNode.pNode->params[0], ctrl.returnValue);
							INF(szNodeInfo);
						}

						if (currentRunNode.pNode->repeatCounter > 0 && ctrl.returnValue == btrvSuccess)
						{
							currentRunNode.pChild = currentRunNode.pNode->pChild;
							nResult = lpBTCtrlPushNode(ctrl, currentRunNode.pNode->pChild);
							LOG_PROCESS_ERROR(nResult);
						}
						else
						{
							nResult = lpBTCtrlPopNode(ctrl);
							LOG_PROCESS_ERROR(nResult);
						}
					}
				}
				break;
			case btntRandom:
				{
					if (currentRunNode.pChild == NULL)
					{
						if (currentRunNode.pNode->childCount >= 1 && currentRunNode.pNode->pChild)
						{
							nTotalWeight = 0;
							nCounter = 0;
							pNode = currentRunNode.pNode->pChild;

							while (pNode)
							{
								LOG_PROCESS_ERROR(currentRunNode.pNode->params[nCounter] > 0);
								nTotalWeight += currentRunNode.pNode->params[nCounter];
								pNode = pNode->pSibling;
								++nCounter;
							}
							LOG_PROCESS_ERROR(nTotalWeight > 0);

							nRandom = rand() % nTotalWeight;
							nCounter = 0;
							pNode = currentRunNode.pNode->pChild;
							while (pNode)
							{
								nRandom -= currentRunNode.pNode->params[nCounter];
								if (nRandom < 0)
								{
									break;
								}

								pNode = pNode->pSibling;
								++nCounter;
							}

							if (ctrl.debugging)
							{
								sprintf_s(szNodeInfo + ctrl.runStackNodeCount - 1, 200, "btntRandom choose child %d on (%d/%d)", 
									nCounter, currentRunNode.pNode->params[nCounter], nTotalWeight);
								INF(szNodeInfo);
							}

							currentRunNode.pChild = pNode;
							ctrl.returnValue = btrvError;

							nResult = lpBTCtrlPushNode(ctrl, pNode);
							LOG_PROCESS_ERROR(nResult);
						}
						else // if(pNode->childCount == 0 || NULL == nodeRunStack.pNode->pChild) // error ?
						{
							ctrl.returnValue = btrvFail;

							if (ctrl.debugging)
							{
								sprintf_s(szNodeInfo + ctrl.runStackNodeCount - 1, 200, "btntRandom no child to choose return %d", ctrl.returnValue);
								INF(szNodeInfo);
							}

							nResult = lpBTCtrlPopNode(ctrl);
							LOG_PROCESS_ERROR(nResult);
						}
					}
					else
					{
						if (ctrl.debugging)
						{
							sprintf_s(szNodeInfo + ctrl.runStackNodeCount - 1, 200, "btntRepeat return %d", ctrl.returnValue);
							INF(szNodeInfo);
						}

						nResult = lpBTCtrlPopNode(ctrl);
						LOG_PROCESS_ERROR(nResult);
					}
				}
				break;
			default:
				{
					LOG_PROCESS_ERROR(gs_pfUserDefinedNodeFunc);

					ctrl.returnValue = gs_pfUserDefinedNodeFunc(&ctrl, currentRunNode.pNode);
					LOG_PROCESS_ERROR(ctrl.returnValue != btrvError);

					nResult = lpBTCtrlPopNode(ctrl);
					LOG_PROCESS_ERROR(nResult);

					PROCESS_SUCCESS(ctrl.returnValue == btrvRunning);
				}
				break;
		}

	}

	PROCESS_ERROR(ctrl.returnValue == btrvSuccess);

Exit1:

	return;

Exit0:

	if (ctrl.returnValue == btrvError)
	{
		if (ctrl.runStackNodeCount >= 1 && ctrl.nodeRunStack[ctrl.runStackNodeCount - 1].pNode)
		{
			ERR("*** bt node %d error!", ctrl.nodeRunStack[ctrl.runStackNodeCount - 1].pNode->type);
		}
		else
		{
			ERR("*** unknow bt node error !");
		}
	}

	ctrl.runStackNodeCount = 0;

	return;
}

DECLARE void lpBTRegisterUserDefinedNodeFunc(pfunBTUserDefinedNodeFunc pfNodeFunc)
{
	gs_pfUserDefinedNodeFunc = pfNodeFunc;
}









//end声明所处的名字空间

NS_LZPL_END
