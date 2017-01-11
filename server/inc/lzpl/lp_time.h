//****************************************************************************/
//    author: caihy
//    date: June 11, 2015
//    description: 
//
//****************************************************************************/
#ifndef _LP_TIME_H_
#define _LP_TIME_H_

#include "lp_base.h"



//begin�������������ֿռ�
NS_LZPL_BEGIN



#define TIME_ZONE_MIN               (-23)
#define TIME_ZONE_MAX               (23)
#define ONE_HOUR_SEC                (3600)
#define ONE_DAY_SEC                 (86400)



// Summary:
//		ʱ��ֵ�ṹ�嶨��
struct TIME_VALUE
{
	LPUINT64    m_qwSec;      // ��
	LPUINT64    m_qwUsec;     // ΢��
};



// Summary:
//   ʱ����
class DECLARE LPTime
{
public:

	// Summary:
	//   ��
	LPTime();

	// Summary:
	//   ��
	LPTime(LPUINT64 qwTimeStamp);

	// Summary:
	//   ��
	~LPTime();

	// Summary:
	//   �ж�ʱ���Ƿ���ȣ��룩 
	BOOL LPAPI SecEqualTo(const LPTime& t);

	// Summary:
	//   �ж�ʱ���Ƿ���ȣ����룩 
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
	//   ʱ��ȫ�ֳ�ʼ��
	// Return:
	//   TRUE-�ɹ���FALSE-ʧ��
	static BOOL LPAPI GlobalInit(LPINT32 nTimezone);

	// Summary:
	//   ��ȡ��ǰʱ�䣬��ʱ��
	static LPTime LPAPI GetNowTime();

	// Summary:
	//   ��ȡ��ǰʱ�������ʱ��
	static LPUINT64 LPAPI GetNowTimetamp();

	// Summary:
	//   ��ȡ��ǰʱ�䣬����ʱ��
	static LPTime LPAPI GetNowUTCTime();

	// Summary:
	//   ��ȡ��ǰʱ���������ʱ��
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

	static LPINT32          ms_nTimezone;     // ʱ��
	static LPUINT64         ms_qwTimezoneSec; //
};



//end�������������ֿռ�
NS_LZPL_END

#endif