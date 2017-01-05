#include "testcase.h"
#include "lp_processerror.h"
#include "lp_lzpl.h"
#include "lp_profile.h"


struct TC_TEST_SIMPLE_LIST_NODE : SIMPLE_LIST_NODE
{
	LPINT32 nIndex;

	TC_TEST_SIMPLE_LIST_NODE()
	{
		nIndex = 0;
	}
};

BOOL TC_TestSimpleList(void)
{
	LPINT32 nResult = FALSE;
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
	LPINT32 nIndex;

	TC_TEST_STACK_LIST_NODE()
	{
		nIndex = 0;
	}
};

BOOL TC_TestStackList(void)
{
	LPINT32 nResult = FALSE;
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
	BOOL PropertyCB##__index__(const LPIDENTID& oOwner, const LPUINT32& dwPropertyID, const ILPDataList& oldVar, const ILPDataList& newVar, const ILPDataList& oCBParams)\
	{\
		const LPINT64 nOldValue = oldVar.Int64(0);\
		const LPINT64 nNewValue = newVar.Int64(0);\
		INF("PropertyCB"#__index__"  \n-> Owner: %s, PropertyID: %d OldValue: " FMT_I64 " NewValue: " FMT_I64,\
			oOwner.ToString().c_str(), dwPropertyID, nOldValue, nNewValue);\
		INF("CBParams ->");\
		PRINTF_DATA_LIST(INF,oCBParams);\
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
	LPINT32 nResult = FALSE;
	LPIDENTID oOwner(1, 2);
	ILPProperty* poProperty = nullptr;

	LPDataList oCBParams0;
	LPDataList oCBParams1;
	oCBParams1 << 1;
	LPDataList oCBParams2;
	oCBParams2 << "2";

	LPNormalPropertyFactory oNormalPropertyFactory;
	poProperty = oNormalPropertyFactory.NewProperty(oOwner, 1, eDataType_Int64);
	LOG_PROCESS_ERROR(poProperty != nullptr);

	poProperty->RegisterCallback(PropertyCB2, 3, oCBParams0);
	poProperty->RegisterCallback(PropertyCB1, 1, oCBParams2);
	poProperty->RegisterCallback(PropertyCB2, 4, ILPDataList::NullDataList());
	poProperty->RegisterCallback(PropertyCB1, 2, oCBParams1);

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
	LPINT32 nResult = FALSE;
	ILPProperty** poPropertyArray = nullptr;
	LPNormalPropertyFactory oNormalPropertyFactory;
	
	LPProperty* pPropertyArray = new LPProperty[100];
	LPINT32 nSize1 = sizeof(pPropertyArray[0]);

	LPINT32 nSize4 = sizeof(LPBaseList);
	LPINT32 nSize5 = sizeof(LPSimpleList);
	LPINT32 nSize6 = sizeof(LPStackList);
	LPINT32 nSize7 = sizeof(BASE_LIST_NODE);
	nSize7 = sizeof(LPProperty);

	std::string strTest1 = "Test1";
	std::string strTest2 = "Test2";
	std::string& strTest3 = strTest1;
	strTest3 = "Test3";
	strTest3 = strTest2;

	std::vector<LPProperty> vecProperty;
	vecProperty.resize(100);
	LPINT32 nSize2 = sizeof(vecProperty);
	ILPProperty& oProperty = vecProperty[0];
	LPINT32 nSize3 = sizeof(vecProperty[0]);
	LPIDENTID oOwner(0, 0);
	nResult = oProperty.Init(oOwner, 1, eDataType_Int64);
	LOG_PROCESS_ERROR(nResult);
	oProperty.SetInt64(100);


	poPropertyArray = oNormalPropertyFactory.NewPropertyArray(100);
	LOG_PROCESS_ERROR(poPropertyArray != nullptr);

	WRN("PropertyInstanceCount = %d", oNormalPropertyFactory.GetPropertyInstanceCount());

	oNormalPropertyFactory.DeletePropertyArray(poPropertyArray, 100);

	WRN("PropertyInstanceCount = %d", oNormalPropertyFactory.GetPropertyInstanceCount());

	return TRUE;
Exit0:
	return FALSE;
}

struct DECLARE StringCmpKey
{
	bool operator() (const std::string& strKey1, const std::string& strKey2) const
	{
		return strcmp(strKey1.c_str(), strKey2.c_str()) < 0;
	}
};

BOOL TC_TestMapEfficiency(void)
{
	LPINT32 nResult = FALSE;
	std::map<LPUINT64, std::string> mapInt2String;
	std::map<std::string, std::string> mapString2String;
	std::map<std::string, std::string, StringCmpKey> mapCmpString2String;

	std::vector<std::string> vectString;
	vectString.resize(100000);
	for (LPUINT64 i = 0; i < vectString.size(); ++i)
	{
		vectString[i] = lpSerializeToString(MAX_INT64_LEN, "%d", i);
	}

	PROFILE_START();

	PROFILE_POINT(1);
	for (LPUINT64 i = 0; i < vectString.size(); ++i)
	{
		LOG_PROCESS_ERROR(mapString2String.insert(std::make_pair(vectString[i], vectString[i])).second);
	}
	PROFILE_POINT(1);

	PROFILE_POINT(2);
	for (LPUINT64 i = 0; i < vectString.size(); ++i)
	{
		LOG_PROCESS_ERROR(mapCmpString2String.insert(std::make_pair(vectString[i], vectString[i])).second);
	}
	PROFILE_POINT(2);

	PROFILE_POINT(3);
	for (LPUINT64 i = 0; i < vectString.size(); ++i)
	{
		LOG_PROCESS_ERROR(mapInt2String.insert(std::make_pair(i, vectString[i])).second);
	}
	PROFILE_POINT(3);

	PROFILE_POINT(4);
	for (LPUINT64 i = 0; i < vectString.size(); ++i)
	{
		LOG_PROCESS_ERROR(mapString2String.find(vectString[i]) != mapString2String.end());
	}
	PROFILE_POINT(4);

	PROFILE_POINT(5);
	for (LPUINT64 i = 0; i < vectString.size(); ++i)
	{
		LOG_PROCESS_ERROR(mapCmpString2String.find(vectString[i]) != mapCmpString2String.end());
	}
	PROFILE_POINT(5);

	PROFILE_POINT(6);
	for (LPUINT64 i = 0; i < vectString.size(); ++i)
	{
		LOG_PROCESS_ERROR(mapInt2String.find(i) != mapInt2String.end());
	}
	PROFILE_POINT(6);


	PROFILE_END();

	LOG_PROCESS_ERROR(TRUE);

	return TRUE;
Exit0:
	return FALSE;
}

BOOL TC_TestTable_Invalid()
{
	LPINT32 nResult = FALSE;

	nResult = sizeof(ILPTable::TABLE_MAP);
	nResult = sizeof(ILPTable::TABLE_INDEX);

	LPIDENTID oIdentID(0, 0);

	LPDataList oRecordType;
	oRecordType << eDataType_Int64 << eDataType_Float << eDataType_Double << eDataType_String;
	LPDataList oMapType;
	oMapType << eTableMapType_MapInt64 << eTableMapType_NotMap << eTableMapType_NotMap;

	LPDataList oRecord;
	oRecord << 1 << 2.2f << 3.3 << "4";
	LPDataList oRecord2;
	oRecord2 << 2 << 2.2f;

	LPNormalTableFactory oNormalTableFactory;
	ILPTable* poTable = oNormalTableFactory.NewTable(oIdentID, 1, oRecordType, ILPDataList::NullDataList());

	ILPTable::IteratorVect vectRecordIter;

	std::string strTest = "teststring";
	LPUINT32 dwColCount = poTable->GetColCount();
	LPUINT32 dwRecordCount = poTable->GetRecordCount();
	E_DataType eDataType = poTable->GetColType(0);
	eDataType = poTable->GetColType(1);
	eDataType = poTable->GetColType(2);
	eDataType = poTable->GetColType(3);

	LPINT64 lValue = 0;
	FLOAT fValue = ZERO_FLOAT;
	DOUBLE dValue = ZERO_DOUBLE;
	std::string strValue = NULL_STR;

	nResult = poTable->AddRecord(oRecord2);
	LOG_CHECK_ERROR(nResult);
	dwRecordCount = poTable->GetRecordCount();

	nResult = poTable->FindInt64(0, 1, vectRecordIter);
	nResult = poTable->FindFloat(1, 2.2f, vectRecordIter);
	nResult = poTable->FindDouble(2, 3.3, vectRecordIter);
	nResult = poTable->FindString(3, strTest, vectRecordIter);

	ILPTable::Iterator iter1;
	vectRecordIter.push_back(iter1);
	for (ILPTable::IteratorVect::iterator iter = vectRecordIter.begin(); iter != vectRecordIter.end(); ++iter)
	{
		nResult = poTable->SetInt64((*iter), 0, 11);
		nResult = poTable->SetInt64((*iter), 0, 2);
		nResult = poTable->SetFloat((*iter), 1, 22.22f);
		nResult = poTable->SetDouble((*iter), 2, 33.33);
		nResult = poTable->SetString((*iter), 3, strTest);

		ILPData& oValue = poTable->GetData((*iter), 0);
		lValue = poTable->GetInt64((*iter), 0);
		fValue = poTable->GetFloat((*iter), 1);
		dValue = poTable->GetDouble((*iter), 2);
		strValue = poTable->GetString((*iter), 3);

		nResult = poTable->Remove((*iter));
	}

	nResult = poTable->Remove(vectRecordIter);
	poTable->Clear();

	oNormalTableFactory.DeleteTable(poTable);

	LOG_PROCESS_ERROR(TRUE);

	return TRUE;
Exit0:
	return FALSE;
}

BOOL TC_TestTable_Col0_Map_Int64()
{
	LPINT32 nResult = FALSE;

	nResult = sizeof(ILPTable::TABLE_MAP);
	nResult = sizeof(ILPTable::TABLE_INDEX);

	LPIDENTID oIdentID(0, 0);
	LPDataList oRecordType;
	oRecordType << eDataType_Int64 << eDataType_Float << eDataType_Double << eDataType_String;
	LPDataList oMapType;
	oMapType << eTableMapType_MapInt64 << eTableMapType_NotMap << eTableMapType_NotMap << eTableMapType_NotMap;

	LPDataList oRecord1;
	oRecord1 << 1 << 1.1f << 1.1 << "1111";
	LPDataList oRecord2;
	oRecord2 << 2 << 2.2f << 2.2 << "2222";
	LPDataList oRecord3;
	oRecord3 << 3 << 3.3f << 3.3 << "3333";
	LPDataList oRecord4;
	oRecord4 << 4 << 4.4f << 4.4 << "4444";
	LPDataList oRecord5;
	oRecord5 << 5 << 5.5f << 5.5 << "5555";
	LPDataList oRecord6;
	oRecord6 << 6 << 6.6f << 6.6 << "6666";

	LPNormalTableFactory oNormalTableFactory;
	ILPTable* poTable = oNormalTableFactory.NewTable(oIdentID, 1, oRecordType, oMapType);

	ILPTable::IteratorVect vectRecordIter;

	std::string strTest = "teststring";
	LPUINT32 dwColCount = poTable->GetColCount();
	LPUINT32 dwRecordCount = poTable->GetRecordCount();
	E_DataType eDataType = poTable->GetColType(0);
	eDataType = poTable->GetColType(1);
	eDataType = poTable->GetColType(2);
	eDataType = poTable->GetColType(3);

	LPINT64 lValue = 0;
	FLOAT fValue = ZERO_FLOAT;
	DOUBLE dValue = ZERO_DOUBLE;
	std::string strValue = NULL_STR;

	nResult = poTable->AddRecord(oRecord1);
	LOG_CHECK_ERROR(nResult);
	dwRecordCount = poTable->GetRecordCount();
	nResult = poTable->AddRecord(oRecord2);
	LOG_CHECK_ERROR(nResult);
	dwRecordCount = poTable->GetRecordCount();
	nResult = poTable->AddRecord(oRecord3);
	LOG_CHECK_ERROR(nResult);
	dwRecordCount = poTable->GetRecordCount();
	nResult = poTable->AddRecord(oRecord4);
	LOG_CHECK_ERROR(nResult);
	dwRecordCount = poTable->GetRecordCount();
	nResult = poTable->AddRecord(oRecord5);
	LOG_CHECK_ERROR(nResult);
	dwRecordCount = poTable->GetRecordCount();
	nResult = poTable->AddRecord(oRecord6);
	LOG_CHECK_ERROR(nResult);
	dwRecordCount = poTable->GetRecordCount();

	nResult = poTable->FindInt64(0, 1, vectRecordIter);
	for (ILPTable::IteratorVect::iterator iter = vectRecordIter.begin(); iter != vectRecordIter.end(); ++iter)
	{
		nResult = poTable->SetInt64((*iter), 0, 123);
		nResult = poTable->SetString((*iter), 0, strTest);
		nResult = poTable->SetFloat((*iter), 1, 22.22f);
		nResult = poTable->SetString((*iter), 1, strTest);
		nResult = poTable->SetDouble((*iter), 2, 33.33);
		nResult = poTable->SetString((*iter), 2, strTest);
		nResult = poTable->SetString((*iter), 3, strTest);
		nResult = poTable->SetInt64((*iter), 3, 123);

		nResult = poTable->Remove((*iter));
	}

	nResult = poTable->FindFloat(1, 2.2f, vectRecordIter);
	for (ILPTable::IteratorVect::iterator iter = vectRecordIter.begin(); iter != vectRecordIter.end(); ++iter)
	{
		nResult = poTable->SetInt64((*iter), 0, 123);
		nResult = poTable->SetString((*iter), 0, strTest);
		nResult = poTable->SetFloat((*iter), 1, 22.22f);
		nResult = poTable->SetString((*iter), 1, strTest);
		nResult = poTable->SetDouble((*iter), 2, 33.33);
		nResult = poTable->SetString((*iter), 2, strTest);
		nResult = poTable->SetString((*iter), 3, strTest);
		nResult = poTable->SetInt64((*iter), 3, 123);

		nResult = poTable->Remove((*iter));
	}

	nResult = poTable->FindDouble(2, 3.3, vectRecordIter);
	for (ILPTable::IteratorVect::iterator iter = vectRecordIter.begin(); iter != vectRecordIter.end(); ++iter)
	{
		nResult = poTable->SetInt64((*iter), 0, 123);
		nResult = poTable->SetString((*iter), 0, strTest);
		nResult = poTable->SetFloat((*iter), 1, 22.22f);
		nResult = poTable->SetString((*iter), 1, strTest);
		nResult = poTable->SetDouble((*iter), 2, 33.33);
		nResult = poTable->SetString((*iter), 2, strTest);
		nResult = poTable->SetString((*iter), 3, strTest);
		nResult = poTable->SetInt64((*iter), 3, 123);

		nResult = poTable->Remove((*iter));
	}

	nResult = poTable->FindString(3, strTest, vectRecordIter);
	for (ILPTable::IteratorVect::iterator iter = vectRecordIter.begin(); iter != vectRecordIter.end(); ++iter)
	{
		nResult = poTable->SetInt64((*iter), 0, 123);
		nResult = poTable->SetString((*iter), 0, strTest);
		nResult = poTable->SetFloat((*iter), 1, 22.22f);
		nResult = poTable->SetString((*iter), 1, strTest);
		nResult = poTable->SetDouble((*iter), 2, 33.33);
		nResult = poTable->SetString((*iter), 2, strTest);
		nResult = poTable->SetString((*iter), 3, strTest);
		nResult = poTable->SetInt64((*iter), 3, 123);

		nResult = poTable->Remove((*iter));
	}

	nResult = poTable->Remove(vectRecordIter);
	poTable->Clear();

	oNormalTableFactory.DeleteTable(poTable);

	LOG_PROCESS_ERROR(TRUE);
	return TRUE;
Exit0:
	return FALSE;
}

BOOL TC_TestTable_Col0_Map_String()
{
	LPINT32 nResult = FALSE;

	nResult = sizeof(ILPTable::TABLE_MAP);
	nResult = sizeof(ILPTable::TABLE_INDEX);

	LPIDENTID oIdentID(0, 0);
	LPDataList oRecordType;
	oRecordType << eDataType_String << eDataType_Float << eDataType_Double << eDataType_String;
	LPDataList oMapType;
	oMapType << eTableMapType_MapString << eTableMapType_NotMap << eTableMapType_NotMap << eTableMapType_NotMap;

	LPDataList oRecord1;
	oRecord1 << "1" << 1.1f << 1.1 << "1111";
	LPDataList oRecord2;
	oRecord2 << "2" << 2.2f << 2.2 << "2222";
	LPDataList oRecord3;
	oRecord3 << "3" << 3.3f << 3.3 << "3333";
	LPDataList oRecord4;
	oRecord4 << "4" << 4.4f << 4.4 << "4444";
	LPDataList oRecord5;
	oRecord5 << "5" << 5.5f << 5.5 << "5555";
	LPDataList oRecord6;
	oRecord6 << "6" << 6.6f << 6.6 << "6666";

	LPNormalTableFactory oNormalTableFactory;
	ILPTable* poTable = oNormalTableFactory.NewTable(oIdentID, 1, oRecordType, oMapType);

	ILPTable::IteratorVect vectRecordIter;

	std::string strTest = "teststring";
	LPUINT32 dwColCount = poTable->GetColCount();
	LPUINT32 dwRecordCount = poTable->GetRecordCount();
	E_DataType eDataType = poTable->GetColType(0);
	eDataType = poTable->GetColType(1);
	eDataType = poTable->GetColType(2);
	eDataType = poTable->GetColType(3);

	LPINT64 lValue = 0;
	FLOAT fValue = ZERO_FLOAT;
	DOUBLE dValue = ZERO_DOUBLE;
	std::string strValue = NULL_STR;

	nResult = poTable->AddRecord(oRecord1);
	LOG_CHECK_ERROR(nResult);
	dwRecordCount = poTable->GetRecordCount();
	nResult = poTable->AddRecord(oRecord2);
	LOG_CHECK_ERROR(nResult);
	dwRecordCount = poTable->GetRecordCount();
	nResult = poTable->AddRecord(oRecord3);
	LOG_CHECK_ERROR(nResult);
	dwRecordCount = poTable->GetRecordCount();
	nResult = poTable->AddRecord(oRecord4);
	LOG_CHECK_ERROR(nResult);
	dwRecordCount = poTable->GetRecordCount();
	nResult = poTable->AddRecord(oRecord5);
	LOG_CHECK_ERROR(nResult);
	dwRecordCount = poTable->GetRecordCount();
	nResult = poTable->AddRecord(oRecord6);
	LOG_CHECK_ERROR(nResult);
	dwRecordCount = poTable->GetRecordCount();

	nResult = poTable->FindString(0, "1", vectRecordIter);
	for (ILPTable::IteratorVect::iterator iter = vectRecordIter.begin(); iter != vectRecordIter.end(); ++iter)
	{
		nResult = poTable->Remove((*iter));
	}

	nResult = poTable->FindString(3, "1111", vectRecordIter);
	for (ILPTable::IteratorVect::iterator iter = vectRecordIter.begin(); iter != vectRecordIter.end(); ++iter)
	{
		nResult = poTable->Remove((*iter));
	}

	nResult = poTable->FindString(3, "2222", vectRecordIter);
	for (ILPTable::IteratorVect::iterator iter = vectRecordIter.begin(); iter != vectRecordIter.end(); ++iter)
	{
		nResult = poTable->Remove((*iter));
	}

	nResult = poTable->Remove(vectRecordIter);
	poTable->Clear();

	oNormalTableFactory.DeleteTable(poTable);

	LOG_PROCESS_ERROR(TRUE);
	return TRUE;
Exit0:
	return FALSE;
}

BOOL TC_TestTable_Col4_Map_Int64()
{
	LPINT32 nResult = FALSE;

	nResult = sizeof(ILPTable::TABLE_MAP);
	nResult = sizeof(ILPTable::TABLE_INDEX);

	LPIDENTID oIdentID(0, 0);
	LPDataList oRecordType;
	oRecordType << eDataType_Int64 << eDataType_Float << eDataType_Double << eDataType_Int64;
	LPDataList oMapType;
	oMapType << eTableMapType_MapInt64 << eTableMapType_NotMap << eTableMapType_NotMap << eTableMapType_MapInt64;

	LPDataList oRecord1;
	oRecord1 << 1 << 1.1f << 1.1 << 1111;
	LPDataList oRecord2;
	oRecord2 << 2 << 2.2f << 2.2 << 2222;
	LPDataList oRecord3;
	oRecord3 << 3 << 3.3f << 3.3 << 3333;
	LPDataList oRecord4;
	oRecord4 << 4 << 4.4f << 4.4 << 4444;
	LPDataList oRecord5;
	oRecord5 << 5 << 5.5f << 5.5 << 5555;
	LPDataList oRecord6;
	oRecord6 << 6 << 6.6f << 6.6 << 6666;

	LPNormalTableFactory oNormalTableFactory;
	ILPTable* poTable = oNormalTableFactory.NewTable(oIdentID, 1, oRecordType, oMapType);

	ILPTable::IteratorVect vectRecordIter;
	LPUINT32 dwRecordCount = 0;

	LPINT64 lValue = 0;
	FLOAT fValue = ZERO_FLOAT;
	DOUBLE dValue = ZERO_DOUBLE;
	std::string strValue = NULL_STR;

	nResult = poTable->AddRecord(oRecord1);
	LOG_CHECK_ERROR(nResult);
	dwRecordCount = poTable->GetRecordCount();
	nResult = poTable->AddRecord(oRecord2);
	LOG_CHECK_ERROR(nResult);
	dwRecordCount = poTable->GetRecordCount();
	nResult = poTable->AddRecord(oRecord3);
	LOG_CHECK_ERROR(nResult);
	dwRecordCount = poTable->GetRecordCount();
	nResult = poTable->AddRecord(oRecord4);
	LOG_CHECK_ERROR(nResult);
	dwRecordCount = poTable->GetRecordCount();
	nResult = poTable->AddRecord(oRecord5);
	LOG_CHECK_ERROR(nResult);
	dwRecordCount = poTable->GetRecordCount();
	nResult = poTable->AddRecord(oRecord6);
	LOG_CHECK_ERROR(nResult);
	dwRecordCount = poTable->GetRecordCount();

	nResult = poTable->FindInt64(0, 1, vectRecordIter);
	for (ILPTable::IteratorVect::iterator iter = vectRecordIter.begin(); iter != vectRecordIter.end(); ++iter)
	{
		nResult = poTable->Remove((*iter));
	}

	nResult = poTable->FindInt64(3, 1111, vectRecordIter);
	for (ILPTable::IteratorVect::iterator iter = vectRecordIter.begin(); iter != vectRecordIter.end(); ++iter)
	{
		nResult = poTable->Remove((*iter));
	}

	nResult = poTable->FindInt64(3, 2222, vectRecordIter);
	for (ILPTable::IteratorVect::iterator iter = vectRecordIter.begin(); iter != vectRecordIter.end(); ++iter)
	{
		nResult = poTable->Remove((*iter));
	}

	nResult = poTable->Remove(vectRecordIter);
	poTable->Clear();

	oNormalTableFactory.DeleteTable(poTable);

	LOG_PROCESS_ERROR(TRUE);
	return TRUE;
Exit0:
	return FALSE;
}

BOOL TC_TestTable_Col4_Map_String()
{
	LPINT32 nResult = FALSE;

	nResult = sizeof(ILPTable::TABLE_MAP);
	nResult = sizeof(ILPTable::TABLE_INDEX);

	LPIDENTID oIdentID(0, 0);
	LPDataList oRecordType;
	oRecordType << eDataType_Int64 << eDataType_Float << eDataType_Double << eDataType_String;
	LPDataList oMapType;
	oMapType << eTableMapType_MapInt64 << eTableMapType_NotMap << eTableMapType_NotMap << eTableMapType_MapString;

	LPDataList oRecord1;
	oRecord1 << 1 << 1.1f << 1.1 << "1111";
	LPDataList oRecord2;
	oRecord2 << 2 << 2.2f << 2.2 << "2222";
	LPDataList oRecord3;
	oRecord3 << 3 << 3.3f << 3.3 << "3333";
	LPDataList oRecord4;
	oRecord4 << 4 << 4.4f << 4.4 << "4444";
	LPDataList oRecord5;
	oRecord5 << 5 << 5.5f << 5.5 << "5555";
	LPDataList oRecord6;
	oRecord6 << 6 << 6.6f << 6.6 << "6666";

	LPNormalTableFactory oNormalTableFactory;
	ILPTable* poTable = oNormalTableFactory.NewTable(oIdentID, 1, oRecordType, oMapType);

	ILPTable::IteratorVect vectRecordIter;

	std::string strTest = "teststring";
	LPUINT32 dwColCount = poTable->GetColCount();
	LPUINT32 dwRecordCount = poTable->GetRecordCount();
	E_DataType eDataType = poTable->GetColType(0);
	eDataType = poTable->GetColType(1);
	eDataType = poTable->GetColType(2);
	eDataType = poTable->GetColType(3);

	LPINT64 lValue = 0;
	FLOAT fValue = ZERO_FLOAT;
	DOUBLE dValue = ZERO_DOUBLE;
	std::string strValue = NULL_STR;

	nResult = poTable->AddRecord(oRecord1);
	LOG_CHECK_ERROR(nResult);
	dwRecordCount = poTable->GetRecordCount();
	nResult = poTable->AddRecord(oRecord2);
	LOG_CHECK_ERROR(nResult);
	dwRecordCount = poTable->GetRecordCount();
	nResult = poTable->AddRecord(oRecord3);
	LOG_CHECK_ERROR(nResult);
	dwRecordCount = poTable->GetRecordCount();
	nResult = poTable->AddRecord(oRecord4);
	LOG_CHECK_ERROR(nResult);
	dwRecordCount = poTable->GetRecordCount();
	nResult = poTable->AddRecord(oRecord5);
	LOG_CHECK_ERROR(nResult);
	dwRecordCount = poTable->GetRecordCount();
	nResult = poTable->AddRecord(oRecord6);
	LOG_CHECK_ERROR(nResult);
	dwRecordCount = poTable->GetRecordCount();

	nResult = poTable->FindInt64(0, 1, vectRecordIter);
	for (ILPTable::IteratorVect::iterator iter = vectRecordIter.begin(); iter != vectRecordIter.end(); ++iter)
	{
		nResult = poTable->Remove((*iter));
	}

	nResult = poTable->FindString(3, "1111", vectRecordIter);
	for (ILPTable::IteratorVect::iterator iter = vectRecordIter.begin(); iter != vectRecordIter.end(); ++iter)
	{
		nResult = poTable->Remove((*iter));
	}

	nResult = poTable->FindString(3, "2222", vectRecordIter);
	for (ILPTable::IteratorVect::iterator iter = vectRecordIter.begin(); iter != vectRecordIter.end(); ++iter)
	{
		nResult = poTable->Remove((*iter));
	}

	nResult = poTable->Remove(vectRecordIter);
	poTable->Clear();

	oNormalTableFactory.DeleteTable(poTable);

	LOG_PROCESS_ERROR(TRUE);
	return TRUE;
Exit0:
	return FALSE;
}

BOOL TC_TestTable_Col4_Mulmap_Int64()
{
	LPINT32 nResult = FALSE;

	nResult = sizeof(ILPTable::TABLE_MAP);
	nResult = sizeof(ILPTable::TABLE_INDEX);

	LPIDENTID oIdentID(0, 0);
	LPDataList oRecordType;
	oRecordType << eDataType_Int64 << eDataType_Float << eDataType_Double << eDataType_Int64;
	LPDataList oMapType;
	oMapType << eTableMapType_MapInt64 << eTableMapType_NotMap << eTableMapType_NotMap << eTableMapType_MulmapInt64;

	LPDataList oRecord1;
	oRecord1 << 1 << 1.1f << 1.1 << 1111;
	LPDataList oRecord11;
	oRecord11 << 11 << 1.1f << 1.1 << 1111;
	LPDataList oRecord2;
	oRecord2 << 2 << 2.2f << 2.2 << 2222;
	LPDataList oRecord22;
	oRecord22 << 22 << 2.2f << 2.2 << 2222;
	LPDataList oRecord3;
	oRecord3 << 3 << 3.3f << 3.3 << 3333;
	LPDataList oRecord33;
	oRecord33 << 33 << 3.3f << 3.3 << 3333;
	LPDataList oRecord4;
	oRecord4 << 4 << 4.4f << 4.4 << 4444;
	LPDataList oRecord5;
	oRecord5 << 5 << 5.5f << 5.5 << 5555;
	LPDataList oRecord6;
	oRecord6 << 6 << 6.6f << 6.6 << 6666;

	LPNormalTableFactory oNormalTableFactory;
	ILPTable* poTable = oNormalTableFactory.NewTable(oIdentID, 1, oRecordType, oMapType);

	ILPTable::IteratorVect vectRecordIter;

	std::string strTest = "teststring";
	LPUINT32 dwColCount = poTable->GetColCount();
	LPUINT32 dwRecordCount = poTable->GetRecordCount();
	E_DataType eDataType = poTable->GetColType(0);
	eDataType = poTable->GetColType(1);
	eDataType = poTable->GetColType(2);
	eDataType = poTable->GetColType(3);

	LPINT64 lValue = 0;
	FLOAT fValue = ZERO_FLOAT;
	DOUBLE dValue = ZERO_DOUBLE;
	std::string strValue = NULL_STR;

	nResult = poTable->AddRecord(oRecord1);
	LOG_CHECK_ERROR(nResult);
	dwRecordCount = poTable->GetRecordCount();
	nResult = poTable->AddRecord(oRecord11);
	LOG_CHECK_ERROR(nResult);
	dwRecordCount = poTable->GetRecordCount();
	nResult = poTable->AddRecord(oRecord2);
	LOG_CHECK_ERROR(nResult);
	dwRecordCount = poTable->GetRecordCount();
	nResult = poTable->AddRecord(oRecord22);
	LOG_CHECK_ERROR(nResult);
	dwRecordCount = poTable->GetRecordCount();
	nResult = poTable->AddRecord(oRecord3);
	LOG_CHECK_ERROR(nResult);
	dwRecordCount = poTable->GetRecordCount();
	nResult = poTable->AddRecord(oRecord33);
	LOG_CHECK_ERROR(nResult);
	dwRecordCount = poTable->GetRecordCount();
	nResult = poTable->AddRecord(oRecord4);
	LOG_CHECK_ERROR(nResult);
	dwRecordCount = poTable->GetRecordCount();
	nResult = poTable->AddRecord(oRecord5);
	LOG_CHECK_ERROR(nResult);
	dwRecordCount = poTable->GetRecordCount();
	nResult = poTable->AddRecord(oRecord6);
	LOG_CHECK_ERROR(nResult);
	dwRecordCount = poTable->GetRecordCount();

	nResult = poTable->FindInt64(0, 1, vectRecordIter);
	for (ILPTable::IteratorVect::iterator iter = vectRecordIter.begin(); iter != vectRecordIter.end(); ++iter)
	{
		nResult = poTable->Remove((*iter));
	}

	nResult = poTable->FindInt64(0, 1, vectRecordIter);
	for (ILPTable::IteratorVect::iterator iter = vectRecordIter.begin(); iter != vectRecordIter.end(); ++iter)
	{
		nResult = poTable->Remove((*iter));
	}

	nResult = poTable->FindInt64(3, 1111, vectRecordIter);
	for (ILPTable::IteratorVect::iterator iter = vectRecordIter.begin(); iter != vectRecordIter.end(); ++iter)
	{
		nResult = poTable->Remove((*iter));
	}

	nResult = poTable->FindInt64(3, 2222, vectRecordIter);
	for (ILPTable::IteratorVect::iterator iter = vectRecordIter.begin(); iter != vectRecordIter.end(); ++iter)
	{
		nResult = poTable->Remove((*iter));
	}

	nResult = poTable->FindInt64(3, 2222, vectRecordIter);
	for (ILPTable::IteratorVect::iterator iter = vectRecordIter.begin(); iter != vectRecordIter.end(); ++iter)
	{
		nResult = poTable->Remove((*iter));
	}

	nResult = poTable->Remove(vectRecordIter);
	poTable->Clear();

	oNormalTableFactory.DeleteTable(poTable);

	LOG_PROCESS_ERROR(TRUE);
	return TRUE;
Exit0:
	return FALSE;
}

BOOL TC_TestTable_Col4_Mulmap_String()
{
	LPINT32 nResult = FALSE;

	nResult = sizeof(ILPTable::TABLE_MAP);
	nResult = sizeof(ILPTable::TABLE_INDEX);

	LPIDENTID oIdentID(0, 0);
	LPDataList oRecordType;
	oRecordType << eDataType_Int64 << eDataType_Float << eDataType_Double << eDataType_String;
	LPDataList oMapType;
	oMapType << eTableMapType_MapInt64 << eTableMapType_NotMap << eTableMapType_NotMap << eTableMapType_MulmapString;

	LPDataList oRecord1;
	oRecord1 << 1 << 1.1f << 1.1 << "1111";
	LPDataList oRecord11;
	oRecord11 << 11 << 1.1f << 1.1 << "1111";
	LPDataList oRecord2;
	oRecord2 << 2 << 2.2f << 2.2 << "2222";
	LPDataList oRecord22;
	oRecord22 << 22 << 2.2f << 2.2 << "2222";
	LPDataList oRecord3;
	oRecord3 << 3 << 3.3f << 3.3 << "3333";
	LPDataList oRecord33;
	oRecord33 << 33 << 3.3f << 3.3 << "3333";
	LPDataList oRecord4;
	oRecord4 << 4 << 4.4f << 4.4 << "4444";
	LPDataList oRecord5;
	oRecord5 << 5 << 5.5f << 5.5 << "5555";
	LPDataList oRecord6;
	oRecord6 << 6 << 6.6f << 6.6 << "6666";

	LPNormalTableFactory oNormalTableFactory;
	ILPTable* poTable = oNormalTableFactory.NewTable(oIdentID, 1, oRecordType, oMapType);

	ILPTable::IteratorVect vectRecordIter;

	std::string strTest = "teststring";
	LPUINT32 dwColCount = poTable->GetColCount();
	LPUINT32 dwRecordCount = poTable->GetRecordCount();
	E_DataType eDataType = poTable->GetColType(0);
	eDataType = poTable->GetColType(1);
	eDataType = poTable->GetColType(2);
	eDataType = poTable->GetColType(3);

	LPINT64 lValue = 0;
	FLOAT fValue = ZERO_FLOAT;
	DOUBLE dValue = ZERO_DOUBLE;
	std::string strValue = NULL_STR;

	nResult = poTable->AddRecord(oRecord1);
	LOG_CHECK_ERROR(nResult);
	dwRecordCount = poTable->GetRecordCount();
	nResult = poTable->AddRecord(oRecord11);
	LOG_CHECK_ERROR(nResult);
	dwRecordCount = poTable->GetRecordCount();
	nResult = poTable->AddRecord(oRecord2);
	LOG_CHECK_ERROR(nResult);
	dwRecordCount = poTable->GetRecordCount();
	nResult = poTable->AddRecord(oRecord22);
	LOG_CHECK_ERROR(nResult);
	dwRecordCount = poTable->GetRecordCount();
	nResult = poTable->AddRecord(oRecord3);
	LOG_CHECK_ERROR(nResult);
	dwRecordCount = poTable->GetRecordCount();
	nResult = poTable->AddRecord(oRecord33);
	LOG_CHECK_ERROR(nResult);
	dwRecordCount = poTable->GetRecordCount();
	nResult = poTable->AddRecord(oRecord4);
	LOG_CHECK_ERROR(nResult);
	dwRecordCount = poTable->GetRecordCount();
	nResult = poTable->AddRecord(oRecord5);
	LOG_CHECK_ERROR(nResult);
	dwRecordCount = poTable->GetRecordCount();
	nResult = poTable->AddRecord(oRecord6);
	LOG_CHECK_ERROR(nResult);
	dwRecordCount = poTable->GetRecordCount();

	nResult = poTable->FindInt64(0, 1, vectRecordIter);
	for (ILPTable::IteratorVect::iterator iter = vectRecordIter.begin(); iter != vectRecordIter.end(); ++iter)
	{
		nResult = poTable->Remove((*iter));
	}

	nResult = poTable->FindInt64(0, 1, vectRecordIter);
	for (ILPTable::IteratorVect::iterator iter = vectRecordIter.begin(); iter != vectRecordIter.end(); ++iter)
	{
		nResult = poTable->Remove((*iter));
	}

	nResult = poTable->FindString(3, "1111", vectRecordIter);
	for (ILPTable::IteratorVect::iterator iter = vectRecordIter.begin(); iter != vectRecordIter.end(); ++iter)
	{
		nResult = poTable->Remove((*iter));
	}

	nResult = poTable->FindString(3, "2222", vectRecordIter);
	for (ILPTable::IteratorVect::iterator iter = vectRecordIter.begin(); iter != vectRecordIter.end(); ++iter)
	{
		nResult = poTable->Remove((*iter));
	}

	nResult = poTable->FindString(3, "2222", vectRecordIter);
	for (ILPTable::IteratorVect::iterator iter = vectRecordIter.begin(); iter != vectRecordIter.end(); ++iter)
	{
		nResult = poTable->Remove((*iter));
	}

	nResult = poTable->Remove(vectRecordIter);
	poTable->Clear();

	oNormalTableFactory.DeleteTable(poTable);

	LOG_PROCESS_ERROR(TRUE);
	return TRUE;
Exit0:
	return FALSE;
}

#include "lp_table.h"
BOOL TC_TestNewDeleteTable()
{
	LPINT32 nResult = FALSE;

	LPINT32 nSize = sizeof(LPTable);

	LOG_PROCESS_ERROR(TRUE);

	return TRUE;
Exit0:
	return FALSE;
}

#define TABLE_CB_DEF(__index__) \
	BOOL TableCB##__index__(const LPIDENTID& oOwner, const LPUINT32& dwTableID, const LPINT32& nOptType, const ILPTable::Iterator& iter,\
		const LPUINT32& dwCol, const ILPDataList& oOldVar, const ILPDataList& oNewVar, const ILPDataList& oCBParams)\
	{\
		switch (nOptType)\
		{\
			case eTableOptType_Add:\
				{\
					INF("TableCB"#__index__"  \n-> Owner(%s), TableID(%d) add record :", oOwner.ToString().c_str(), dwTableID);\
					INF("OldVal ->");\
					PRINTF_DATA_LIST(INF, oOldVar);\
					INF("NewVal ->");\
					PRINTF_DATA_LIST(INF, oNewVar);\
					INF("CBParamsVal ->");\
					PRINTF_DATA_LIST(INF, oCBParams);\
				}\
				break;\
			case eTableOptType_Del:\
				{\
					INF("TableCB"#__index__"  \n-> Owner(%s), TableID(%d) del record :", oOwner.ToString().c_str(), dwTableID);\
					INF("OldVal ->");\
					PRINTF_DATA_LIST(INF, oOldVar);\
					INF("NewVal ->");\
					PRINTF_DATA_LIST(INF, oNewVar);\
					INF("CBParamsVal ->");\
					PRINTF_DATA_LIST(INF, oCBParams);\
				}\
				break;\
			case eTableOptType_Update:\
				{\
					INF("TableCB"#__index__"  \n-> Owner(%s), TableID(%d) update col(%d) :", oOwner.ToString().c_str(), dwTableID, dwCol);\
					INF("OldVal ->");\
					PRINTF_DATA_LIST(INF, oOldVar);\
					INF("NewVal ->");\
					PRINTF_DATA_LIST(INF, oNewVar);\
					INF("CBParamsVal ->");\
					PRINTF_DATA_LIST(INF, oCBParams);\
				}\
				break;\
			case eTableOptType_Clear:\
				{\
					INF("TableCB"#__index__"  \n-> Owner(%s), TableID(%d) clear record :", oOwner.ToString().c_str(), dwTableID);\
					INF("OldVal ->");\
					PRINTF_DATA_LIST(INF, oOldVar);\
					INF("NewVal ->");\
					PRINTF_DATA_LIST(INF, oNewVar);\
					INF("CBParamsVal ->");\
					PRINTF_DATA_LIST(INF, oCBParams);\
				}\
				break;\
			case eTableOptType_Invalid:\
			case eTableOptType_Total:\
			default:\
				LOG_CHECK_ERROR(FALSE);\
				LPASSERT(FALSE);\
				break;\
		}\
		return TRUE;\
	}

TABLE_CB_DEF(1)
TABLE_CB_DEF(2)
TABLE_CB_DEF(3)
TABLE_CB_DEF(4)

BOOL TC_TestTableCB()
{
	LPINT32 nResult = FALSE;

	LPDataList oColType;
	oColType << eDataType_String << eDataType_String << eDataType_String << eDataType_Float << eDataType_Double << eDataType_Int64;

	LPDataList oColIndex;
	oColIndex << eTableMapType_MapString << eTableMapType_MapString << eTableMapType_NotMap << eTableMapType_NotMap << eTableMapType_NotMap << eTableMapType_NotMap;
	
	LPDataList oRecord1;
	oRecord1 << "1" << "2" << "3" << 4.0f << 5.0 << 6;

	LPDataList oRecord2;
	oRecord2 << "11" << "22" << "33" << 44.0f << 55.0 << 66;

	LPDataList oRecord3;
	oRecord3 << "111" << "222" << "333" << 444.0f << 555.0 << 666;

	LPDataList oRecord4;
	oRecord4 << "1111" << "2222" << "3333" << 4444.0f << 5555.0 << 6666;

	LPDataList oRecord5;
	oRecord5 << "11111" << "22222" << "33333" << 44444.0f << 55555.0 << 66666;

	LPDataList oCBParams0;
	LPDataList oCBParams1;
	oCBParams1 << 1;
	LPDataList oCBParams2;
	oCBParams2 << "2";

	ILPTable::IteratorVect vectRecordIter;
	LPNormalTableFactory oNormalTableFactory;
	ILPTable* poTable = oNormalTableFactory.NewTable(LPIDENTID(), 1, oColType, oColIndex);
	LOG_PROCESS_ERROR(poTable);
	
	nResult = poTable->RegisterCallback(TableCB2, 4, ILPDataList::NullDataList());
	LOG_PROCESS_ERROR(nResult);

	nResult = poTable->RegisterCallback(TableCB2, 2, oCBParams0);
	LOG_PROCESS_ERROR(nResult);

	nResult = poTable->RegisterCallback(TableCB1, 1, oCBParams1);
	LOG_PROCESS_ERROR(nResult);

	nResult = poTable->RegisterCallback(TableCB2, 3, oCBParams2);
	LOG_PROCESS_ERROR(nResult);

	nResult = poTable->AddRecord(oRecord1);
	LOG_PROCESS_ERROR(nResult);

	nResult = poTable->AddRecord(oRecord2);
	LOG_PROCESS_ERROR(nResult);

	nResult = poTable->AddRecord(oRecord3);
	LOG_PROCESS_ERROR(nResult);

	nResult = poTable->AddRecord(oRecord4);
	LOG_PROCESS_ERROR(nResult);

	nResult = poTable->AddRecord(oRecord5);
	LOG_PROCESS_ERROR(nResult);
	
	nResult = poTable->FindString(0, "11", vectRecordIter);
	LOG_PROCESS_ERROR(nResult);
	nResult = poTable->FindString(2, "11", vectRecordIter);
	LOG_PROCESS_ERROR(nResult);
	nResult = poTable->FindFloat(3, 44.0, vectRecordIter);
	LOG_PROCESS_ERROR(nResult);
	nResult = poTable->FindDouble(4, 55.0, vectRecordIter);
	LOG_PROCESS_ERROR(nResult);
	nResult = poTable->FindInt64(5, 66, vectRecordIter);
	LOG_PROCESS_ERROR(nResult);
	for (ILPTable::IteratorVect::iterator iter = vectRecordIter.begin(); iter != vectRecordIter.end(); ++iter)
	{
		nResult = poTable->SetString((*iter), 2, "hello");
		nResult = poTable->SetFloat((*iter), 3, 1.23f);
		nResult = poTable->SetDouble((*iter), 4, 4.56);
		nResult = poTable->SetInt64((*iter), 5, 888);
		LOG_PROCESS_ERROR(nResult);
	}

	nResult = poTable->Remove(vectRecordIter);
	LOG_PROCESS_ERROR(nResult);

	LOG_PROCESS_ERROR(TRUE);

	return TRUE;
Exit0:
	return FALSE;
}

BOOL TC_TestTable(void)
{
	LPINT32 nResult = FALSE;

	nResult = TC_TestTable_Col4_Mulmap_String();
	LOG_PROCESS_ERROR(nResult);

	return TRUE;
Exit0:
	return FALSE;
}
