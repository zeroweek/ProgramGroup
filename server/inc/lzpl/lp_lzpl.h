//****************************************************************************/
//    author: caihy
//    contact: 1003217929@qq.com
//    date: October 21, 2015
//    description: LZPLģ��ͷ�ļ�
//
//****************************************************************************/
#ifndef _LP_LZPL_H_
#define _LP_LZPL_H_


#include "lp_base.h"
#include "lpi_logger.h"
#include "lpi_file.h"
#include "lpi_net.h"

#include "lp_dynamicarray.h"
#include "lp_simplelist.h"
#include "lp_stacklist.h"
#include "lp_baselist.h"
#include "lp_looplist.h"
#include "lp_string.h"
#include "lp_loopbuf.h"
#include "lp_system.h"
#include "lp_objpool.h"
#include "lp_processerror.h"
#include "lp_datamanager.h"
#include "lp_global.h"
#include "lp_singleton.h"
#include "lp_bt.h"
#include "lp_luascriptmgr.h"
#include "lpi_property.h"
#include "lp_table.h"


//begin�������������ֿռ�
NS_LZPL_BEGIN



// Summary:
//   ��ʼ��LZPLĬ����־���ƶ���
//   ע��ʹ�ò�Ӧ����ʹ��LZPL���֮ǰ���ô���־ģ���ʼ������
// Input:
//  stLogConfig: ��־����
// Return:
//		TRUE-�ɹ���FALSE-ʧ��
DECLARE BOOL LPAPI lpInitLzpl(LOG_CONFIG& stLogConfig);


// Summary:
//   ����ʼ��LZPLģ��
DECLARE void LPAPI lpUnInitLzpl();




//end�������������ֿռ�
NS_LZPL_END

#endif