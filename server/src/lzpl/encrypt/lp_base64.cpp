#include "lp_base64.h"
#include "lp_processerror.h"



//begin�������������ֿռ�
NS_LZPL_BEGIN



const char gs_szBase64Table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

LPINT32 _GetBase64TableIndex(char l)
{
	LPINT32 nIndex = -1;

	if (l == '+')
	{
		nIndex = 62;
	}
	else if(l == '/')
	{
		nIndex = 63;
	}
	else if (l >= 'A' && l <= 'Z')
	{
		nIndex = l - 'A';
	}
	else if (l >= 'a' && l <= 'z')
	{
		nIndex = l - 'a' + 26;
	}
	else if(l >= '0' && l <= '9')
	{
		nIndex = l - '0' + 52;
	}
	else
	{
		LOG_PROCESS_ERROR(FALSE);
	}

Exit0:
	return nIndex;
}

DECLARE BOOL LPAPI lpBase64Encode(const char* pcszInStr, LPUINT32 dwInSize, char* pszOutStr, lpInOut LPUINT32& dwOutSize, eBase64CodeType eType)
{
	LPUINT8 byP0 = 0;
	LPUINT8 byP1 = 0;
	LPUINT8 byP2 = 0;
	LPUINT8 byTableIndex = 0;

	LPUINT32 dwIndex = 0;
	LPUINT32 dwMeetCount = 0;       // ǰ����Ա�3�������ֽ���
	LPUINT32 dwRemainCount = 0;     // ���治���Ա�3����ʣ�µ��ֽ���
	LPUINT32 dwEqualOptCount = 0;   // ��Ҫ�����'='�Ÿ���
	LPUINT32 dwOutputIndex = 0;     // 
	LPUINT32 dwOutputCount = 0;     // Ӧ��������ֽ���

	LOG_PROCESS_ERROR(pcszInStr);
	LOG_PROCESS_ERROR(pszOutStr);

	dwRemainCount = dwInSize % 3;
	dwMeetCount = dwInSize - dwRemainCount;
	dwOutputCount = (dwInSize / 3) * 4;
	if (dwRemainCount > 0)
	{
		if (dwRemainCount == 1)
		{
			dwEqualOptCount = 2;
		}
		else // dwRemainCount == 2
		{
			dwEqualOptCount = 1;
		}

		dwOutputCount += 4;
	}

	LOG_PROCESS_ERROR(dwOutputCount <= dwOutSize);

	for (; dwIndex < dwMeetCount;)
	{
		byP0 = pcszInStr[dwIndex];
		byP1 = pcszInStr[dwIndex + 1];
		byP2 = pcszInStr[dwIndex + 2];

		//��1�ֽڵ�ǰ6λ
		byTableIndex = byP0 >> 2;
		pszOutStr[dwOutputIndex++] = gs_szBase64Table[byTableIndex];

		//��1�ֽں�2λ�͵�2�ֽ�ǰ4λ
		byTableIndex = ((byP0 & 0x03) << 4) | (byP1 >> 4);
		pszOutStr[dwOutputIndex++] = gs_szBase64Table[byTableIndex];

		//��2�ֽں�4λ�͵�3�ֽ�ǰ2λ
		byTableIndex = ((byP1 & 0xf) << 2) | (byP2 >> 6);
		pszOutStr[dwOutputIndex++] = gs_szBase64Table[byTableIndex];

		//��3�ֽں�6λ
		byTableIndex = byP2 & 0x3f;
		pszOutStr[dwOutputIndex++] = gs_szBase64Table[byTableIndex];

		dwIndex += 3;
	}

	switch (dwEqualOptCount)
	{
	case 1:
		{
			byP0 = (unsigned char)pcszInStr[dwIndex];
			//��1�ֽ�ǰ6λ  
			byTableIndex = (byP0 >> 2) & 0x3f;
			pszOutStr[dwOutputIndex++] = gs_szBase64Table[byTableIndex];

			//��1�ֽں�2λ��4��0  
			byTableIndex = (byP0 & 0x03) << 4;
			pszOutStr[dwOutputIndex++] = gs_szBase64Table[byTableIndex];

			//��'='�Ų���
			pszOutStr[dwOutputIndex++] = '=';
			pszOutStr[dwOutputIndex++] = '=';
		}
		break;
	case 2:
		{
			byP0 = pcszInStr[dwIndex];
			byP1 = pcszInStr[dwIndex + 1];

			//��1�ֽ�ǰ6λ  
			byTableIndex = (byP0 >> 2) & 0x3f;
			pszOutStr[dwOutputIndex++] = gs_szBase64Table[byTableIndex];

			//��1�ֽں�2λ�͵�2�ֽ�ǰ4λ  
			byTableIndex = ((byP0 & 0x03) << 4) | ((byP1 >> 4) & 0x0f);
			pszOutStr[dwOutputIndex++] = gs_szBase64Table[byTableIndex];

			//��2�ֽں�4λ��2��0  
			byTableIndex = ((byP1 << 4) & 0xf0) >> 2;
			pszOutStr[dwOutputIndex++] = gs_szBase64Table[byTableIndex];

			//��'='�Ų���
			pszOutStr[dwOutputIndex++] = '=';
		}
		break;
	default:
		break;
	}

	LOG_PROCESS_ERROR(dwOutputCount == dwOutputIndex);
	dwOutSize = dwOutputCount;

	return TRUE;
Exit0:
	return FALSE;
}

DECLARE LPINT32 LPAPI lpBase64Decode(const char* pcszInStr, LPUINT32 dwInSize, char* pszOutStr, lpInOut LPUINT32& dwOutSize, eBase64CodeType eType)
{
	LPINT32 nP0 = 0;
	LPINT32 nP1 = 0;
	LPINT32 nP2 = 0;
	LPINT32 nP3 = 0;

	LPUINT32 dwIndex = 0;
	LPUINT32 dwMeetCount = 0;       // ǰ��û�а���'='�ŵĿ��Ա�4�������ֽ���
	LPUINT32 dwEqualOptCount = 0;   // ĩβ'='�ŵĸ���
	LPUINT32 dwOutputIndex = 0;
	LPUINT32 dwOutputCount = 0;     // Ӧ��������ֽ���


	LOG_PROCESS_ERROR(pcszInStr);
	LOG_PROCESS_ERROR(pszOutStr);
	LOG_PROCESS_ERROR(dwInSize % 4 == 0 && dwInSize / 4 > 0);

	LOG_PROCESS_ERROR((pcszInStr[dwInSize - 2] == '=' && pcszInStr[dwInSize - 1] == '=') ||
		(pcszInStr[dwInSize - 2] != '=' && pcszInStr[dwInSize - 1] == '=') ||
		(pcszInStr[dwInSize - 2] != '=' && pcszInStr[dwInSize - 1] != '='));

	dwOutputCount = (dwInSize / 4) * 3;
	if (pcszInStr[dwInSize - 1] == '=')
	{
		if (pcszInStr[dwInSize - 2] != '=')
		{
			dwEqualOptCount = 1;
			dwOutputCount -= 1;
		}
		else
		{
			dwEqualOptCount = 2;
			dwOutputCount -= 2;
		}
	}

	if (dwEqualOptCount > 0)
	{
		dwMeetCount = dwInSize - 4;
	}
	else
	{
		dwMeetCount = dwInSize;
	}

	for (; dwIndex < dwMeetCount;)
	{
		nP0 = _GetBase64TableIndex(pcszInStr[dwIndex]);
		nP1 = _GetBase64TableIndex(pcszInStr[dwIndex + 1]);
		nP2 = _GetBase64TableIndex(pcszInStr[dwIndex + 2]);
		nP3 = _GetBase64TableIndex(pcszInStr[dwIndex + 3]);

		LOG_PROCESS_ERROR(nP0 != -1 && nP1 != -1 && nP2 != -1 && nP3 != -1);

		//��1�ֽڵĺ�6λ�͵�2�ֽڵ�3-4λ
		pszOutStr[dwOutputIndex++] = (nP0 << 2) | (nP1 >> 4);

		//��2�ֽڵĺ�4λ�͵�3�ֽڵ�3-6λ
		pszOutStr[dwOutputIndex++] = ((nP1 & 0xf) << 4) | (nP2 >> 2);

		//��3�ֽڵĺ�2λ�͵�4�ֽڵĺ�6λ
		pszOutStr[dwOutputIndex++] = (nP2 << 6) | nP3;

		dwIndex += 4;
	}

	switch (dwEqualOptCount)
	{
	case 1:
		{
			nP0 = _GetBase64TableIndex(pcszInStr[dwIndex]);
			nP1 = _GetBase64TableIndex(pcszInStr[dwIndex + 1]);
			nP2 = _GetBase64TableIndex(pcszInStr[dwIndex + 2]);

			pszOutStr[dwOutputIndex++] = (nP0 << 2) | (nP1 >> 4);
			pszOutStr[dwOutputIndex++] = (nP1 << 4) | (nP2 >> 2);
		}
		break;
	case 2:
		{
			nP0 = _GetBase64TableIndex(pcszInStr[dwIndex]);
			nP1 = _GetBase64TableIndex(pcszInStr[dwIndex + 1]);

			pszOutStr[dwOutputIndex++] = (nP0 << 2) | (nP1 >> 4);
		}
		break;
	default:
		break;
	}

	LOG_PROCESS_ERROR(dwOutputCount == dwOutputIndex);
	dwOutSize = dwOutputCount;

	return TRUE;
Exit0:
	return FALSE;
}



//end�������������ֿռ�
NS_LZPL_END