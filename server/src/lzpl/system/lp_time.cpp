#include "lp_time.h"
#include "lp_processerror.h"
#include "lp_system.h"

//begin声明所处的名字空间
NS_LZPL_BEGIN



INT_32 LZPL::LPTime::ms_nTimezone = 0;
UINT_64 LZPL::LPTime::ms_qwTimezoneSec = 0;;



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

BOOL LPAPI LPTime::GlobalInit(INT_32 nTimezone)
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
	INT_32 nResult = 0;
	LPTime stTime;
	time_t tTime = 0;;
	struct tm stTm;

	lpGetTimeEx(stTime.m_stTimeval);
	stTime.m_stTimeval.m_qwSec += ms_qwTimezoneSec;

	tTime = stTime.m_stTimeval.m_qwSec;
	nResult = gmtime_s(&stTm, &tTime);
	PROCESS_ERROR(nResult == 0);

	stTime.m_Sec  = stTm.tm_sec;
	stTime.m_Min  = stTm.tm_min;
	stTime.m_Hour = stTm.tm_hour;
	stTime.m_Mday = stTm.tm_mday;
	stTime.m_Mon  = stTm.tm_mon + 1;
	stTime.m_Year = stTm.tm_year + 1900;
	stTime.m_Wday = stTm.tm_wday;
	stTime.m_Yday = stTm.tm_yday;
	stTime.m_Msec = (UINT_32)(stTime.m_stTimeval.m_qwUsec / 1000);

Exit0:
	return stTime;
}

UINT_64 LPAPI LPTime::GetNowTimetamp()
{
	return time(NULL) + ms_qwTimezoneSec;
}

LPTime LPAPI LPTime::GetNowUTCTime()
{
	INT_32 nResult = 0;
	LPTime stTime;
	time_t tTime = 0;;
	struct tm stTm;

	lpGetTimeEx(stTime.m_stTimeval);

	tTime = stTime.m_stTimeval.m_qwSec;
	nResult = gmtime_s(&stTm, &tTime);
	PROCESS_ERROR(nResult == 0);

	stTime.m_Sec  = stTm.tm_sec;
	stTime.m_Min  = stTm.tm_min;
	stTime.m_Hour = stTm.tm_hour;
	stTime.m_Mday = stTm.tm_mday;
	stTime.m_Mon  = stTm.tm_mon + 1;
	stTime.m_Year = stTm.tm_year + 1900;
	stTime.m_Wday = stTm.tm_wday;
	stTime.m_Yday = stTm.tm_yday;
	stTime.m_Msec = (UINT_32)(stTime.m_stTimeval.m_qwUsec / 1000);

Exit0:
	return stTime;
}

UINT_64 LPAPI LPTime::GetNowUTCTimestamp()
{
	return time(NULL);
}



//end声明所处的名字空间


NS_LZPL_END