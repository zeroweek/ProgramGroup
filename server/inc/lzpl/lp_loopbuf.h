//****************************************************************************/
//    author: caihy
//    date: October 20, 2014
//    description:
//
//****************************************************************************/
#ifndef _LP_LOOP_BUF_POOL_H_
#define _LP_LOOP_BUF_POOL_H_

#include "lpi_buf.h"
#include "lp_system.h"
#include "lp_lock.h"



//begin�������������ֿռ�
NS_LZPL_BEGIN



#define BUF_INVALID_POOL_ID            (0)    // ��Ч�����ID



// Summary:
//      �������ӿ��ࣺ ֧�ֵ��̶߳������߳�д��������
//      ˵���� ������ֻ���޸Ķ�ָ�룬д����ֻ���޸�дָ�롣
//          �ɶ����ݴ�С��ȡ�󣬾���֮����д�����ݣ��ɶ�����Ҳ����ڻ�ȡ֮ǰ�ĳ��ȣ�
//          ͬ����д���ݴ�С��ȡҲһ����
class DECLARE LPLoopBuf : public ILPLoopBuf
{
public:

    // Summary:
    //      ��
    LPLoopBuf();

    // Summary:
    //      ��
    virtual ~LPLoopBuf();

    // Summary:
    //      ��ʼ������ʼ�ڴ�������
    // Input:
    //      dwSize���û�������ʵ���ڴ��С
    //      dwPoolId�����ڳص�id
    // Return:
    //      TRUE-�ڴ����ɹ���FALSE-�ڴ����ʧ��
    BOOL LPAPI Init(LPUINT32 dwSize, LPUINT32 dwPoolId = BUF_INVALID_POOL_ID);

    // Summary:
    //      ��
    BOOL LPAPI UnInit();

    // Summary:
    //      ��
    void LPAPI Reset();

    // Summary:
    //      ��ȡ���ڳص�id
    // Return:
    //      ��id
    LPUINT32 LPAPI GetPoolId();

    // Summary:
    //      ��ȡ��ǰ�ܹ��ɶ����ݳ��ȣ���ָ��ǰ���Լ�����Ŀɶ�ȡ�����ݴ�С����������
    //      ע�������������ã��ڼ�û�ж���������õ�ֵ���ܲ�һ�������ü������д������ɣ���
    //              ���Ե����е��ú��������жϺ�ֵ�ĳ�������Ҫ�洢��һ�ε��õ�ֵ���ڸ�ֵ
    // Return:
    //      ��ǰ�ܹ��ɶ����ݳ���
    virtual LPUINT32 LPAPI GetTotalReadableLen();

    // Summary:
    //      ��ȡ��ǰһ�οɶ����ݳ��ȣ�ֻ�����ָ�����ɶ�ȡ�����ݴ�С
    //      ע�������������ã��ڼ�û�ж���������õ�ֵ���ܲ�һ�������ü������д������ɣ���
    //              ���Ե����е��ú��������жϺ�ֵ�ĳ�������Ҫ�洢��һ�ε��õ�ֵ���ڸ�ֵ
    // Return:
    //      ��ǰһ�οɶ����ݳ���
    virtual LPUINT32 LPAPI GetOnceReadableLen();

    // Summary:
    //      ��ȡ��ǰ�ܹ���д���ȣ�дָ��ǰ���Լ�����Ŀ�д������ݴ�С����������
    //      ע�������������ã��ڼ�û��д��������õ�ֵ���ܲ�һ�������ü�����ж�������ɣ���
    //              ���Ե����е��ú��������жϺ�ֵ�ĳ�������Ҫ�洢��һ�ε��õ�ֵ���ڸ�ֵ
    // Return:
    //      ��ǰ�ܹ���д����
    virtual LPUINT32 LPAPI GetTotalWritableLen();

    // Summary:
    //      ��ȡ��ǰһ�ο�д���ȣ�ֻ����дָ������д������ݴ�С
    //      ע�������������ã��ڼ�û��д��������õ�ֵ���ܲ�һ�������ü�����ж�������ɣ���
    //              ���Ե����е��ú��������жϺ�ֵ�ĳ�������Ҫ�洢��һ�ε��õ�ֵ���ڸ�ֵ
    // Return:
    //      ��ǰһ�ο�д����
    virtual LPUINT32 LPAPI GetOnceWritableLen();

    // Summary:
    //      ��ȡ���ݣ��ú������Զ�����<��ָ��>��λ�ã��Լ���ǰ��Ч���ݳ���
    // Input:
    //      pDst��Ŀ�Ļ�����
    //      dwReadLen��Ҫ��ȡ�ĳ���
    //      bDoRead��TRUE-��ȡָ���������ݣ����޸���Ч���ݳ��ȣ�
    //          FALSE-ֻ����ָ���������ݣ������޸���Ч���ݳ���
    //      bNullTerminate���Ƿ���pDst����NULL������
    // Return:
    //      TRUE-�ɹ���ȡָ���������ݣ�FALSE-���κβ���
    virtual BOOL LPAPI Read(char* pDst, LPUINT32 dwReadLen, BOOL bDoRead, BOOL bNullTerminate);

    // Summary:
    //      д�����ݣ��ú������Զ�����<дָ��>��λ�ã��Լ���ǰ��Ч���ݳ���
    // Input:
    //      pSrc��Դ������
    //      dwWriteLen��Ҫд��ĳ���
    // Return:
    //      TRUE-�ɹ�д��ָ���������ݣ�FALSE-���κβ���
    virtual BOOL LPAPI Write(const char* pSrc, LPUINT32 dwWriteLen);

    // Summary:
    //      ��ȡ��ָ�룬����GetOnceReadableLen��FinishReadʹ��
    virtual char* LPAPI ReadPtr();

    // Summary:
    //      ��ȡдָ�룬����GetOnceWritableLen��FinishWriteʹ��
    //      ע��֧�ֵ��̶߳������߳�д��������
    virtual char* LPAPI WritePtr();

    // Summary:
    //      ����GetOnceReadableLen��ReadPtrʹ��
    //      �ۼ��Ѷ�ȡ���ݴ�С�����������ʵ�ʿɶ������ݳ��ȣ���ֻ�ۼ�ʵ�ʿɶ�����
    // Input:
    //      dwReadLen���ۼӶ�ȡ�Ĵ�С
    virtual void LPAPI FinishRead(LPUINT32 dwReadLen);

    // Summary:
    //      ����GetOnceWritableLen��WritePtrʹ��
    //      �ۼ���д�����ݴ�С�����������ʵ�ʿ�д�����ݳ��ȣ���ֻ�ۼ�ʵ�ʿ�д����
    // Input:
    //      dwWriteLen���ۼ�д��Ĵ�С
    virtual void LPAPI FinishWrite(LPUINT32 dwWriteLen);

private:

    char*                   m_pBuf;          // �������ڴ�
    char*                   m_pRead;         // ��ָ�룬���ݴӸ�λ�ÿ�ʼ��ȡ
    char*                   m_pWrite;        // дָ�룬���ݴӸ�λ�ÿ�ʼд��
    volatile atomic_uint    m_dwDataLen;     // ��Ч���ݳ���

    LPUINT32                 m_dwBufSize;     // ��������С
    LPUINT32                 m_dwPoolId;      // ���ڳص�id
};



// Summary:
//      ���LPLoopBuf����ĳ��࣬ÿ������ض����Ӧһ��size
class DECLARE LPLoopBufPool : public ILPLoopBufPool
{
public:

    typedef std::set<lp_shared_ptr<ILPLoopBuf>> LPSetBuf;
    typedef std::list<lp_shared_ptr<ILPLoopBuf>> LPListBuf;

public:

    // Summary:
    //      ��
    LPLoopBufPool();

    // Summary:
    //      ��
    virtual ~LPLoopBufPool();

public:

    // Summary:
    //      ��
    // Input:
    //      dwBufSize�������������С
    //      bUsePool���Ƿ�ʹ�ö����
    //      dwPoolInitCount: �س�ʼ����С
    virtual BOOL LPAPI Init(LPUINT32 dwBufSize, BOOL bUsePool, LPUINT32 dwPoolInitCount);

    // Summary:
    //      ��ȡ�����������С
    // Return:
    //      �����������С
    virtual LPUINT32 LPAPI GetBufSize();

    // Summary:
    //      ����ָ����С�Ļ���������
    // Return:
    //      ʧ�ܷ���NULL
    virtual lp_shared_ptr<ILPLoopBuf> LPAPI Create();

    // Summary:
    //      �ͷŻ���������
    // Input:
    //      pBuf������������
    virtual void LPAPI Release(lp_shared_ptr<ILPLoopBuf>& pBuf);

private:

    // Summary:
    //      ��
    BOOL LPAPI UnInit();

    // Summary:
    //      Ԥ����������
    // Input:
    //      dwBatchCount: ��������
    BOOL LPAPI PreBatchCreate(LPUINT32 dwBatchCount);

    // Summary:
    //      ��ȡ�ض���id
    // Return:
    //      id
    static LPUINT32 LPAPI CreatePoolId();

private:

    BOOL                     m_bUsePool;     // �Ƿ�ʹ�ö����
    LPUINT32                  m_dwPoolId;     // ��id
    LPUINT32                  m_dwSizeBuf;    // ����ÿ��������������ڴ��С
    LPUINT32                  dwPoolInitCount;// �س�ʼ����С
    LPListBuf                m_oFreeBufList; // ���л����������б�
    LPSetBuf                 m_oBufSet;      // ��������¼�������ڼ�¼���о��ɸóش����Ķ���

    static LPUINT32           m_dwBaseId;  // ����ID
    static LPLock            *m_poLock;    // id������

};



// Summary:
//      LPLoopBufPool��map
typedef std::map<LPUINT32, LPLoopBufPool*>   MAP_LOOP_BUF_POOL;



//end�������������ֿռ�
NS_LZPL_END

#endif