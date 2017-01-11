//****************************************************************************/
//    author: caihy
//    date: June 11, 2015
//    description: 
//
//****************************************************************************/
#ifndef _LP_TIME_H_
#define _LP_TIME_H_

#include "lp_base.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



#define TIME_ZONE_MIN               (-23)
#define TIME_ZONE_MAX               (23)
#define ONE_HOUR_SEC                (3600)
#define ONE_DAY_SEC                 (86400)



// Summary:
//		时间值结构体定义
struct TIME_VALUE
{
	LPUINT64    m_qwSec;      // 秒
	LPUINT64    m_qwUsec;     // 微妙
};



// Summary:
//   时间类
class DECLARE LPTime
{
public:

	// Summary:
	//   无
	LPTime();

	// Summary:
	//   无
	LPTime(LPUINT64 qwTimeStamp);

	// Summary:
	//   无
	~LPTime();

	// Summary:
	//   判断时间是否相等（秒） 
	BOOL LPAPI SecEqualTo(const LPTime& t);

	// Summary:
	//   判断时间是否相等（毫秒） 
	BOOL LPAPI UsecEqualTo(const LPTime& t);

	inline LPUINT32 LPAPI GetYear() { return m_Year; }
	inline LPUINT32 LPAPI GetMon() { return m_Mon; }
	inline LPUINT32 LPAPI GetMday() { return m_Mday; }
	inline LPUINT32 LPAPI GetHour() { return m_Hour; }
	inline LPUINT32 LPAPI GetMin() { return m_Min; }
	inline LPUINT32 LPAPI GetSec() { return m_Sec; }
	inline LPUINT32 LPAPI GetMsec() { return m_Msec; }
	inline LPUINT32 LPAPI GetUsec() { return (LPUINT32)m_stTimeval.m_qwUsec; }
	inline LPUINT64 LPAPI GetTimestamp() { return m_stTimeval.m_qwSec; }

public:

	// Summary:
	//   时间全局初始化
	// Return:
	//   TRUE-成功，FALSE-失败
	static BOOL LPAPI GlobalInit(LPINT32 nTimezone);

	// Summary:
	//   获取当前时间，带时区
	static LPTime LPAPI GetNowTime();

	// Summary:
	//   获取当前时间戳，带时区
	static LPUINT64 LPAPI GetNowTimetamp();

	// Summary:
	//   获取当前时间，不带时区
	static LPTime LPAPI GetNowUTCTime();

	// Summary:
	//   获取当前时间戳，不带时区
	static LPUINT64 LPAPI GetNowUTCTimestamp();

private:

	LPUINT32                 m_Sec;            // seconds after the minute   - [0,59]
	LPUINT32                 m_Min;            // minutes after the hour     - [0,59]
	LPUINT32                 m_Hour;           // hours since midnight       - [0,23]
	LPUINT32                 m_Mday;           // day of the month           - [1,31]
	LPUINT32                 m_Mon;            // months since January       - [1,12]
	LPUINT32                 m_Year;           // 
	LPUINT32                 m_Wday;           // days since Sunday          - [0,6]
	LPUINT32                 m_Yday;           // days since January 1       - [0,365]
	LPUINT32                 m_Msec;           //

	TIME_VALUE             m_stTimeval;      // 

	static LPINT32          ms_nTimezone;     // 时区
	static LPUINT64         ms_qwTimezoneSec; //
};



//end声明所处的名字空间
NS_LZPL_END

#endif