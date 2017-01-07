//****************************************************************************/
//    author: caihy
//    date: December 2, 2015
//    description: 配置数据管理模板类定义
//
//****************************************************************************/
#ifndef _LP_DATA_MANAGER_H_
#define _LP_DATA_MANAGER_H_

#include "lp_base.h"
#include "lp_tabfile.h"
#include "lp_string.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



#define READ_DATA_START()					\
		LPINT32 nResult = 0;					\
		LPINT32 nValue = 0;

#define READ_INT_DATA(_field_, _var_)							\
		nResult = pTabFile->ReadData(_field_, nRow, nValue);	\
		LOG_PROCESS_ERROR(nResult && _field_);					\
		pData->_var_ = nValue;

#define FAST_READ_INT_DATA(_col_, _var_)						\
		nResult = pTabFile->ReadData(_col_, nRow, nValue);		\
		LOG_PROCESS_ERROR(nResult);								\
		pData->_var_ = nValue;

#define READ_STR_DATA(_field_, _var_)														\
		nResult = pTabFile->ReadData(_field_, nRow, pData->_var_, sizeof(pData->_var_));	\
		LOG_PROCESS_ERROR(nResult && _field_);

#define FAST_READ_STR_DATA(_col_, _var_)													\
		nResult = pTabFile->ReadData(_col_, nRow, pData->_var_, sizeof(pData->_var_));		\
		LOG_PROCESS_ERROR(nResult);

#define READ_DATA_END(_key_)		\
		rnId = pData->_key_;		\
		return TRUE;				\
	Exit0:							\
		rnId = pData->_key_;		\
		return FALSE;



// Summary:
//		每条加载的配置数据的结构
struct LOADED_DATA
{
	LPINT32         nRowIndex;
	LPINT32         nId;
	const char*    pcszName;
	void*          pData;

	LOADED_DATA() { pData = NULL; }
	~LOADED_DATA() { SAFE_DELETE(pData); }
};



// Summary:
//		配置数据的列表
struct LOADED_DATA_LIST
{
	BOOL                        bInit;
	LPINT32                     nRowCount;
	LPUINT64                    qwStartTick;
	char                        szConfigName[MAX_PATH];
	std::list<LOADED_DATA*>     lstData;

	LOADED_DATA_LIST() {}
	~LOADED_DATA_LIST()
	{
		LOADED_DATA* pLoadedData = NULL;
		std::list<LOADED_DATA*>::iterator it;
		for (it = lstData.begin(); it != lstData.end(); ++it)
		{
			pLoadedData = *it;
			SAFE_DELETE(pLoadedData);
		}
	}
};



// Summary:
//		配置数据管理模板类
template <class T>
class LPDataManger
{
public:

	typedef BOOL(*pfunPostProcessDataFunc)(T* pData, BOOL bInit);

public:

	// Summary:
	//		无
	LPDataManger();

	// Summary:
	//		无
	virtual ~LPDataManger();

	// Summary:
	//		无
	inline static LPDataManger& Instance(void);

	// Summary：
	//		无
	// Input:
	//		pFunc: 
	BOOL LPAPI Init(const char* pcszConfigName, const char* pcszPath, pfunPostProcessDataFunc pFunc);

	// Summary：
	//		无
	BOOL LPAPI UnInit(void);

	// Summary：
	//		无
	virtual BOOL LPAPI Reload(BOOL bInit);

	// Summary：
	//		post数据，执行指定的操作；程序启动第一次加载后执行
	virtual BOOL LPAPI PostProcessData(void);

	// Summary：
	//		无
	const T* LPAPI GetData(LPINT32 nDataId);

	// Summary：
	//		无
	T* LPAPI GetWritableData(LPINT32 nDataId);

	// Summary：
	//		无
	inline virtual LPINT32 LPAPI GetDataCount(void);

	// Summary：
	//		无
	inline virtual LPUINT32 LPAPI GetDataSize(void);

	// Summary：
	//		无
	inline virtual LPUINT32 LPAPI GetCRC(void);

	// Summary：
	//		遍历数据，执行相关操作
	template <class TFunc>
	inline BOOL LPAPI Traverse(TFunc& rFunc);

	// Summary：
	//		无
	const T* LPAPI GeFirstData(void);

	// Summary：
	//		第一次调用前，需要先调用GeFirstData
	const T* LPAPI GeNextData(void);

	// Summary：
	//		无
	void LPAPI Print(void);

private:

	// Summary：
	//		第一次加载时调用，插入新数据
	BOOL LPAPI _SetData(LPINT32 nDataId, T* pData);

	// Summary：
	//		加载配置文件数据
	virtual BOOL LPAPI _Load(LOADED_DATA_LIST* pLoadedDataList);

	// Summary：
	//		新加载数据处理，数据当前不存在则插入，存在则覆盖
	virtual BOOL LPAPI _Insert(LOADED_DATA_LIST* pLoadedDataList);

private:

	static LPDataManger<T>             m_oInstance;

	typedef std::map<LPINT32, T*> MAP_ID_2_DATA;
	MAP_ID_2_DATA                      m_mapId2Data;

	char                               m_szConfigName[MAX_PATH];
	char                               m_szPath[MAX_PATH];
	pfunPostProcessDataFunc            m_pfnPostProcessDataFunc;
	BOOL                               m_bNeedPostProcessData;
	LPUINT32                            m_dwCRC;

	typename MAP_ID_2_DATA::iterator   m_Id2DataIt;
};

template <class T>
LPDataManger<T> LPDataManger<T>::m_oInstance;

template<class T>
LPDataManger<T>::LPDataManger()
{
	m_szConfigName[0] = 0;
	m_szPath[0] = 0;
	m_pfnPostProcessDataFunc = NULL;
	m_bNeedPostProcessData = FALSE;
	m_dwCRC = 0;
}

template<class T>
LPDataManger<T>::~LPDataManger()
{
	UnInit();
}

template<class T>
inline LPDataManger<T> & LPDataManger<T>::Instance(void)
{
	return m_oInstance;
}

template<class T>
BOOL LPAPI LPDataManger<T>::Init(const char * pcszConfigName, const char * pcszPath, pfunPostProcessDataFunc pFunc)
{
	LPINT32 nResult = 0;

	LOG_PROCESS_ERROR(pcszConfigName);
	LOG_PROCESS_ERROR(pcszPath);

	lpStrCpyN(m_szConfigName, pcszConfigName, MAX_PATH);
	lpStrCpyN(m_szPath, pcszPath, MAX_PATH);
	m_pfnPostProcessDataFunc = pFunc;
	m_bNeedPostProcessData = FALSE;
	m_dwCRC = 0;

	nResult = Reload(TRUE);
	LOG_PROCESS_ERROR(nResult);

	return TRUE;
Exit0:
	return FALSE;
}

template<class T>
BOOL LPAPI LPDataManger<T>::UnInit(void)
{
	const T* pData = NULL;
	typename MAP_ID_2_DATA::iterator it;

	for (it = m_mapId2Data.begin(); it != m_mapId2Data.end(); ++it)
	{
		pData = it->second;
		SAFE_DELETE(pData);
	}
	
	m_mapId2Data.clear();

	return TRUE;
}

template<class T>
BOOL LPAPI LPDataManger<T>::Reload(BOOL bInit)
{
	LPINT32 nResult = 0;
	LOADED_DATA_LIST* pLoadedDataList = NULL;

	pLoadedDataList = new LOADED_DATA_LIST;
	LOG_PROCESS_ERROR(pLoadedDataList);

	pLoadedDataList->bInit = bInit;
	pLoadedDataList->nRowCount = 0;
	pLoadedDataList->qwStartTick = lpGetTickCountEx();
	lpStrCpyN(pLoadedDataList->szConfigName, m_szConfigName, MAX_PATH);

	nResult = _Load(pLoadedDataList);
	LOG_PROCESS_ERROR(nResult);

	nResult = _Insert(pLoadedDataList);
	LOG_PROCESS_ERROR(nResult);

	SAFE_DELETE(pLoadedDataList);

	return TRUE;

Exit0:

	SAFE_DELETE(pLoadedDataList);

	return FALSE;
}

template<class T>
BOOL LPAPI LPDataManger<T>::PostProcessData(void)
{
	LPINT32 nResult = 0;
	typename MAP_ID_2_DATA::iterator it;

	if(m_pfnPostProcessDataFunc && m_bNeedPostProcessData)
	{
		for (it = m_mapId2Data.begin(); it != m_mapId2Data.end(); ++it)
		{
			nResult = m_pfnPostProcessDataFunc(it->second, TRUE);
			if (!nResult)
			{
				WRN("failed to post process data id(%d) in %s.txt data file", it->first, m_szConfigName);
				continue;
			}
		}
	}

	m_bNeedPostProcessData = FALSE;

	return TRUE;
}

template<class T>
const T *LPAPI LPDataManger<T>::GetData(LPINT32 nDataId)
{
	typename MAP_ID_2_DATA::iterator fit;

	fit = m_mapId2Data.find(nDataId);
	if (fit != m_mapId2Data.end())
	{
		return fit->second;
	}

	return NULL;
}

template<class T>
T *LPAPI LPDataManger<T>::GetWritableData(LPINT32 nDataId)
{
	typename MAP_ID_2_DATA::iterator fit;

	fit = m_mapId2Data.find(nDataId);
	if (fit != m_mapId2Data.end())
	{
		return fit->second;
	}

	return NULL;
}

template<class T>
inline LPINT32 LPAPI LPDataManger<T>::GetDataCount(void)
{
	return (LPINT32)m_mapId2Data.size();
}

template<class T>
inline LPUINT32 LPAPI LPDataManger<T>::GetDataSize(void)
{
	return sizeof(T);
}

template<class T>
inline LPUINT32 LPAPI LPDataManger<T>::GetCRC(void)
{
	return m_dwCRC;
}

template<class T>
const T *LPAPI LPDataManger<T>::GeFirstData(void)
{
	m_Id2DataIt = m_mapId2Data.begin();
	if (m_Id2DataIt != m_mapId2Data.end())
	{
		return m_Id2DataIt->second;
	}
	else
	{
		return NULL;
	}
}

template<class T>
const T *LPAPI LPDataManger<T>::GeNextData(void)
{
	const T* pResult = NULL;

	if (m_Id2DataIt != m_mapId2Data.end())
	{
		++m_Id2DataIt;
		if (m_Id2DataIt != m_mapId2Data.end())
		{
			return m_Id2DataIt->second;
		}
	}

	return pResult;
}

template<class T>
void LPAPI LPDataManger<T>::Print(void)
{
	typename MAP_ID_2_DATA::iterator it;

	LPPRINTF("\n\nprint data file %s.txt :\n\n", m_szConfigName);
	for (it = m_mapId2Data.begin(); it != m_mapId2Data.end(); ++it)
	{
		T::Print(it->second);
	}
	LPPRINTF("\n");
}

template<class T>
BOOL LPAPI LPDataManger<T>::_SetData(LPINT32 nDataId, T * pData)
{
	typename MAP_ID_2_DATA::iterator fitId;

	PROCESS_ERROR(pData);

	fitId = m_mapId2Data.find(nDataId);
	PROCESS_ERROR(fitId == m_mapId2Data.end());

	m_mapId2Data[nDataId] = pData;

	return TRUE;
Exit0:
	return FALSE;
}

template<class T>
BOOL LPAPI LPDataManger<T>::_Load(LOADED_DATA_LIST* pLoadedDataList)
{
	LPINT32 nResult = 0;
	LPINT32 nRowIndex = 0;
	LPTabFile oTabFile;
	LOADED_DATA* pLoadedData = NULL;
	char szFileName[MAX_PATH];

	LOG_PROCESS_ERROR(pLoadedDataList);

	sprintf_s(szFileName, MAX_PATH, "%s/%s.txt", m_szPath, m_szConfigName);
	nResult = oTabFile.Init(szFileName, 1, TRUE);
	LOG_PROCESS_ERROR(nResult);

	for (nRowIndex = 0; nRowIndex < oTabFile.GetRowCount(); ++nRowIndex)
	{
		pLoadedData = new LOADED_DATA;
		LOG_PROCESS_ERROR(pLoadedData);

		pLoadedData->nRowIndex = nRowIndex;
		pLoadedData->nId = -1;
		pLoadedData->pData = new T;
		LOG_PROCESS_ERROR(pLoadedData->pData);
		memset(pLoadedData->pData, 0, sizeof(T));

		nResult = T::ReadLine(pLoadedData->nId, (T*)pLoadedData->pData, &oTabFile, nRowIndex);
		if (!nResult)
		{
			WRN("failed to read data at row(%d) id(%d) in %s.txt data file", nRowIndex + 2, pLoadedData->nId, m_szConfigName);
			SAFE_DELETE(pLoadedData->pData);
			continue;
		}

		pLoadedDataList->lstData.push_back(pLoadedData);
		pLoadedData = NULL;
	}

	pLoadedDataList->nRowCount = oTabFile.GetRowCount();
	m_dwCRC = oTabFile.GetCRC();
	
	nResult = oTabFile.UnInit();
	LOG_PROCESS_ERROR(nResult);

	return TRUE;

Exit0:

	FTL("failed to read data in %s.txt", m_szConfigName);

	SAFE_DELETE(pLoadedData);
	
	nResult = oTabFile.UnInit();
	LOG_CHECK_ERROR(nResult);

	UnInit();

	return FALSE;
}

template<class T>
BOOL LPAPI LPDataManger<T>::_Insert(LOADED_DATA_LIST* pLoadedDataList)
{
	LPINT32 nResult = 0;
	LPINT32 nCounter = 0;
	LPUINT64 qwStartTick = 0;
	LPUINT64 qwCurrentTick = 0;
	LOADED_DATA* pLoadedData = NULL;
	typename MAP_ID_2_DATA::iterator itData;

	LOG_PROCESS_ERROR(pLoadedDataList);

	qwStartTick = lpGetTickCountEx();

	for (std::list<LOADED_DATA*>::iterator it = pLoadedDataList->lstData.begin();
	it != pLoadedDataList->lstData.end(); ++it)
	{
		pLoadedData = *it;
		if (!pLoadedDataList->bInit)
		{
			itData = m_mapId2Data.find(pLoadedData->nId);
		}

		if (!pLoadedDataList->bInit && itData != m_mapId2Data.end())
		{
			if (m_pfnPostProcessDataFunc)
			{
				nResult = m_pfnPostProcessDataFunc(itData->second, FALSE);
				if (!nResult)
				{
					WRN("failed to post process original data at row(%d) id(%d) in %s.txt data file",
						pLoadedData->nRowIndex + 2, pLoadedData->nId, m_szConfigName);
					continue;
				}
			}

			memcpy(itData->second, pLoadedData->pData, sizeof(T));
		}
		else
		{
			nResult = _SetData(pLoadedData->nId, (T*)pLoadedData->pData);
			if (!nResult)
			{
				WRN("duplicated id(%d) at row(%d) in %s.txt data file",
					pLoadedData->nId, pLoadedData->nRowIndex + 2, m_szConfigName);
				continue;
			}

			pLoadedData->pData = NULL;
		}

		nCounter++;
	}

	qwCurrentTick = lpGetTickCountEx();
	INF("read %d / %d data in %s.txt successfully (cost %d ms %d ms %.2f MB)", nCounter, pLoadedDataList->nRowCount, m_szConfigName,
		(LPINT32)(qwCurrentTick - pLoadedDataList->qwStartTick), (LPINT32)(qwCurrentTick - qwStartTick), (sizeof(T) + sizeof(void*) * 4) * nCounter / (1024 * 1024.0f));

#ifdef _DEBUG
	if (nCounter < pLoadedDataList->nRowCount)
	{
		WRN("data file %s.txt have %d data error", m_szConfigName, pLoadedDataList->nRowCount - nCounter);
	}
#endif

	if (pLoadedDataList->bInit)
	{
		m_bNeedPostProcessData = TRUE;
	}

	return TRUE;

Exit0:

	FTL("failed to insert data in %s.txt data file", m_szConfigName);

	return FALSE;
}

template<class T>
template<class TFunc>
BOOL LPAPI LPDataManger<T>::Traverse(TFunc & rFunc)
{
	LPINT32 nResult = 0;
	typename MAP_ID_2_DATA::iterator it;

	for (it = m_mapId2Data.begin(); it != m_mapId2Data.end(); ++it)
	{
		nResult = rFunc(it->second);
		PROCESS_ERROR(nResult);
	}

	return TRUE;
Exit0:
	return FALSE;
}





//end声明所处的名字空间
NS_LZPL_END

#endif
