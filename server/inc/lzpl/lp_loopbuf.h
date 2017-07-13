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



//begin声明所处的名字空间
NS_LZPL_BEGIN



#define BUF_INVALID_POOL_ID            (0)    // 无效缓冲池ID



// Summary:
//      缓冲区接口类： 支持单线程读、单线程写无锁操作
//      说明： 读操作只会修改读指针，写操作只会修改写指针。
//          可读数据大小获取后，就算之后又写入数据，可读数据也会大于获取之前的长度；
//          同理，可写数据大小获取也一样。
class DECLARE LPLoopBuf : public ILPLoopBuf
{
public:

    // Summary:
    //      无
    LPLoopBuf();

    // Summary:
    //      无
    virtual ~LPLoopBuf();

    // Summary:
    //      初始化，开始内存分配操作
    // Input:
    //      dwSize：该缓冲区的实际内存大小
    //      dwPoolId：所在池的id
    // Return:
    //      TRUE-内存分配成功，FALSE-内存分配失败
    BOOL LPAPI Init(LPUINT32 dwSize, LPUINT32 dwPoolId = BUF_INVALID_POOL_ID);

    // Summary:
    //      无
    BOOL LPAPI UnInit();

    // Summary:
    //      无
    void LPAPI Reset();

    // Summary:
    //      获取所在池的id
    // Return:
    //      池id
    LPUINT32 LPAPI GetPoolId();

    // Summary:
    //      获取当前总共可读数据长度，读指针前面以及后面的可读取的数据大小都计算在内
    //      注：两次连续调用（期间没有读操作）获得的值可能不一样（调用间可能有写操作完成），
    //              所以当进行调用函数用于判断后赋值的场景，需要存储第一次调用的值用于赋值
    // Return:
    //      当前总共可读数据长度
    virtual LPUINT32 LPAPI GetTotalReadableLen();

    // Summary:
    //      获取当前一次可读数据长度，只计算读指针后面可读取的数据大小
    //      注：两次连续调用（期间没有读操作）获得的值可能不一样（调用间可能有写操作完成），
    //              所以当进行调用函数用于判断后赋值的场景，需要存储第一次调用的值用于赋值
    // Return:
    //      当前一次可读数据长度
    virtual LPUINT32 LPAPI GetOnceReadableLen();

    // Summary:
    //      获取当前总共可写长度，写指针前面以及后面的可写入的数据大小都计算在内
    //      注：两次连续调用（期间没有写操作）获得的值可能不一样（调用间可能有读操作完成），
    //              所以当进行调用函数用于判断后赋值的场景，需要存储第一次调用的值用于赋值
    // Return:
    //      当前总共可写长度
    virtual LPUINT32 LPAPI GetTotalWritableLen();

    // Summary:
    //      获取当前一次可写长度，只计算写指针后面可写入的数据大小
    //      注：两次连续调用（期间没有写操作）获得的值可能不一样（调用间可能有读操作完成），
    //              所以当进行调用函数用于判断后赋值的场景，需要存储第一次调用的值用于赋值
    // Return:
    //      当前一次可写长度
    virtual LPUINT32 LPAPI GetOnceWritableLen();

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
    virtual BOOL LPAPI Read(char* pDst, LPUINT32 dwReadLen, BOOL bDoRead, BOOL bNullTerminate);

    // Summary:
    //      写入数据，该函数会自动调整<写指针>的位置，以及当前有效数据长度
    // Input:
    //      pSrc：源缓冲区
    //      dwWriteLen：要写入的长度
    // Return:
    //      TRUE-成功写入指定长度数据，FALSE-无任何操作
    virtual BOOL LPAPI Write(const char* pSrc, LPUINT32 dwWriteLen);

    // Summary:
    //      获取读指针，配套GetOnceReadableLen和FinishRead使用
    virtual char* LPAPI ReadPtr();

    // Summary:
    //      获取写指针，配套GetOnceWritableLen和FinishWrite使用
    //      注：支持单线程读、单线程写无锁操作
    virtual char* LPAPI WritePtr();

    // Summary:
    //      配套GetOnceReadableLen和ReadPtr使用
    //      累加已读取数据大小，如果超过了实际可读的数据长度，则只累加实际可读长度
    // Input:
    //      dwReadLen：累加读取的大小
    virtual void LPAPI FinishRead(LPUINT32 dwReadLen);

    // Summary:
    //      配套GetOnceWritableLen和WritePtr使用
    //      累加已写入数据大小，如果超过了实际可写的数据长度，则只累加实际可写长度
    // Input:
    //      dwWriteLen：累加写入的大小
    virtual void LPAPI FinishWrite(LPUINT32 dwWriteLen);

private:

    char*                   m_pBuf;          // 缓冲区内存
    char*                   m_pRead;         // 读指针，数据从该位置开始读取
    char*                   m_pWrite;        // 写指针，数据从该位置开始写入
    volatile atomic_uint    m_dwDataLen;     // 有效数据长度

    LPUINT32                 m_dwBufSize;     // 缓冲区大小
    LPUINT32                 m_dwPoolId;      // 所在池的id
};



// Summary:
//      存放LPLoopBuf对象的池类，每个缓冲池对象对应一个size
class DECLARE LPLoopBufPool : public ILPLoopBufPool
{
public:

    typedef std::set<lp_shared_ptr<ILPLoopBuf>> LPSetBuf;
    typedef std::list<lp_shared_ptr<ILPLoopBuf>> LPListBuf;

public:

    // Summary:
    //      无
    LPLoopBufPool();

    // Summary:
    //      无
    virtual ~LPLoopBufPool();

public:

    // Summary:
    //      无
    // Input:
    //      dwBufSize：缓冲区对象大小
    //      bUsePool：是否使用对象池
    //      dwPoolInitCount: 池初始化大小
    virtual BOOL LPAPI Init(LPUINT32 dwBufSize, BOOL bUsePool, LPUINT32 dwPoolInitCount);

    // Summary:
    //      获取缓冲区对象大小
    // Return:
    //      缓冲区对象大小
    virtual LPUINT32 LPAPI GetBufSize();

    // Summary:
    //      创建指定大小的缓冲区对象
    // Return:
    //      失败返回NULL
    virtual lp_shared_ptr<ILPLoopBuf> LPAPI Create();

    // Summary:
    //      释放缓冲区对象
    // Input:
    //      pBuf：缓冲区对象
    virtual void LPAPI Release(lp_shared_ptr<ILPLoopBuf>& pBuf);

private:

    // Summary:
    //      无
    BOOL LPAPI UnInit();

    // Summary:
    //      预先批量创建
    // Input:
    //      dwBatchCount: 创建个数
    BOOL LPAPI PreBatchCreate(LPUINT32 dwBatchCount);

    // Summary:
    //      获取池对象id
    // Return:
    //      id
    static LPUINT32 LPAPI CreatePoolId();

private:

    BOOL                     m_bUsePool;     // 是否使用对象池
    LPUINT32                  m_dwPoolId;     // 池id
    LPUINT32                  m_dwSizeBuf;    // 池中每个缓冲区对象的内存大小
    LPUINT32                  dwPoolInitCount;// 池初始化大小
    LPListBuf                m_oFreeBufList; // 空闲缓冲区对象列表
    LPSetBuf                 m_oBufSet;      // 缓冲对象记录集，用于记录所有经由该池创建的对象

    static LPUINT32           m_dwBaseId;  // 基础ID
    static LPLock            *m_poLock;    // id创建锁

};



// Summary:
//      LPLoopBufPool池map
typedef std::map<LPUINT32, LPLoopBufPool*>   MAP_LOOP_BUF_POOL;



//end声明所处的名字空间
NS_LZPL_END

#endif