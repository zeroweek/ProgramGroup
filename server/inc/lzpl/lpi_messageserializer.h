//****************************************************************************/
//    author: caihy
//    date: October 21, 2015
//    description: 消息解析接口定义
//
//****************************************************************************/
#ifndef _LPI_MESSAGE_PARSER_H_
#define _LPI_MESSAGE_PARSER_H_

#include "lp_base.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



// Summary:
//      消息包接口类
class DECLARE ILPMessageSerializer
{
public:

    // Summary:
    //      无
    virtual ~ILPMessageSerializer() {}

    // Summary:
    //      无
    virtual BOOL LPAPI ReadUint8(LPUINT8* pbyValue) = 0;

    // Summary:
    //      无
    virtual BOOL LPAPI WriteUint8(LPUINT8 byValue) = 0;

    // Summary:
    //      无
    virtual BOOL LPAPI ReadUint16(LPUINT16* pwValue) = 0;

    // Summary:
    //      无
    virtual BOOL LPAPI WriteUint16(LPUINT16 wValue) = 0;

    // Summary:
    //      无
    virtual BOOL LPAPI ReadUint32(LPUINT32* pdwValue) = 0;

    // Summary:
    //      无
    virtual BOOL LPAPI WriteUint32(LPUINT32 dwValue) = 0;

    // Summary:
    //      无
    virtual BOOL LPAPI ReadUint64(LPUINT64* pqwValue) = 0;

    // Summary:
    //      无
    virtual BOOL LPAPI WriteUint64(LPUINT64 qwValue) = 0;

    // Summary:
    //      无
    virtual BOOL LPAPI ReadString(char* pcszData) = 0;

    // Summary:
    //      无
    virtual BOOL LPAPI WriteString(const char* pcszData, LPUINT32 dwLen, LPUINT32 dwMaxLen) = 0;

    // Summary:
    //      无
    virtual char* LPAPI GetSerializeBuf(void) = 0;

    // Summary:
    //      无
    virtual LPUINT32 LPAPI GetSerializeSize(void) = 0;

};








//end声明所处的名字空间
NS_LZPL_END

#endif