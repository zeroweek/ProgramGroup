#include "lp_looplist.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



LPLoopList::LPLoopList()
{
	m_dwSize = 0;
	m_dwPop  = 0;
	m_dwPush = 0;
	m_ppData = NULL;
	m_dwValidCount = 0;
}

LPLoopList::~LPLoopList()
{
	UnInit();
}

BOOL LPAPI LPLoopList::Init(UINT_32 dwSize)
{
	LOG_PROCESS_ERROR(dwSize > 0);

	m_ppData = new void*[dwSize];
	LOG_PROCESS_ERROR(m_ppData);

	m_dwSize = dwSize;
	m_dwPop  = 0;
	m_dwPush = 0;
	m_dwValidCount = 0;

	return TRUE;

Exit0:

	return FALSE;
}

BOOL LPAPI LPLoopList::UnInit(void)
{
	if (m_ppData)
	{
		SAFE_DELETE_SZ(m_ppData);
		m_ppData = NULL;
	}

	m_dwSize = 0;
	m_dwPop  = 0;
	m_dwPush = 0;
	m_dwValidCount = 0;

	return TRUE;
}






//end声明所处的名字空间

NS_LZPL_END
