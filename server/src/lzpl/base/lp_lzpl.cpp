#include "lp_lzpl.h"



//begin�������������ֿռ�
NS_LZPL_BEGIN



static void lpsGlobalAssert()
{
	LPASSERT(sizeof(INT_8) == 1);
	LPASSERT(sizeof(INT_16) == 2);
	LPASSERT(sizeof(INT_32) == 4);
	LPASSERT(sizeof(INT_64) == 8);
	LPASSERT(sizeof(UINT_8) == 1);
	LPASSERT(sizeof(UINT_16) == 2);
	LPASSERT(sizeof(UINT_32) == 4);
	LPASSERT(sizeof(UINT_64) == 8);
}

// Summary:
//   ��ʼ��LZPLĬ����־���ƶ���
//   ע��ʹ�ò�Ӧ����ʹ��LZPL���֮ǰ���ô���־ģ���ʼ������
// Input:
//  stLogConfig: ��־����
// Return:
//		TRUE-�ɹ���FALSE-ʧ��
DECLARE BOOL LPAPI lpInitLzpl(LOG_CONFIG& stLogConfig)
{
	INT_32 nResult = 0;

	lpsGlobalAssert();

	nResult = LPTime::GlobalInit(stLogConfig.nTimezone);
	PRINTF_PROCESS_ERROR(nResult);

	nResult = lpInitLzplLoggerCtrl(stLogConfig);
	PRINTF_PROCESS_ERROR(nResult);

	return TRUE;

Exit0:
	return FALSE;
}


DECLARE void LPAPI lpUnInitLzpl()
{

}









//end�������������ֿռ�

NS_LZPL_END
