//****************************************************************************/
//    author: caihy
//    date: Janurary 4, 2016
//    description:
//
//****************************************************************************/
#ifndef _LP_DB_INIT_H_
#define _LP_DB_INIT_H_

#include "lp_base.h"



typedef struct st_mysql MYSQL;


//begin�������������ֿռ�
NS_LZPL_BEGIN



DECLARE BOOL LPAPI lpDBExist(MYSQL* pMysql, const char* pcszDBName);
DECLARE BOOL LPAPI lpDBCreate(MYSQL* pMysql, const char* pcszDBName);
DECLARE BOOL LPAPI lpDBUpdate(MYSQL* pMysql, const char* pcszDBName, const char* pcszDBPrefix, LPINT32 nNewDBVersion);



//end�������������ֿռ�
NS_LZPL_END

#endif