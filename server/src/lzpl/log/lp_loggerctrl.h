//****************************************************************************/
//    author: caihy
//    date: June 10, 2015
//    description: ��־������ʵ��
//
//****************************************************************************/
#ifndef _LP_LOGGER_CTRL_H_
#define _LP_LOGGER_CTRL_H_

#include "lpi_logger.h"
#include "lp_lock.h"



//begin�������������ֿռ�
NS_LZPL_BEGIN



// Summary:
//      ��־������
class DECLARE LPLoggerCtrl : public ILPLoggerCtrl
{
public:

    // Summary:
    //      ��
    LPLoggerCtrl();

    // Summary:
    //      ��
    virtual ~LPLoggerCtrl();

    // Summary:
    //      ��ʼ��
    // Input:
    //      pLogger���ײ�ʹ�õ���־����
    //      pszPrefix����־��Ϣǰ׺��������NULL
    //      dwLogLevelConfig����־�ȼ�����
    virtual BOOL LPAPI Init(lp_shared_ptr<ILPLogger> pLogger, LOG_CONFIG& stLogConfig);

    // Summary:
    //      ��
    virtual BOOL LPAPI UnInit(void);

    // Summary:
    //      lua��Ϣ
    virtual void LPAPI Lua(const char* format, ...);

    // Summary:
    //      ����������Ϣ
    virtual void LPAPI Fatal(const char* format, ...);

    // Summary:
    //      һ���Դ���Ϣ
    virtual void LPAPI Error(const char* format, ...);

    // Summary:
    //      ����
    virtual void LPAPI Warn(const char* format, ...);

    // Summary:
    //      ��Ҫ��Ϣ
    virtual void LPAPI Important(const char* format, ...);

    // Summary:
    //      ��ͨ��Ϣ
    virtual void LPAPI Info(const char* format, ...);

    // Summary:
    //      ������Ϣ
    virtual void LPAPI Debug(const char* format, ...);

private:

    // Summary:
    //      ��
    void LPAPI _Log(e_LogLevel eLogLevel, const char* format, va_list argptr);

    // Summary:
    //      ��
    const char* LPAPI _GetLogLevelPrompt(e_LogLevel eLogLevel);

private:

    lp_shared_ptr<ILPLogger>            m_pLogger;                             // �ײ�ʹ�õ���־����
    BOOL                                m_bLogLock;                            // �Ƿ�ʹ����
    LPLock                              m_oLock;                               // ��
    LPUINT32                            m_dwLogLevelConfig;                    // ��־�ȼ����ã����Ƹ����ȼ���־�Ŀ���
    LPUINT32                            m_dwOutputMask;
    char                                m_szModulePrefix[COMMON_NAME_LEN];     // ��־ģ��ǰ׺���������ĸ�ģ���¼����־��Ϣ
    LOG_CONFIG                          m_stLogConfig;                         // ��־����
};



//end�������������ֿռ�
NS_LZPL_END
#endif