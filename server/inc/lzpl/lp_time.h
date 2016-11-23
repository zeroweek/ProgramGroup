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
	UINT_64    m_qwSec;      // ��
	UINT_64    m_qwUsec;     // ΢��
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
	~LPTime();

	// Summary:
	//   �ж�ʱ���Ƿ���ȣ��룩 
	BOOL LPAPI SecEqualTo(const LPTime& t);

	// Summary:
	//   �ж�ʱ���Ƿ���ȣ����룩 
	BOOL LPAPI UsecEqualTo(const LPTime& t);

	inline UINT_32 LPAPI GetYear() { return m_Year; }
	inline UINT_32 LPAPI GetMon() { return m_Mon; }
	inline UINT_32 LPAPI GetMday() { return m_Mday; }
	inline UINT_32 LPAPI GetHour() { return m_Hour; }
	inline UINT_32 LPAPI GetMin() { return m_Min; }
	inline UINT_32 LPAPI GetSec() { return m_Sec; }
	inline UINT_32 LPAPI GetMsec() { return m_Msec; }
	inline UINT_32 LPAPI GetUsec() { return (UINT_32)m_stTimeval.m_qwUsec; }
	inline UINT_64 LPAPI GetTimestamp() { return m_stTimeval.m_qwSec; }

public:

	// Summary:
	//   ʱ��ȫ�ֳ�ʼ��
	// Return:
	//   TRUE-�ɹ���FALSE-ʧ��
	static BOOL LPAPI GlobalInit(INT_32 nTimezone);

	// Summary:
	//   ��ȡ��ǰʱ�䣬��ʱ��
	static LPTime LPAPI GetNowTime();

	// Summary:
	//   ��ȡ��ǰʱ�������ʱ��
	static UINT_64 LPAPI GetNowTimetamp();

	// Summary:
	//   ��ȡ��ǰʱ�䣬����ʱ��
	static LPTime LPAPI GetNowUTCTime();

	// Summary:
	//   ��ȡ��ǰʱ���������ʱ��
	static UINT_64 LPAPI GetNowUTCTimestamp();

private:

	UINT_32                 m_Sec;            // seconds after the minute   - [0,59]
	UINT_32                 m_Min;            // minutes after the hour     - [0,59]
	UINT_32                 m_Hour;           // hours since midnight       - [0,23]
	UINT_32                 m_Mday;           // day of the month           - [1,31]
	UINT_32                 m_Mon;            // months since January       - [1,12]
	UINT_32                 m_Year;           // 
	UINT_32                 m_Wday;           // days since Sunday          - [0,6]
	UINT_32                 m_Yday;           // days since January 1       - [0,365]
	UINT_32                 m_Msec;           //

	TIME_VALUE             m_stTimeval;      // 

	static INT_32          ms_nTimezone;     // ʱ��
	static UINT_64         ms_qwTimezoneSec; //
};







//end�������������ֿռ�
NS_LZPL_END
#endif