#include "lp_global.h"
#include "lp_processerror.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN


DECLARE LPUINT8 LPAPI lpNumToHex(LPUINT8 n, BOOL bCapital)
{
	LPUINT8 cResult = 0;

	LOG_PROCESS_ERROR(n < 16);

	if (bCapital)
	{
		cResult =  n > 9 ? n + 55 : n + 48;
	}
	else
	{
		cResult =  n > 9 ? n + 87 : n + 48;
	}

Exit0:
	return cResult;
}

DECLARE LPUINT8 LPAPI lpHexToNum(LPUINT8 h)
{
	LPUINT8 n = 0;

	if (h >= 'A' && h <= 'Z')
	{
		n = h - 'A' + 10;
	}
	else if (h >= 'a' && h <= 'z')
	{
		n = h - 'a' + 10;
	}
	else if (h >= '0' && h <= '9')
	{
		n = h - '0';
	}
	else
	{
		LOG_PROCESS_ERROR(FALSE);
	}

Exit0:
	return n;
}











//end声明所处的名字空间

NS_LZPL_END
