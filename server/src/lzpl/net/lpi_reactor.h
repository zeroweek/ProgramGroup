//****************************************************************************/
//    author: caihy
//    date: May 22, 2015
//    description: 
//
//        ��Ӧ��ILPReactor�ӿ�������
//
//****************************************************************************/
#ifndef _LPI_REACTOR_H_
#define _LPI_REACTOR_H_

#include "lpi_eventhandler.h"
#include "lp_netdef.h"



//begin�������������ֿռ�
NS_LZPL_BEGIN



//�ṹ������
struct NET_CONFIG;



// Summary��
//     ��Ӧ������ILPEventHandler���������������¼�����
class DECLARE ILPReactor
{
public:

	// Summary:
	//		��
	virtual ~ILPReactor(){}

	// Summary:
	//     ע��ILPEventHandler
	// Returns:
	//     TRUE-�ɹ���FALSE-ʧ��
	virtual BOOL LPAPI RegisterEventHandler(ILPEventHandler* pEventHandler) = 0;

	// Summary��
	//     ע��ILPEventHandler
	// Returns:
	//     TRUE-�ɹ���FALSE-ʧ��
	virtual BOOL LPAPI UnRegisterEventHandler(ILPEventHandler* pEventHandler) = 0;

public:

	static std::shared_ptr<ILPReactor> LPAPI NewReactor(NET_CONFIG& stNetConfig);
	static void LPAPI DeleteReactor(std::shared_ptr<ILPReactor>& pReactor);

	static const char * LPAPI GetIoOptTypeName(e_IoOptType eType)
	{
		switch (eType)
		{
		case LZPL::eIoOptType_None:
			LOG_CHECK_ERROR(FALSE);
			return "eIoOptType_None";
			break;
		case LZPL::eIoOptType_Recv:
			return "eIoOptType_Recv";
			break;
		case LZPL::eIoOptType_Send:
			return "eIoOptType_Send";
			break;
		case LZPL::eIoOptType_Accept:
			return "eIoOptType_Accept";
			break;
		case LZPL::eIoOptType_Connect:
			return "eIoOptType_Connect";
			break;
		case LZPL::eIoOptType_Max:
			LOG_CHECK_ERROR(FALSE);
			return "eIoOptType_Max";
			break;
		default:
			LOG_PROCESS_ERROR(FALSE);
			break;
		}

	Exit0:
		return "*** unknow e_IoOptType ***";
	}
};



//end�������������ֿռ�
NS_LZPL_END

#endif