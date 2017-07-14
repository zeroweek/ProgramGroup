//****************************************************************************/
//    author: caihy
//    date: June 16, 2015
//    description: 缓冲区接口定义
//
//****************************************************************************/
#ifndef _LPI_BUF_H_
#define _LPI_BUF_H_

#include "lp_base.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



// Summary:
//      缓冲区接口类： 支持单线程读、单线程写无锁操作
//      说明： 读操作只会修改读指针，写操作只会修改写指针。
//          可读数据大小获取后，就算之后又写入数据，可读数据也会大于获取之前的长度；
//          同理，可写数据大小获取也一样。
class DECLARE ILPLoopBuf
{
public:

    // Summary:
    //      无
    virtual ~ILPLoopBuf() {}

    // Summary:
    //      获取当前总共可读数据长度，读指针前面以及后面的可读取的数据大小都计算在内
    //      注：两次连续调用（期间没有读操作）获得的值可能不一样（调用间可能有写操作完成），
    //              所以当进行调用函数用于判断后赋值的场景，需要存储第一次调用的值用于赋值
    // Return:
    //      当前总共可读数据长度
    virtual LPUINT32 LPAPI GetTotalReadableLen() = 0;

    // Summary:
    //      获取当前一次可读数据长度，只计算读指针后面可读取的数据大小
    //      注：两次连续调用（期间没有读操作）获得的值可能不一样（调用间可能有写操作完成），
    //              所以当进行调用函数用于判断后赋值的场景，需要存储第一次调用的值用于赋值
    // Return:
    //      当前一次可读数据长度
    virtual LPUINT32 LPAPI GetOnceReadableLen() = 0;

    // Summary:
    //      获取当前总共可写长度，写指针前面以及后面的可写入的数据大小都计算在内
    //      注：两次连续调用（期间没有写操作）获得的值可能不一样（调用间可能有读操作完成），
    //              所以当进行调用函数用于判断后赋值的场景，需要存储第一次调用的值用于赋值
    // Return:
    //      当前总共可写长度
    virtual LPUINT32 LPAPI GetTotalWritableLen() = 0;

    // Summary:
    //      获取当前一次可写长度，只计算写指针后面可写入的数据大小
    //      注：两次连续调用（期间没有写操作）获得的值可能不一样（调用间可能有读操作完成），
    //              所以当进行调用函数用于判断后赋值的场景，需要存储第一次调用的值用于赋值
    // Return:
    //      当前一次可写长度
    virtual LPUINT32 LPAPI GetOnceWritableLen() = 0;

    // Summary:
    //      读取数据，该函数会自动调整<读指针>的位置，以及当前有效数据长度
    // Input:
    //      pDst：目的缓冲区
    //      dwReadLen：要读取的长度
    //      bDoRead：TRUE-读取指定长度数据，会修改有效数据长度，
    //          FALSE-只拷贝指定长度数据，不会修改有效数据长度
    //      bNullTerminate：是否向pDst填入NULL结束符
    // Return:
    //      TRUE-成功读取指定长度数据，FALSE-无任何操作
    virtual BOOL LPAPI Read(char* pDst, LPUINT32 dwReadLen, BOOL bDoRead, BOOL bNullTerminate) = 0;

    // Summary:
    //      读取数据，该函数会自动调整<读指针>的位置，以及当前有效数据长度
    // Input:
    //      pLoopBuf：目的缓冲区
    //      dwReadLen：要读取的长度
    // Return:
    //      TRUE-成功读取指定长度数据，FALSE-无任何操作
    virtual BOOL LPAPI Read(lp_shared_ptr<ILPLoopBuf> pLoopBuf, LPUINT32 dwReadLen) = 0;

    // Summary:
    //      写入数据，该函数会自动调整<写指针>的位置，以及当前有效数据长度
    // Input:
    //      pSrc：源缓冲区
    //      dwWriteLen：要写入的长度
    // Return:
    //      TRUE-成功写入指定长度数据，FALSE-无任何操作
    virtual BOOL LPAPI Write(const char* pSrc, LPUINT32 dwWriteLen) = 0;

    // Summary:
    //      获取读指针，配套GetOnceReadableLen和FinishRead使用
    virtual char* LPAPI ReadPtr() = 0;

    // Summary:
    //      获取写指针，配套GetOnceWritableLen和FinishWrite使用
    virtual char* LPAPI WritePtr() = 0;

    // Summary:
    //      配套GetOnceReadableLen和ReadPtr使用
    //      累加已读取数据大小，如果超过了实际可读的数据长度，则只累加实际可读长度
    // Input:
    //      dwReadLen：累加读取的大小
    virtual void LPAPI FinishRead(LPUINT32 dwReadLen) = 0;

    // Summary:
    //      配套GetOnceWritableLen和WritePtr使用
    //      累加已写入数据大小，如果超过了实际可写的数据长度，则只累加实际可写长度
    // Input:
    //      dwWriteLen：累加写入的大小
    virtual void LPAPI FinishWrite(LPUINT32 dwWriteLen) = 0;

public:

    static lp_shared_ptr<ILPLoopBuf> CreateBuf(LPUINT32 dwSize);
    static void ReleaseBuf(lp_shared_ptr<ILPLoopBuf>& pLoopBuf);
};



// Summary:
//      存放LPLoopBuf对象的池类，每个缓冲池对象对应一个size
class DECLARE ILPLoopBufPool
{
public:

    // Summary:
    //      无
    virtual ~ILPLoopBufPool() {}

public:

    // Summary:
    //      无
    // Input:
    //      dwBufSize：缓冲区对象大小
    //      bUsePool：是否使用对象池
    //      dwPoolInitCount: 池初始化大小
    virtual BOOL LPAPI Init(LPUINT32 dwBufSize, BOOL bUsePool, LPUINT32 dwPoolInitCount) = 0;

    // Summary:
    //      获取缓冲区对象大小
    // Return:
    //      缓冲区对象大小
    virtual LPUINT32 LPAPI GetBufSize() = 0;

    // Summary:
    //      创建指定大小的缓冲区对象
    // Return:
    //      失败返回NULL
    virtual lp_shared_ptr<ILPLoopBuf> LPAPI Create() = 0;

    // Summary:
    //      释放缓冲区对象
    // Input:
    //      pBuf：缓冲区对象
    virtual void LPAPI Release(lp_shared_ptr<ILPLoopBuf>& pBuf) = 0;

public:

    static lp_shared_ptr<ILPLoopBufPool> CreatePool();
    static void ReleasePool(lp_shared_ptr<ILPLoopBufPool>& pLoopBufPool);

};



//end声明所处的名字空间
NS_LZPL_END

#endif