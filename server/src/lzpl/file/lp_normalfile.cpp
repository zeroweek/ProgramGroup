#include "lp_normalfile.h"
#include "lp_processerror.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



ILPFile* LPAPI ILPFile::OpenFile(const char* pcszFileName, const char* pcszMode)
{
	LPINT32 nRetCode = 0;
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

BOOL LPAPI ILPFile::IsFileExist(const char* pcszFileName)
{
	FILE* fp = NULL;

	LOG_PROCESS_ERROR(pcszFileName);

#   ifdef _WIN32
	{
		LPINT32 nRetCode = fopen_s(&fp, pcszFileName, "r");
		LOG_PROCESS_ERROR(0 == nRetCode);
	}
#   else
	{
		fp = fopen(pcszFileName, "r");
		LOG_PROCESS_ERROR(fp != nullptr);
	}
#   endif

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
	LOG_PROCESS_ERROR(pcszFileName);
	LOG_PROCESS_ERROR(pcszMode);
	LOG_PROCESS_ERROR(NULL == m_pFile);

#   ifdef _WIN32
	{
		LPINT32 nRetCode = fopen_s(&m_pFile, pcszFileName, pcszMode);
		LOG_PROCESS_ERROR(0 == nRetCode);
	}
#   else
	{
		m_pFile = fopen(pcszFileName, pcszMode);
		LOG_PROCESS_ERROR(m_pFile != nullptr);
	}
#   endif

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

LPUINT32 LPAPI LPNormalFile::Read(void * pData, LPUINT32 dwLen)
{
	LOG_PROCESS_ERROR(pData);

	return (LPUINT32)fread(pData, 1, dwLen, m_pFile);

Exit0:
	return 0;
}

LPUINT32 LPAPI LPNormalFile::Write(const void * pData, LPUINT32 dwLen)
{
	LOG_PROCESS_ERROR(pData);

	return (LPUINT32)fwrite(pData, 1, dwLen, m_pFile);

Exit0:
	return 0;
}

BOOL LPAPI LPNormalFile::Seek(LPINT32 nOffset, LPINT32 nOrigin)
{
	LOG_PROCESS_ERROR(m_pFile);

	return (fseek(m_pFile, nOffset, nOrigin) == 0);

Exit0:
	return FALSE;
}

LPUINT32 LPAPI LPNormalFile::Tell(void)
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

LPUINT32 LPAPI LPNormalFile::Size(void)
{
	LPINT32 nRetCode = 0;
	LPUINT32 dwSize = 0;
	LPUINT32 dwCurrentPos = 0;

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