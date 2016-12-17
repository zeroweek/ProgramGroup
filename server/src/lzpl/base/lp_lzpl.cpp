#include "lp_lzpl.h"



//begin�������������ֿռ�
NS_LZPL_BEGIN



static void lpsGlobalAssert()
{
	LPASSERT(sizeof(LPINT8) == 1);
	LPASSERT(sizeof(LPINT16) == 2);
	LPASSERT(sizeof(LPINT32) == 4);
	LPASSERT(sizeof(LPINT64) == 8);
	LPASSERT(sizeof(LPUINT8) == 1);
	LPASSERT(sizeof(LPUINT16) == 2);
	LPASSERT(sizeof(LPUINT32) == 4);
	LPASSERT(sizeof(LPUINT64) == 8);
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
	LPINT32 nResult = 0;

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
