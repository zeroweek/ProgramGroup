#include "testcase.h"
#include "lp_processerror.h"


struct TC_TEST_SIMPLE_LIST_NODE : SIMPLE_LIST_NODE
{
	INT_32 nIndex;

	TC_TEST_SIMPLE_LIST_NODE()
	{
		nIndex = 0;
	}
};

BOOL TC_TestSimpleList(void)
{
	INT_32 nResult = FALSE;
	LPSimpleList oSimpleList;

	TC_TEST_SIMPLE_LIST_NODE* tNode0 = nullptr;
	TC_TEST_SIMPLE_LIST_NODE* tNode1 = new TC_TEST_SIMPLE_LIST_NODE();
	TC_TEST_SIMPLE_LIST_NODE* tNode2 = new TC_TEST_SIMPLE_LIST_NODE();
	TC_TEST_SIMPLE_LIST_NODE* tNode3 = new TC_TEST_SIMPLE_LIST_NODE();
	TC_TEST_SIMPLE_LIST_NODE* tNode4 = new TC_TEST_SIMPLE_LIST_NODE();
	TC_TEST_SIMPLE_LIST_NODE* tNode5 = new TC_TEST_SIMPLE_LIST_NODE();
	TC_TEST_SIMPLE_LIST_NODE* tNode6 = new TC_TEST_SIMPLE_LIST_NODE();
	TC_TEST_SIMPLE_LIST_NODE* tNode7 = new TC_TEST_SIMPLE_LIST_NODE();
	tNode1->nIndex = 1;
	tNode2->nIndex = 2;
	tNode3->nIndex = 3;
	tNode4->nIndex = 4;
	tNode5->nIndex = 5;
	tNode6->nIndex = 6;
	tNode7->nIndex = 7;

	nResult = oSimpleList.Append(tNode1);
	LOG_PROCESS_ERROR(nResult);
	nResult = oSimpleList.InsertBefore(tNode2, oSimpleList.Head());
	LOG_PROCESS_ERROR(nResult);
	nResult = oSimpleList.InsertAfter(tNode3, tNode2);
	LOG_PROCESS_ERROR(nResult);
	nResult = oSimpleList.InsertAfter(tNode4, tNode2);
	LOG_PROCESS_ERROR(nResult);
	nResult = oSimpleList.InsertBefore(tNode5, tNode4);
	LOG_PROCESS_ERROR(nResult);
	nResult = oSimpleList.InsertBefore(tNode6, tNode3);
	LOG_PROCESS_ERROR(nResult);
	nResult = oSimpleList.InsertAfter(tNode7, tNode4);

	/*
	1
	2 1
	2 3 1
	2 4 3 1
	2 5 4 3 1
	2 5 4 6 3 1
	2 5 4 7 6 3 1
	*/

	for (SIMPLE_LIST_NODE* pNode = oSimpleList.Head(); pNode != nullptr;)
	{
		TC_TEST_SIMPLE_LIST_NODE* ptTcNode = (TC_TEST_SIMPLE_LIST_NODE*)pNode;
		LOG_PROCESS_ERROR(ptTcNode);
		LPPRINTF("%d\n", ptTcNode->nIndex);

		pNode = pNode->pstNext;
	}
	LPPRINTF("\n");

	//tNode0 = (TC_TEST_SIMPLE_LIST_NODE*)oSimpleList.Pop();
	nResult = oSimpleList.Remove(tNode4);
	LOG_PROCESS_ERROR(nResult);

	for (SIMPLE_LIST_NODE* pNode = oSimpleList.Head(); pNode != nullptr;)
	{
		TC_TEST_SIMPLE_LIST_NODE* ptTcNode = (TC_TEST_SIMPLE_LIST_NODE*)pNode;
		LOG_PROCESS_ERROR(ptTcNode);
		LPPRINTF("%d\n", ptTcNode->nIndex);

		pNode = pNode->pstNext;
	}
	LPPRINTF("\n");

	//tNode0 = (TC_TEST_SIMPLE_LIST_NODE*)oSimpleList.Pop();
	nResult = oSimpleList.Remove(tNode6);
	LOG_PROCESS_ERROR(nResult);

	for (SIMPLE_LIST_NODE* pNode = oSimpleList.Head(); pNode != nullptr;)
	{
		TC_TEST_SIMPLE_LIST_NODE* ptTcNode = (TC_TEST_SIMPLE_LIST_NODE*)pNode;
		LOG_PROCESS_ERROR(ptTcNode);
		LPPRINTF("%d\n", ptTcNode->nIndex);

		pNode = pNode->pstNext;
	}
	LPPRINTF("\n");

	//tNode0 = (TC_TEST_SIMPLE_LIST_NODE*)oSimpleList.Pop();
	nResult = oSimpleList.Remove(tNode1);
	LOG_PROCESS_ERROR(nResult);

	for (SIMPLE_LIST_NODE* pNode = oSimpleList.Head(); pNode != nullptr;)
	{
		TC_TEST_SIMPLE_LIST_NODE* ptTcNode = (TC_TEST_SIMPLE_LIST_NODE*)pNode;
		LOG_PROCESS_ERROR(ptTcNode);
		LPPRINTF("%d\n", ptTcNode->nIndex);

		pNode = pNode->pstNext;
	}
	LPPRINTF("\n");

	//tNode0 = (TC_TEST_SIMPLE_LIST_NODE*)oSimpleList.Pop();
	nResult = oSimpleList.Remove(tNode2);
	LOG_PROCESS_ERROR(nResult);

	for (SIMPLE_LIST_NODE* pNode = oSimpleList.Head(); pNode != nullptr;)
	{
		TC_TEST_SIMPLE_LIST_NODE* ptTcNode = (TC_TEST_SIMPLE_LIST_NODE*)pNode;
		LOG_PROCESS_ERROR(ptTcNode);
		LPPRINTF("%d\n", ptTcNode->nIndex);

		pNode = pNode->pstNext;
	}
	LPPRINTF("\n");

	//tNode0 = (TC_TEST_SIMPLE_LIST_NODE*)oSimpleList.Pop();
	nResult = oSimpleList.Remove(tNode5);
	LOG_PROCESS_ERROR(nResult);

	for (SIMPLE_LIST_NODE* pNode = oSimpleList.Head(); pNode != nullptr;)
	{
		TC_TEST_SIMPLE_LIST_NODE* ptTcNode = (TC_TEST_SIMPLE_LIST_NODE*)pNode;
		LOG_PROCESS_ERROR(ptTcNode);
		LPPRINTF("%d\n", ptTcNode->nIndex);

		pNode = pNode->pstNext;
	}
	LPPRINTF("\n");

	//tNode0 = (TC_TEST_SIMPLE_LIST_NODE*)oSimpleList.Pop();
	nResult = oSimpleList.Remove(tNode3);
	LOG_PROCESS_ERROR(nResult);

	for (SIMPLE_LIST_NODE* pNode = oSimpleList.Head(); pNode != nullptr;)
	{
		TC_TEST_SIMPLE_LIST_NODE* ptTcNode = (TC_TEST_SIMPLE_LIST_NODE*)pNode;
		LOG_PROCESS_ERROR(ptTcNode);
		LPPRINTF("%d\n", ptTcNode->nIndex);

		pNode = pNode->pstNext;
	}
	LPPRINTF("\n");

	//tNode0 = (TC_TEST_SIMPLE_LIST_NODE*)oSimpleList.Pop();
	nResult = oSimpleList.Remove(tNode7);
	LOG_PROCESS_ERROR(nResult);

	for (SIMPLE_LIST_NODE* pNode = oSimpleList.Head(); pNode != nullptr;)
	{
		TC_TEST_SIMPLE_LIST_NODE* ptTcNode = (TC_TEST_SIMPLE_LIST_NODE*)pNode;
		LOG_PROCESS_ERROR(ptTcNode);
		LPPRINTF("%d\n", ptTcNode->nIndex);

		pNode = pNode->pstNext;
	}
	LPPRINTF("\n");

	//nResult = oSimpleList.Remove(tNode7);
	//LOG_PROCESS_ERROR(nResult);
	tNode0 = (TC_TEST_SIMPLE_LIST_NODE*)oSimpleList.Pop();

	oSimpleList.Clear();
	nResult = TRUE;

Exit0:

	SAFE_DELETE(tNode1);
	SAFE_DELETE(tNode2);
	SAFE_DELETE(tNode3);
	SAFE_DELETE(tNode4);
	SAFE_DELETE(tNode5);
	SAFE_DELETE(tNode6);
	SAFE_DELETE(tNode7);

	return nResult;
}

struct TC_TEST_STACK_LIST_NODE : STACK_LIST_NODE
{
	INT_32 nIndex;

	TC_TEST_STACK_LIST_NODE()
	{
		nIndex = 0;
	}
};

BOOL TC_TestStackList(void)
{
	INT_32 nResult = FALSE;
	LPStackList oStackList;

	TC_TEST_STACK_LIST_NODE* tNode0 = nullptr;
	TC_TEST_STACK_LIST_NODE* tNode1 = new TC_TEST_STACK_LIST_NODE();
	TC_TEST_STACK_LIST_NODE* tNode2 = new TC_TEST_STACK_LIST_NODE();
	TC_TEST_STACK_LIST_NODE* tNode3 = new TC_TEST_STACK_LIST_NODE();
	TC_TEST_STACK_LIST_NODE* tNode4 = new TC_TEST_STACK_LIST_NODE();
	TC_TEST_STACK_LIST_NODE* tNode5 = new TC_TEST_STACK_LIST_NODE();
	TC_TEST_STACK_LIST_NODE* tNode6 = new TC_TEST_STACK_LIST_NODE();
	TC_TEST_STACK_LIST_NODE* tNode7 = new TC_TEST_STACK_LIST_NODE();
	tNode1->nIndex = 1;
	tNode2->nIndex = 2;
	tNode3->nIndex = 3;
	tNode4->nIndex = 4;
	tNode5->nIndex = 5;
	tNode6->nIndex = 6;
	tNode7->nIndex = 7;

	nResult = oStackList.Push(tNode1);
	LOG_PROCESS_ERROR(nResult);
	nResult = oStackList.Push(tNode2);
	LOG_PROCESS_ERROR(nResult);
	nResult = oStackList.InsertBefore(tNode3, oStackList.Top());
	LOG_PROCESS_ERROR(nResult);
	nResult = oStackList.InsertAfter(tNode4, tNode2);
	LOG_PROCESS_ERROR(nResult);
	nResult = oStackList.InsertBefore(tNode5, tNode4);
	LOG_PROCESS_ERROR(nResult);
	nResult = oStackList.InsertBefore(tNode6, tNode3);
	LOG_PROCESS_ERROR(nResult);
	nResult = oStackList.InsertAfter(tNode7, tNode4);

	/*
	1
	2 1
	3 2 1
	3 2 4 1
	3 2 5 4 1
	6 3 2 5 4 1
	6 3 2 5 4 7 1
	*/

	for (STACK_LIST_NODE* pNode = oStackList.Top(); pNode != nullptr;)
	{
		TC_TEST_STACK_LIST_NODE* ptTcNode = (TC_TEST_STACK_LIST_NODE*)pNode;
		LOG_PROCESS_ERROR(ptTcNode);
		LPPRINTF("%d\n", ptTcNode->nIndex);

		pNode = pNode->pstNext;
	}
	LPPRINTF("\n");

	//tNode0 = (TC_TEST_STACK_LIST_NODE*)oStackList.Pop();
	nResult = oStackList.Remove(tNode4);
	LOG_PROCESS_ERROR(nResult);

	for (STACK_LIST_NODE* pNode = oStackList.Top(); pNode != nullptr;)
	{
		TC_TEST_STACK_LIST_NODE* ptTcNode = (TC_TEST_STACK_LIST_NODE*)pNode;
		LOG_PROCESS_ERROR(ptTcNode);
		LPPRINTF("%d\n", ptTcNode->nIndex);

		pNode = pNode->pstNext;
	}
	LPPRINTF("\n");

	//tNode0 = (TC_TEST_STACK_LIST_NODE*)oStackList.Pop();
	nResult = oStackList.Remove(tNode6);
	LOG_PROCESS_ERROR(nResult);

	for (STACK_LIST_NODE* pNode = oStackList.Top(); pNode != nullptr;)
	{
		TC_TEST_STACK_LIST_NODE* ptTcNode = (TC_TEST_STACK_LIST_NODE*)pNode;
		LOG_PROCESS_ERROR(ptTcNode);
		LPPRINTF("%d\n", ptTcNode->nIndex);

		pNode = pNode->pstNext;
	}
	LPPRINTF("\n");

	//tNode0 = (TC_TEST_STACK_LIST_NODE*)oStackList.Pop();
	nResult = oStackList.Remove(tNode1);
	LOG_PROCESS_ERROR(nResult);

	for (STACK_LIST_NODE* pNode = oStackList.Top(); pNode != nullptr;)
	{
		TC_TEST_STACK_LIST_NODE* ptTcNode = (TC_TEST_STACK_LIST_NODE*)pNode;
		LOG_PROCESS_ERROR(ptTcNode);
		LPPRINTF("%d\n", ptTcNode->nIndex);

		pNode = pNode->pstNext;
	}
	LPPRINTF("\n");

	//tNode0 = (TC_TEST_STACK_LIST_NODE*)oStackList.Pop();
	nResult = oStackList.Remove(tNode2);
	LOG_PROCESS_ERROR(nResult);

	for (STACK_LIST_NODE* pNode = oStackList.Top(); pNode != nullptr;)
	{
		TC_TEST_STACK_LIST_NODE* ptTcNode = (TC_TEST_STACK_LIST_NODE*)pNode;
		LOG_PROCESS_ERROR(ptTcNode);
		LPPRINTF("%d\n", ptTcNode->nIndex);

		pNode = pNode->pstNext;
	}
	LPPRINTF("\n");

	//tNode0 = (TC_TEST_STACK_LIST_NODE*)oStackList.Pop();
	nResult = oStackList.Remove(tNode5);
	LOG_PROCESS_ERROR(nResult);

	for (STACK_LIST_NODE* pNode = oStackList.Top(); pNode != nullptr;)
	{
		TC_TEST_STACK_LIST_NODE* ptTcNode = (TC_TEST_STACK_LIST_NODE*)pNode;
		LOG_PROCESS_ERROR(ptTcNode);
		LPPRINTF("%d\n", ptTcNode->nIndex);

		pNode = pNode->pstNext;
	}
	LPPRINTF("\n");

	//tNode0 = (TC_TEST_STACK_LIST_NODE*)oStackList.Pop();
	nResult = oStackList.Remove(tNode3);
	LOG_PROCESS_ERROR(nResult);

	for (STACK_LIST_NODE* pNode = oStackList.Top(); pNode != nullptr;)
	{
		TC_TEST_STACK_LIST_NODE* ptTcNode = (TC_TEST_STACK_LIST_NODE*)pNode;
		LOG_PROCESS_ERROR(ptTcNode);
		LPPRINTF("%d\n", ptTcNode->nIndex);

		pNode = pNode->pstNext;
	}
	LPPRINTF("\n");

	//tNode0 = (TC_TEST_STACK_LIST_NODE*)oStackList.Pop();
	nResult = oStackList.Remove(tNode7);
	LOG_PROCESS_ERROR(nResult);

	for (STACK_LIST_NODE* pNode = oStackList.Top(); pNode != nullptr;)
	{
		TC_TEST_STACK_LIST_NODE* ptTcNode = (TC_TEST_STACK_LIST_NODE*)pNode;
		LOG_PROCESS_ERROR(ptTcNode);
		LPPRINTF("%d\n", ptTcNode->nIndex);

		pNode = pNode->pstNext;
	}
	LPPRINTF("\n");

	//nResult = oStackList.Remove(tNode7);
	//LOG_PROCESS_ERROR(nResult);
	tNode0 = (TC_TEST_STACK_LIST_NODE*)oStackList.Pop();

	oStackList.Clear();
	nResult = TRUE;

Exit0:

	SAFE_DELETE(tNode1);
	SAFE_DELETE(tNode2);
	SAFE_DELETE(tNode3);
	SAFE_DELETE(tNode4);
	SAFE_DELETE(tNode5);
	SAFE_DELETE(tNode6);
	SAFE_DELETE(tNode7);

	return nResult;
}
