#include "testcase.h"
#include "lp_processerror.h"
#include "lp_lzpl.h"


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

	SIMPLE_LIST_FOR_BEGIN(oSimpleList)
	{
		TC_TEST_SIMPLE_LIST_NODE* ptTcNode = (TC_TEST_SIMPLE_LIST_NODE*)ptNode;
		LOG_PROCESS_ERROR(ptTcNode);
		LPPRINTF("%d\n", ptTcNode->nIndex);
	}
	SIMPLE_LIST_FOR_END
	LPPRINTF("\n");

	//tNode0 = (TC_TEST_SIMPLE_LIST_NODE*)oSimpleList.Pop();
	nResult = oSimpleList.Remove(tNode4);
	LOG_PROCESS_ERROR(nResult);

	SIMPLE_LIST_FOR_BEGIN(oSimpleList)
	{
		TC_TEST_SIMPLE_LIST_NODE* ptTcNode = (TC_TEST_SIMPLE_LIST_NODE*)ptNode;
		LOG_PROCESS_ERROR(ptTcNode);
		LPPRINTF("%d\n", ptTcNode->nIndex);
	}
	SIMPLE_LIST_FOR_END
	LPPRINTF("\n");

	//tNode0 = (TC_TEST_SIMPLE_LIST_NODE*)oSimpleList.Pop();
	nResult = oSimpleList.Remove(tNode6);
	LOG_PROCESS_ERROR(nResult);

	SIMPLE_LIST_FOR_BEGIN(oSimpleList)
	{
		TC_TEST_SIMPLE_LIST_NODE* ptTcNode = (TC_TEST_SIMPLE_LIST_NODE*)ptNode;
		LOG_PROCESS_ERROR(ptTcNode);
		LPPRINTF("%d\n", ptTcNode->nIndex);
	}
	SIMPLE_LIST_FOR_END
	LPPRINTF("\n");

	//tNode0 = (TC_TEST_SIMPLE_LIST_NODE*)oSimpleList.Pop();
	nResult = oSimpleList.Remove(tNode1);
	LOG_PROCESS_ERROR(nResult);

	SIMPLE_LIST_FOR_BEGIN(oSimpleList)
	{
		TC_TEST_SIMPLE_LIST_NODE* ptTcNode = (TC_TEST_SIMPLE_LIST_NODE*)ptNode;
		LOG_PROCESS_ERROR(ptTcNode);
		LPPRINTF("%d\n", ptTcNode->nIndex);
	}
	SIMPLE_LIST_FOR_END
	LPPRINTF("\n");

	//tNode0 = (TC_TEST_SIMPLE_LIST_NODE*)oSimpleList.Pop();
	nResult = oSimpleList.Remove(tNode2);
	LOG_PROCESS_ERROR(nResult);

	SIMPLE_LIST_FOR_BEGIN(oSimpleList)
	{
		TC_TEST_SIMPLE_LIST_NODE* ptTcNode = (TC_TEST_SIMPLE_LIST_NODE*)ptNode;
		LOG_PROCESS_ERROR(ptTcNode);
		LPPRINTF("%d\n", ptTcNode->nIndex);
	}
	SIMPLE_LIST_FOR_END
	LPPRINTF("\n");

	//tNode0 = (TC_TEST_SIMPLE_LIST_NODE*)oSimpleList.Pop();
	nResult = oSimpleList.Remove(tNode5);
	LOG_PROCESS_ERROR(nResult);

	SIMPLE_LIST_FOR_BEGIN(oSimpleList)
	{
		TC_TEST_SIMPLE_LIST_NODE* ptTcNode = (TC_TEST_SIMPLE_LIST_NODE*)ptNode;
		LOG_PROCESS_ERROR(ptTcNode);
		LPPRINTF("%d\n", ptTcNode->nIndex);
	}
	SIMPLE_LIST_FOR_END
	LPPRINTF("\n");

	//tNode0 = (TC_TEST_SIMPLE_LIST_NODE*)oSimpleList.Pop();
	nResult = oSimpleList.Remove(tNode3);
	LOG_PROCESS_ERROR(nResult);

	SIMPLE_LIST_FOR_BEGIN(oSimpleList)
	{
		TC_TEST_SIMPLE_LIST_NODE* ptTcNode = (TC_TEST_SIMPLE_LIST_NODE*)ptNode;
		LOG_PROCESS_ERROR(ptTcNode);
		LPPRINTF("%d\n", ptTcNode->nIndex);
	}
	SIMPLE_LIST_FOR_END
	LPPRINTF("\n");

	//tNode0 = (TC_TEST_SIMPLE_LIST_NODE*)oSimpleList.Pop();
	nResult = oSimpleList.Remove(tNode7);
	LOG_PROCESS_ERROR(nResult);

	SIMPLE_LIST_FOR_BEGIN(oSimpleList)
	{
		TC_TEST_SIMPLE_LIST_NODE* ptTcNode = (TC_TEST_SIMPLE_LIST_NODE*)ptNode;
		LOG_PROCESS_ERROR(ptTcNode);
		LPPRINTF("%d\n", ptTcNode->nIndex);
	}
	SIMPLE_LIST_FOR_END
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

	STACK_LIST_FOR_BEGIN(oStackList)
	{
		TC_TEST_STACK_LIST_NODE* ptTcNode = (TC_TEST_STACK_LIST_NODE*)ptNode;
		LOG_PROCESS_ERROR(ptTcNode);
		LPPRINTF("%d\n", ptTcNode->nIndex);
	}
	STACK_LIST_FOR_END
	LPPRINTF("\n");

	//tNode0 = (TC_TEST_STACK_LIST_NODE*)oStackList.Pop();
	nResult = oStackList.Remove(tNode4);
	LOG_PROCESS_ERROR(nResult);

	STACK_LIST_FOR_BEGIN(oStackList)
	{
		TC_TEST_STACK_LIST_NODE* ptTcNode = (TC_TEST_STACK_LIST_NODE*)ptNode;
		LOG_PROCESS_ERROR(ptTcNode);
		LPPRINTF("%d\n", ptTcNode->nIndex);
	}
	STACK_LIST_FOR_END
	LPPRINTF("\n");

	//tNode0 = (TC_TEST_STACK_LIST_NODE*)oStackList.Pop();
	nResult = oStackList.Remove(tNode6);
	LOG_PROCESS_ERROR(nResult);

	STACK_LIST_FOR_BEGIN(oStackList)
	{
		TC_TEST_STACK_LIST_NODE* ptTcNode = (TC_TEST_STACK_LIST_NODE*)ptNode;
		LOG_PROCESS_ERROR(ptTcNode);
		LPPRINTF("%d\n", ptTcNode->nIndex);
	}
	STACK_LIST_FOR_END
	LPPRINTF("\n");

	//tNode0 = (TC_TEST_STACK_LIST_NODE*)oStackList.Pop();
	nResult = oStackList.Remove(tNode1);
	LOG_PROCESS_ERROR(nResult);

	STACK_LIST_FOR_BEGIN(oStackList)
	{
		TC_TEST_STACK_LIST_NODE* ptTcNode = (TC_TEST_STACK_LIST_NODE*)ptNode;
		LOG_PROCESS_ERROR(ptTcNode);
		LPPRINTF("%d\n", ptTcNode->nIndex);
	}
	STACK_LIST_FOR_END
	LPPRINTF("\n");

	//tNode0 = (TC_TEST_STACK_LIST_NODE*)oStackList.Pop();
	nResult = oStackList.Remove(tNode2);
	LOG_PROCESS_ERROR(nResult);

	STACK_LIST_FOR_BEGIN(oStackList)
	{
		TC_TEST_STACK_LIST_NODE* ptTcNode = (TC_TEST_STACK_LIST_NODE*)ptNode;
		LOG_PROCESS_ERROR(ptTcNode);
		LPPRINTF("%d\n", ptTcNode->nIndex);
	}
	STACK_LIST_FOR_END
	LPPRINTF("\n");

	//tNode0 = (TC_TEST_STACK_LIST_NODE*)oStackList.Pop();
	nResult = oStackList.Remove(tNode5);
	LOG_PROCESS_ERROR(nResult);

	STACK_LIST_FOR_BEGIN(oStackList)
	{
		TC_TEST_STACK_LIST_NODE* ptTcNode = (TC_TEST_STACK_LIST_NODE*)ptNode;
		LOG_PROCESS_ERROR(ptTcNode);
		LPPRINTF("%d\n", ptTcNode->nIndex);
	}
	STACK_LIST_FOR_END
	LPPRINTF("\n");

	//tNode0 = (TC_TEST_STACK_LIST_NODE*)oStackList.Pop();
	nResult = oStackList.Remove(tNode3);
	LOG_PROCESS_ERROR(nResult);

	STACK_LIST_FOR_BEGIN(oStackList)
	{
		TC_TEST_STACK_LIST_NODE* ptTcNode = (TC_TEST_STACK_LIST_NODE*)ptNode;
		LOG_PROCESS_ERROR(ptTcNode);
		LPPRINTF("%d\n", ptTcNode->nIndex);
	}
	STACK_LIST_FOR_END
	LPPRINTF("\n");

	//tNode0 = (TC_TEST_STACK_LIST_NODE*)oStackList.Pop();
	nResult = oStackList.Remove(tNode7);
	LOG_PROCESS_ERROR(nResult);

	STACK_LIST_FOR_BEGIN(oStackList)
	{
		TC_TEST_STACK_LIST_NODE* ptTcNode = (TC_TEST_STACK_LIST_NODE*)ptNode;
		LOG_PROCESS_ERROR(ptTcNode);
		LPPRINTF("%d\n", ptTcNode->nIndex);
	}
	STACK_LIST_FOR_END
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

#define PROPERTY_CB_DEF(__index__) \
	BOOL PropertyCB##__index__(const LPIDENTID& oOwner, const UINT_32& dwPropertyID, const ILPDataList& oldVar, const ILPDataList& newVar, const ILPDataList& nullVar)\
	{\
		const INT_64 nOldValue = oldVar.Int64(0);\
		const INT_64 nNewValue = newVar.Int64(0);\
		INF("PropertyCB"#__index__"  \n-> Owner: %s, PropertyID: %d OldValue: " FMT_I64 " NewValue: " FMT_I64,\
			oOwner.ToString().c_str(), dwPropertyID, nOldValue, nNewValue);\
		return TRUE;\
	}

PROPERTY_CB_DEF(1)
PROPERTY_CB_DEF(2)
PROPERTY_CB_DEF(3)
PROPERTY_CB_DEF(4)
PROPERTY_CB_DEF(5)
PROPERTY_CB_DEF(6)
PROPERTY_CB_DEF(7)

BOOL TC_TestPropertyCallBack(void)
{
	INT_32 nResult = FALSE;
	LPIDENTID oOwner(1, 2);
	ILPProperty* poProperty = nullptr;

	LPNormalPropertyFactory oNormalPropertyFactory;
	poProperty = oNormalPropertyFactory.NewProperty(oOwner, 1, eDataType_Int64);
	LOG_PROCESS_ERROR(poProperty != nullptr);

	poProperty->RegisterCallback(PropertyCB1, 1, ILPDataList::NullDataList());
	poProperty->RegisterCallback(PropertyCB2, 3, ILPDataList::NullDataList());
	poProperty->RegisterCallback(PropertyCB3, 5, ILPDataList::NullDataList());
	poProperty->RegisterCallback(PropertyCB4, 2, ILPDataList::NullDataList());
	poProperty->RegisterCallback(PropertyCB5, 2, ILPDataList::NullDataList());
	poProperty->RegisterCallback(PropertyCB6, 3, ILPDataList::NullDataList());
	poProperty->RegisterCallback(PropertyCB7, 4, ILPDataList::NullDataList());

	poProperty->SetInt64(0);
	poProperty->SetInt64(1);

	LOG_PROCESS_ERROR(TRUE);

	return TRUE;
Exit0:
	return FALSE;
}

#include "lp_property.h"
BOOL TC_TestNewDeleteProperty(void)
{
	INT_32 nResult = FALSE;
	ILPProperty* poPropertyArray = nullptr;
	LPNormalPropertyFactory oNormalPropertyFactory;
	
	LPProperty* pPropertyArray = new LPProperty[100];
	INT_32 nSize1 = sizeof(pPropertyArray[0]);

	INT_32 nSize4 = sizeof(LPBaseList);
	INT_32 nSize5 = sizeof(LPSimpleList);
	INT_32 nSize6 = sizeof(LPStackList);
	INT_32 nSize7 = sizeof(BASE_LIST_NODE);

	std::string strTest1 = "Test1";
	std::string strTest2 = "Test2";
	std::string& strTest3 = strTest1;
	strTest3 = "Test3";
	strTest3 = strTest2;

	std::vector<LPProperty> vecProperty;
	vecProperty.resize(100);
	INT_32 nSize2 = sizeof(vecProperty);
	ILPProperty& oProperty = vecProperty[0];
	INT_32 nSize3 = sizeof(vecProperty[0]);
	LPIDENTID oOwner(0, 0);
	nResult = oProperty.Init(oOwner, 1, eDataType_Int64);
	LOG_PROCESS_ERROR(nResult);
	oProperty.SetInt64(100);


	poPropertyArray = oNormalPropertyFactory.NewPropertyArray(100);
	LOG_PROCESS_ERROR(poPropertyArray != nullptr);

	WRN("PropertyInstanceCount = %d", oNormalPropertyFactory.GetPropertyInstanceCount());

	oNormalPropertyFactory.DeletePropertyArray(poPropertyArray);

	WRN("PropertyInstanceCount = %d", oNormalPropertyFactory.GetPropertyInstanceCount());

	return TRUE;
Exit0:
	return FALSE;
}
