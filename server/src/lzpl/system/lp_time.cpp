#include "lp_time.h"
#include "lp_processerror.h"
#include "lp_system.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



LPINT32 LPTime::ms_nTimezone = 0;
LPUINT64 LPTime::ms_qwTimezoneSec = 0;

LPTime::LPTime()
{
	m_Sec = 0;
	m_Min = 0;
	m_Hour = 0;
	m_Mday = 0;
	m_Mon = 0;
	m_Year = 0;
	m_Wday = 0;
	m_Yday = 0;
	m_Msec = 0;

	memset(&m_stTimeval, 0, sizeof(m_stTimeval));
}

LZPL::LPTime::LPTime(LPUINT64 qwTimeStamp)
{
	LPTime stTime;
	time_t tTime = qwTimeStamp;
	struct tm stTm;
	struct tm* pTm = nullptr;

#	ifdef _WIN32
	{
		LPINT32 nResult = gmtime_s(&stTm, &tTime);
		PROCESS_ERROR(nResult == 0);
	}
#	else
	{
		pTm = gmtime_r(&tTime, &stTm);
		PROCESS_ERROR(pTm != nullptr);
	}
#	endif

	m_Sec = stTm.tm_sec;
	m_Min = stTm.tm_min;
	m_Hour = stTm.tm_hour;
	m_Mday = stTm.tm_mday;
	m_Mon = stTm.tm_mon + 1;
	m_Year = stTm.tm_year + 1900;
	m_Wday = stTm.tm_wday;
	m_Yday = stTm.tm_yday;
	m_Msec = 0;

	m_stTimeval.m_qwSec = qwTimeStamp;
	m_stTimeval.m_qwUsec = 0;

Exit0:
	return ;
}

LPTime::~LPTime()
{

}

BOOL LPAPI LPTime::SecEqualTo(const LPTime& t)
{
	return (m_stTimeval.m_qwSec == t.m_stTimeval.m_qwSec);
}

BOOL LPAPI LPTime::UsecEqualTo(const LPTime& t)
{
	return (m_stTimeval.m_qwSec == t.m_stTimeval.m_qwSec
		&& m_stTimeval.m_qwUsec == t.m_stTimeval.m_qwUsec);
}

BOOL LPAPI LPTime::GlobalInit(LPINT32 nTimezone)
{
	PROCESS_ERROR(TIME_ZONE_MIN <= nTimezone && nTimezone <= TIME_ZONE_MAX);
	ms_nTimezone = nTimezone;
	ms_qwTimezoneSec = ms_nTimezone * ONE_HOUR_SEC;

	return TRUE;

Exit0:
	return FALSE;
}

LPTime LPAPI LPTime::GetNowTime()
{
	LPTime stTime;
	time_t tTime = 0;
	struct tm stTm;
	struct tm* pTm = nullptr;

	lpGetTimeEx(stTime.m_stTimeval);
	stTime.m_stTimeval.m_qwSec += ms_qwTimezoneSec;

	tTime = stTime.m_stTimeval.m_qwSec;
#	ifdef _WIN32
	{
		LPINT32 nResult = gmtime_s(&stTm, &tTime);
		PROCESS_ERROR(nResult == 0);
	}
#	else
	{
		pTm = gmtime_r(&tTime, &stTm);
		PROCESS_ERROR(pTm != nullptr);
	}
#	endif

	stTime.m_Sec  = stTm.tm_sec;
	stTime.m_Min  = stTm.tm_min;
	stTime.m_Hour = stTm.tm_hour;
	stTime.m_Mday = stTm.tm_mday;
	stTime.m_Mon  = stTm.tm_mon + 1;
	stTime.m_Year = stTm.tm_year + 1900;
	stTime.m_Wday = stTm.tm_wday;
	stTime.m_Yday = stTm.tm_yday;
	stTime.m_Msec = (LPUINT32)(stTime.m_stTimeval.m_qwUsec / 1000);

Exit0:
	return stTime;
}

LPUINT64 LPAPI LPTime::GetNowTimetamp()
{
	return time(NULL) + ms_qwTimezoneSec;
}

LPTime LPAPI LPTime::GetNowUTCTime()
{
	LPTime stTime;
	time_t tTime = 0;;
	struct tm stTm;
	struct tm* pTm = nullptr;

	lpGetTimeEx(stTime.m_stTimeval);

	tTime = stTime.m_stTimeval.m_qwSec;
#	ifdef _WIN32
	{
		LPINT32 nResult = gmtime_s(&stTm, &tTime);
		PROCESS_ERROR(nResult == 0);
	}
#	else
	{
		pTm = gmtime_r(&tTime, &stTm);
		PROCESS_ERROR(pTm != nullptr);
	}
#	endif

	stTime.m_Sec  = stTm.tm_sec;
	stTime.m_Min  = stTm.tm_min;
	stTime.m_Hour = stTm.tm_hour;
	stTime.m_Mday = stTm.tm_mday;
	stTime.m_Mon  = stTm.tm_mon + 1;
	stTime.m_Year = stTm.tm_year + 1900;
	stTime.m_Wday = stTm.tm_wday;
	stTime.m_Yday = stTm.tm_yday;
	stTime.m_Msec = (LPUINT32)(stTime.m_stTimeval.m_qwUsec / 1000);

Exit0:
	return stTime;
}

LPUINT64 LPAPI LPTime::GetNowUTCTimestamp()
{
	return time(NULL);
}



//end声明所处的名字空间

NS_LZPL_END