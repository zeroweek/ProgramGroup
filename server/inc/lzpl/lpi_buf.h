//****************************************************************************/
//    author: caihy
//    date: June 16, 2015
//    description: �������ӿڶ���
//
//****************************************************************************/
#ifndef _LPI_BUF_H_
#define _LPI_BUF_H_

#include "lp_base.h"



//begin�������������ֿռ�
NS_LZPL_BEGIN



// Summary:
//      �������ӿ��ࣺ ֧�ֵ��̶߳������߳�д��������
//      ˵���� ������ֻ���޸Ķ�ָ�룬д����ֻ���޸�дָ�롣
//          �ɶ����ݴ�С��ȡ�󣬾���֮����д�����ݣ��ɶ�����Ҳ����ڻ�ȡ֮ǰ�ĳ��ȣ�
//          ͬ����д���ݴ�С��ȡҲһ����
class DECLARE ILPLoopBuf
{
public:

    // Summary:
    //      ��
    virtual ~ILPLoopBuf() {}

    // Summary:
    //      ��ȡ��ǰ�ܹ��ɶ����ݳ��ȣ���ָ��ǰ���Լ�����Ŀɶ�ȡ�����ݴ�С����������
    //      ע�������������ã��ڼ�û�ж���������õ�ֵ���ܲ�һ�������ü������д������ɣ���
    //              ���Ե����е��ú��������жϺ�ֵ�ĳ�������Ҫ�洢��һ�ε��õ�ֵ���ڸ�ֵ
    // Return:
    //      ��ǰ�ܹ��ɶ����ݳ���
    virtual LPUINT32 LPAPI GetTotalReadableLen() = 0;

    // Summary:
    //      ��ȡ��ǰһ�οɶ����ݳ��ȣ�ֻ�����ָ�����ɶ�ȡ�����ݴ�С
    //      ע�������������ã��ڼ�û�ж���������õ�ֵ���ܲ�һ�������ü������д������ɣ���
    //              ���Ե����е��ú��������жϺ�ֵ�ĳ�������Ҫ�洢��һ�ε��õ�ֵ���ڸ�ֵ
    // Return:
    //      ��ǰһ�οɶ����ݳ���
    virtual LPUINT32 LPAPI GetOnceReadableLen() = 0;

    // Summary:
    //      ��ȡ��ǰ�ܹ���д���ȣ�дָ��ǰ���Լ�����Ŀ�д������ݴ�С����������
    //      ע�������������ã��ڼ�û��д��������õ�ֵ���ܲ�һ�������ü�����ж�������ɣ���
    //              ���Ե����е��ú��������жϺ�ֵ�ĳ�������Ҫ�洢��һ�ε��õ�ֵ���ڸ�ֵ
    // Return:
    //      ��ǰ�ܹ���д����
    virtual LPUINT32 LPAPI GetTotalWritableLen() = 0;

    // Summary:
    //      ��ȡ��ǰһ�ο�д���ȣ�ֻ����дָ������д������ݴ�С
    //      ע�������������ã��ڼ�û��д��������õ�ֵ���ܲ�һ�������ü�����ж�������ɣ���
    //              ���Ե����е��ú��������жϺ�ֵ�ĳ�������Ҫ�洢��һ�ε��õ�ֵ���ڸ�ֵ
    // Return:
    //      ��ǰһ�ο�д����
    virtual LPUINT32 LPAPI GetOnceWritableLen() = 0;

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
    virtual BOOL LPAPI Read(char* pDst, LPUINT32 dwReadLen, BOOL bDoRead, BOOL bNullTerminate) = 0;

    // Summary:
    //      ��ȡ���ݣ��ú������Զ�����<��ָ��>��λ�ã��Լ���ǰ��Ч���ݳ���
    // Input:
    //      pLoopBuf��Ŀ�Ļ�����
    //      dwReadLen��Ҫ��ȡ�ĳ���
    // Return:
    //      TRUE-�ɹ���ȡָ���������ݣ�FALSE-���κβ���
    virtual BOOL LPAPI Read(lp_shared_ptr<ILPLoopBuf> pLoopBuf, LPUINT32 dwReadLen) = 0;

    // Summary:
    //      д�����ݣ��ú������Զ�����<дָ��>��λ�ã��Լ���ǰ��Ч���ݳ���
    // Input:
    //      pSrc��Դ������
    //      dwWriteLen��Ҫд��ĳ���
    // Return:
    //      TRUE-�ɹ�д��ָ���������ݣ�FALSE-���κβ���
    virtual BOOL LPAPI Write(const char* pSrc, LPUINT32 dwWriteLen) = 0;

    // Summary:
    //      ��ȡ��ָ�룬����GetOnceReadableLen��FinishReadʹ��
    virtual char* LPAPI ReadPtr() = 0;

    // Summary:
    //      ��ȡдָ�룬����GetOnceWritableLen��FinishWriteʹ��
    virtual char* LPAPI WritePtr() = 0;

    // Summary:
    //      ����GetOnceReadableLen��ReadPtrʹ��
    //      �ۼ��Ѷ�ȡ���ݴ�С�����������ʵ�ʿɶ������ݳ��ȣ���ֻ�ۼ�ʵ�ʿɶ�����
    // Input:
    //      dwReadLen���ۼӶ�ȡ�Ĵ�С
    virtual void LPAPI FinishRead(LPUINT32 dwReadLen) = 0;

    // Summary:
    //      ����GetOnceWritableLen��WritePtrʹ��
    //      �ۼ���д�����ݴ�С�����������ʵ�ʿ�д�����ݳ��ȣ���ֻ�ۼ�ʵ�ʿ�д����
    // Input:
    //      dwWriteLen���ۼ�д��Ĵ�С
    virtual void LPAPI FinishWrite(LPUINT32 dwWriteLen) = 0;

public:

    static lp_shared_ptr<ILPLoopBuf> CreateBuf(LPUINT32 dwSize);
    static void ReleaseBuf(lp_shared_ptr<ILPLoopBuf>& pLoopBuf);
};



// Summary:
//      ���LPLoopBuf����ĳ��࣬ÿ������ض����Ӧһ��size
class DECLARE ILPLoopBufPool
{
public:

    // Summary:
    //      ��
    virtual ~ILPLoopBufPool() {}

public:

    // Summary:
    //      ��
    // Input:
    //      dwBufSize�������������С
    //      bUsePool���Ƿ�ʹ�ö����
    //      dwPoolInitCount: �س�ʼ����С
    virtual BOOL LPAPI Init(LPUINT32 dwBufSize, BOOL bUsePool, LPUINT32 dwPoolInitCount) = 0;

    // Summary:
    //      ��ȡ�����������С
    // Return:
    //      �����������С
    virtual LPUINT32 LPAPI GetBufSize() = 0;

    // Summary:
    //      ����ָ����С�Ļ���������
    // Return:
    //      ʧ�ܷ���NULL
    virtual lp_shared_ptr<ILPLoopBuf> LPAPI Create() = 0;

    // Summary:
    //      �ͷŻ���������
    // Input:
    //      pBuf������������
    virtual void LPAPI Release(lp_shared_ptr<ILPLoopBuf>& pBuf) = 0;

public:

    static lp_shared_ptr<ILPLoopBufPool> CreatePool();
    static void ReleasePool(lp_shared_ptr<ILPLoopBufPool>& pLoopBufPool);

};



//end�������������ֿռ�
NS_LZPL_END

#endif