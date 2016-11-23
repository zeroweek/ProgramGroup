#include "lp_normalfile.h"
#include "lp_processerror.h"


//begin声明所处的名字空间
NS_LZPL_BEGIN


DECLARE ILPFile *LPAPI lpFileOpen(const char * pcszFileName, const char * pcszMode)
{
	INT_32 nRetCode = 0;
	ILPFile* pFile = NULL;

	LOG_PROCESS_ERROR(pcszFileName);
	LOG_PROCESS_ERROR(pcszMode);

	pFile = new LPNormalFile();
	LOG_PROCESS_ERROR(pFile);

	nRetCode = ((LPNormalFile*)pFile)->Init(pcszFileName, pcszMode);
	LOG_PROCESS_ERROR(nRetCode);

	return pFile;

Exit0:
	SAFE_DELETE(pFile);
	return NULL;
}

DECLARE BOOL LPAPI lpFileIsExist(const char * pcszFileName)
{
	INT_32 nRetCode = 0;
	FILE* fp = NULL;

	LOG_PROCESS_ERROR(pcszFileName);

	nRetCode = fopen_s(&fp, pcszFileName, "r");
	LOG_PROCESS_ERROR(0 == nRetCode);

	fclose(fp);
	return TRUE;

Exit0:
	return FALSE;
}

LPNormalFile::LPNormalFile()
{
	m_pFile = NULL;
}

LPNormalFile::~LPNormalFile()
{
	UnInit();
}

BOOL LPAPI LPNormalFile::Init(const char * pcszFileName, const char * pcszMode)
{
	INT_32 nRetCode = 0;

	LOG_PROCESS_ERROR(pcszFileName);
	LOG_PROCESS_ERROR(pcszMode);
	LOG_PROCESS_ERROR(NULL == m_pFile);

	nRetCode = fopen_s(&m_pFile, pcszFileName, pcszMode);
	LOG_PROCESS_ERROR(0 == nRetCode);

	return TRUE;
Exit0:
	return FALSE;
}

BOOL LPAPI LPNormalFile::UnInit(void)
{
	if (m_pFile)
	{
		fclose(m_pFile);
	}
	m_pFile = NULL;

	return TRUE;
}

void LPAPI LPNormalFile::Close(void)
{
	UnInit();

	delete this;
}

UINT_32 LPAPI LPNormalFile::Read(void * pData, UINT_32 dwLen)
{
	LOG_PROCESS_ERROR(pData);

	return (UINT_32)fread(pData, 1, dwLen, m_pFile);

Exit0:
	return 0;
}

UINT_32 LPAPI LPNormalFile::Write(const void * pData, UINT_32 dwLen)
{
	LOG_PROCESS_ERROR(pData);

	return (UINT_32)fwrite(pData, 1, dwLen, m_pFile);

Exit0:
	return 0;
}

BOOL LPAPI LPNormalFile::Seek(INT_32 nOffset, INT_32 nOrigin)
{
	LOG_PROCESS_ERROR(m_pFile);

	return (fseek(m_pFile, nOffset, nOrigin) == 0);

Exit0:
	return FALSE;
}

UINT_32 LPAPI LPNormalFile::Tell(void)
{
	LOG_PROCESS_ERROR(m_pFile);

	return ftell(m_pFile);

Exit0:
	return 0;
}

BOOL LPAPI LPNormalFile::Eof(void)
{
	if (m_pFile)
	{
		return feof(m_pFile);
	}

	return TRUE;
}

UINT_32 LPAPI LPNormalFile::Size(void)
{
	INT_32 nRetCode = 0;
	UINT_32 dwSize = 0;
	UINT_32 dwCurrentPos = 0;

	LOG_PROCESS_ERROR(m_pFile);

	dwCurrentPos = ftell(m_pFile);

	nRetCode = fseek(m_pFile, 0, SEEK_END);
	LOG_PROCESS_ERROR(0 == nRetCode);

	dwSize = ftell(m_pFile);

	nRetCode = fseek(m_pFile, dwCurrentPos, SEEK_SET);
	LOG_PROCESS_ERROR(0 == nRetCode);

Exit0:
	return dwSize;
}








//end声明所处的名字空间
NS_LZPL_END
