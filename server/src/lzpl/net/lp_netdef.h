//****************************************************************************/
//    author: caihy
//    date: Jan 6, 2017
//    description:
//
//****************************************************************************/
#ifndef _LP_NET_DEF_H_
#define _LP_NET_DEF_H_

#include "lp_base.h"
#include "boost/asio.hpp"
#include "boost/bind.hpp"
#include "boost/asio/buffer.hpp"
#include "boost/thread.hpp"


using namespace boost::asio;


#ifndef _WIN32

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <netinet/tcp.h>

#endif



//begin声明所处的名字空间
NS_LZPL_BEGIN



//end声明所处的名字空间
NS_LZPL_END

#endif