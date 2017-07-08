//****************************************************************************/
//    author: caihy
//    date: June 10, 2015
//    description: �ļ�������־ʵ��
//
//****************************************************************************/
#ifndef _LP_FILE_LOGGER_H_
#define _LP_FILE_LOGGER_H_

#include "lpi_logger.h"
#include "lp_system.h"
#include "lp_loopbuf.h"
#include "lp_lock.h"
#include "lp_thread.h"



//begin�������������ֿռ�
NS_LZPL_BEGIN



// Summary:
//      �ļ���־ʵ����
class LPFileLogger : public ILPFileLogger, public LPThread
{

public:

    // Summary:
    //      ��
    LPFileLogger();

    // Summary:
    //      ��
    virtual ~LPFileLogger();

    // Summary:
    //      ���ü�����1
    virtual void LPAPI AddRef(void);

    // Summary:
    //      ���ü�����1
    virtual LPUINT32 LPAPI QueryRef(void);

    // Summary:
    //      �ͷ�
    virtual void LPAPI Release();

    // Summary:
    //      ��¼��־
    // Input:
    //      pszLog����־����
    virtual void LPAPI LogText(const char *pszLog, LPUINT32 dwLen);

    // Summary:
    //      ������־��Ϣid
    // Input:
    //      pszLog����־����
    virtual LPUINT32 LPAPI CreateMsgId();

    // Summary:
    //   ��ʼ��
    // Input:
    //   stLogConfig����־����
    virtual BOOL LPAPI Init(LOG_CONFIG& stLogConfig);

    // Summary:
    //      ��
    virtual void LPAPI UnInit();

private:

    // Summary:
    //      ��
    void LPAPI _Flush();

    // Summary:
    //      ��
    void LPAPI _CloseFile();

    // Summary:
    //      �����ļ�ָ���λ��
    void LPAPI _UpdateFilePointer();

    // Summary:
    //      ��ȡ�ļ�·��
    const char* LPAPI _GetFilePath(LPTime& oTime);

    // Summary:
    //   �򿪻򴴽���־�ļ�
    FILE* LPAPI _OpenOrCreateLogFile(LPTime& oTime);

private:

    static THREAD_FUNC_DECLARE(_ThreadProc)(void* pParam);

private:

    BOOL                     m_bRun;                       // �߳����б��
    BOOL                     m_bInit;                      // �Ƿ��ʼ�����
    volatile atomic_bool     m_bErrorLog;                  // ��־����

    LPThread                 m_oThread;
    LPUINT32                  m_dwRef;                      // ����־�������ü���
    e_LogMode                m_eLogMode;                   // ��־��¼ģʽ
    LPUINT32                  m_dwOutputMask;                // �������

    char                     m_szDir[MAX_PATH];            // Ŀ¼·��
    char                     m_szFileName[MAX_FILE_NAME];  // ��־�ļ���
    char                     m_szFilePath[MAX_PATH];       // �ļ�·��

    FILE*                    m_fpFile;                     // �򿪵��ļ����
    BOOL                     m_bDirty;                     // �Ƿ������ݻ�ûfflush���ļ�
    LPLoopBuf*               m_pLoopBuf;                   // ��־������
    LPUINT32                  m_dwSubFile;                  // ���ļ���ţ����������ļ���С����Ҫ�����ļ���¼��
    LPTime                   m_oLastTime;                  // �ϴ�ʱ��
    LPTime                   m_oCurTime;                   // ��ǰʱ��
    LPUINT32                  m_dwMsgId;                    // ��Ϣid���
    LOG_CONFIG               m_stLogConfig;                // ��־����

};



//end�������������ֿռ�
NS_LZPL_END
#endif