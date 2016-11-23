//****************************************************************************/
//    author: caihy
//    date: June 10, 2015
//    description: ��־����ӿڶ���
//
//****************************************************************************/
#ifndef _LPI_LOGGER_H_
#define _LPI_LOGGER_H_

#include "lp_base.h"

#ifdef _WIN32
#	include <Windows.h>
#endif



//begin�������������ֿռ�
NS_LZPL_BEGIN



// Summary:
//		��־������
enum e_LogLevel
{
	eLogLevel_Off       = 0x00000000,   // �ر�����
	eLogLevel_Debug     = 0x00000001,   // ������־
	eLogLevel_Info      = 0x00000002,   // ��ͨ��Ϣ
	eLogLevel_Warn      = 0x00000004,   // ����
	eLogLevel_Important = 0x00000008,   // ����
	eLogLevel_Error     = 0x00000010,   // һ���Դ���
	eLogLevel_Fatal     = 0x00000020,   // ��������
	eLogLevel_Lua       = 0x00000040,   // lua��Ϣ
	eLogLevel_All       = 0x0fffffff    // ������
};



// Summary:
//		�ռǼ�¼ģʽ������ļ�������־
enum e_LogMode
{
	eLogMode_None            = 0,      // none
	eLogMode_SingleFile      = 1,      // ��¼�ڵ����ļ�
	eLogMode_DayDivide       = 2,      // ����ֿ����ڲ�ͬ�ļ�
	eLogMode_MonthDivide     = 3,      // ���·ֿ����ڲ�ͬ�ļ�
	eLogMode_HourDivide      = 4,      // ��Сʱ�ֿ����ڲ�ͬ�ļ�
	eLogMode_Max
};



// Summary:
//		�ռ����������
enum e_OutputType
{
	eOutputType_Off       = 0,            // none
	eOutputType_File      = 0x00000001,   // ������ļ�
	eOutputType_Console   = 0x00000002,   // ���������̨
	eOutputType_Debug     = 0x00000004,   // �������
	eOutputType_All       = 0x0fffffff    // all
};



// Summary:
//		��־����
enum e_LogType
{
	eLogType_None            = 0,      // none
	eLogType_File            = 1,      // �ļ���־����־��¼���ļ�
	eLogType_Net             = 2,      // ������־����־���͵�����
	eLogType_Max
};



// Summary:
//   ����̨ǰ����ɫ
// Color Define:
//   0 = ��ɫ      8 = ��ɫ
//   1 = ��ɫ      9 = ����ɫ
//   2 = ��ɫ      A = ����ɫ
//   3 = ǳ��ɫ    B = ��ǳ��ɫ
//   4 = ��ɫ      C = ����ɫ
//   5 = ��ɫ      D = ����ɫ
//   6 = ��ɫ      E = ����ɫ
//   7 = ��ɫ      F = ����ɫ
enum e_ConsoleTextColor 
{
#   ifdef _WIN32
		CTC_RED = FOREGROUND_INTENSITY | FOREGROUND_RED,
		CTC_GREEN = FOREGROUND_INTENSITY | FOREGROUND_GREEN,
		CTC_BLUE = FOREGROUND_INTENSITY | FOREGROUND_BLUE,
		CTC_YELLOW = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
		CTC_PURPLE = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
		CTC_CYAN = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
		CTC_GRAY = FOREGROUND_INTENSITY,
		CTC_WHITE = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
		CTC_HIGH_WHITE = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
		CTC_BLACK = 0
#   else
		CTC_RED = 0,
		CTC_GREEN = 0,
		CTC_BLUE = 0,
		CTC_YELLOW = 0,
		CTC_PURPLE = 0,
		CTC_CYAN = 0,
		CTC_WHITE = 0,
		CTC_HIGH_WHITE = 0,
		CTC_BLACK = 0
#   endif
};



// Summary:
//   ����̨������ɫ
// Color Define:
//   ��ɫͬǰ����ɫһ����ֻ��������16λ
enum e_ConsoleBackGroundColor
{
#   ifdef _WIN32
		CBC_RED = BACKGROUND_INTENSITY | BACKGROUND_RED,
		CBC_GREEN = BACKGROUND_INTENSITY | BACKGROUND_GREEN,
		CBC_BLUE = BACKGROUND_INTENSITY | BACKGROUND_BLUE,
		CBC_YELLOW = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN,
		CBC_PURPLE = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE,
		CBC_CYAN = BACKGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_BLUE,
		CBC_WHITE = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,
		CBC_HIGH_WHITE = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,
		CBC_BLACK = 0
#   else
		CBC_RED = 0,
		CBC_GREEN = 0,
		CBC_BLUE = 0,
		CBC_YELLOW = 0,
		CBC_PURPLE = 0,
		CBC_CYAN = 0,
		CBC_WHITE = 0,
		CBC_HIGH_WHITE = 0,
		CBC_BLACK = 0
#   endif
};



// Summary:
//   ���ÿ���̨�����ɫ
DECLARE void LPAPI lpSetConsoleColor(e_ConsoleTextColor textColor = CTC_WHITE, e_ConsoleBackGroundColor backgroundColor = CBC_BLACK);



// Summary:
//   ��־���ýṹ����
struct LOG_CONFIG
{
	char                szLogDir[MAX_PATH];              //��־�ļ���Ŀ¼
	char                szFileName[MAX_FILE_NAME];       //��־�ļ���
	char                szModulePrefix[COMMON_NAME_LEN]; //��־��Ϣģ��ǰ׺
	BOOL                bWrithLock;                      //��־�Ƿ�д����
	UINT_32             dwLogMode;                       //��־ģʽ
	UINT_32             dwLogLevelConfig;                //��־�ȼ�����
	UINT_32             dwOutputMask;                    //��־�������

	UINT_32             dwMaxLogLoopBufSize;             //�ļ���־��������С��Ĭ��=10*1024*1024
	UINT_32             dwMaxLogSingleFileSize;          //�ļ���־�����ļ����������Ĭ��=10*1024*1024
	UINT_32             dwMaxLogOneMsgSize;              //��־��Ϣ����ֽ�����Ĭ��=4096
	UINT_32             dwLogFlushInterval;              //��־flush�������λ/���룩��Ĭ��=3000
	INT_32              nTimezone;                       //ʱ��
};



// Summary:
//		������־����
// Input:
//		pszDir����־�����ļ���Ŀ¼
//		pszFileName����־�����ļ���
// Return:
//		TRUE-�ɹ���FALSE-ʧ��
DECLARE BOOL LPAPI lpLoadLogConfig(const char* pszLogConfigDir, const char* pszLogConfigFileName, LOG_CONFIG& stLogConfig);



// Summary:
//		��־�ӿ���
class DECLARE ILPLogger
{
public:

	// Summary:
	//		��
	virtual ~ILPLogger(){}

	// Summary:
	//		���ü�����1
	virtual void LPAPI AddRef(void) = 0;

	// Summary:
	//		���ü�����1
	virtual UINT_32 LPAPI QueryRef(void) = 0;

	// Summary:
	//		�ͷ�
	virtual void LPAPI Release() = 0;

	// Summary:
	//		��¼��־
	// Input:
	//		pszLog����־����
	virtual void LPAPI LogText(const char *pszLog, UINT_32 dwLen) = 0;

	// Summary:
	//		������־��Ϣid
	// Input:
	//		pszLog����־����
	virtual UINT_32 LPAPI CreateMsgId() = 0;

};



// Summary:
//		��־���ƽӿ���
class DECLARE ILPLoggerCtrl
{
public:

	// Summary:
	//		��
	virtual ~ILPLoggerCtrl(){}

	// Summary:
	//   ��ʼ��
	// Input:
	//   pLogger���ײ�ʹ�õ���־����
	//   stLogConfig����־����
	virtual BOOL LPAPI Init(ILPLogger* pLogger, LOG_CONFIG& stLogConfig) = 0;

	// Summary:
	//		lua��Ϣ
	virtual void LPAPI Lua(const char* format, ...) = 0;

	// Summary:
	//		����������Ϣ
	virtual void LPAPI Fatal(const char* format, ...) = 0;

	// Summary:
	//		һ���Դ���Ϣ
	virtual void LPAPI Error(const char* format, ...) = 0;

	// Summary:
	//		����
	virtual void LPAPI Warn(const char* format, ...) = 0;

	// Summary:
	//		��Ҫ��Ϣ
	virtual void LPAPI Important(const char* format, ...) = 0;

	// Summary:
	//		��ͨ��Ϣ
	virtual void LPAPI Info(const char* format, ...) = 0;

	// Summary:
	//		������Ϣ
	virtual void LPAPI Debug(const char* format, ...) = 0;

	// Summary:
	//		�ͷ�
	virtual void LPAPI Release() = 0;
};



// Summary:
//		�������������־���ƶ���
// Return:
//		�����齨��־���ƶ���
DECLARE ILPLoggerCtrl* LPAPI lpCreateLoggerCtrl();



// Summary:
//		�ļ���־�ӿ���
class DECLARE ILPFileLogger : public ILPLogger
{
public:

	// Summary:
	//		��
	virtual ~ILPFileLogger(){}

	// Summary:
	//   ��ʼ��
	// Input:
	//   stLogConfig����־����
	virtual BOOL LPAPI Init(LOG_CONFIG& stLogConfig) = 0;

};



// Summary:
//		�����ļ���־����
// Return:
//		�ļ���־����
DECLARE ILPFileLogger* LPAPI lpCreateFileLogger();



// Summary:
//   ��ʼ��LZPLĬ����־���ƶ���
//   ע��ʹ�ò�Ӧ����ʹ��LZPL���֮ǰ���ô���־ģ���ʼ������
// Input:
//   stLogConfig: ��־����
// Return:
//   TRUE-�ɹ���FALSE-ʧ��
DECLARE BOOL LPAPI lpInitLzplLoggerCtrl(LOG_CONFIG& stLogConfig);

// Summary:
//		��������ڲ�ʹ�õ�ILPLoggerCtrl����ʹ��Ĭ�ϵģ�
// Input:
//		poLzplLoggerCtrl����־����������
// Return:
//		TRUE-�ɹ���FALSE-ʧ��
DECLARE BOOL LPAPI lpSetLzplLoggerCtrl(ILPLoggerCtrl* poLzplLoggerCtrl);



// Summary:
//		��ȡLZPL�ڲ���־���ƶ���
// Return:
//		�����齨��־���ƶ���
DECLARE ILPLoggerCtrl* LPAPI lpGetLzplLoggerCtrl();



//lua�����Ϣ�������Σ�
#define LUA        if(NULL!=lpGetLzplLoggerCtrl()) lpGetLzplLoggerCtrl()->Lua
//������Ϣ�������Σ�
#define FTL        if(NULL!=lpGetLzplLoggerCtrl()) lpGetLzplLoggerCtrl()->Fatal
//������Ϣ�������Σ�
#define ERR        if(NULL!=lpGetLzplLoggerCtrl()) lpGetLzplLoggerCtrl()->Error
//������Ϣ�������Σ�
#define WRN        if(NULL!=lpGetLzplLoggerCtrl()) lpGetLzplLoggerCtrl()->Warn
//��Ҫ��Ϣ�������Σ�
#define IMP        if(NULL!=lpGetLzplLoggerCtrl()) lpGetLzplLoggerCtrl()->Important
//һ����Ϣ��release�����Σ�
#define INF        if(NULL!=lpGetLzplLoggerCtrl()) lpGetLzplLoggerCtrl()->Info
//������ʾ��release���Σ�
#define DBG        if(NULL!=lpGetLzplLoggerCtrl()) lpGetLzplLoggerCtrl()->Debug





//end�������������ֿռ�
NS_LZPL_END

#endif