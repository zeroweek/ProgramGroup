1. 服务器关闭流程
	
	* listener 停止监听
	* 服务逻辑关闭：各模块close（包括所有连接的关闭）
	* 执行完所有未执行完的事件
	* 服务器uninit
	
	* listener
	* connector
	* eventmgr
	* sockermgr
	* reactor
	* netmessagehandler

2. socker关闭流程
	
	* *** closesocket和释放socker的操作分开执行，先close
	* 被动关闭：无延迟closesocket，无延迟释放socker（但需要对io退回事件过滤拦截）
	* 主动关闭：延迟closesocket等待未发送的数据发送完，延迟释放socker防止未关闭的recv访问已被释放的socker


3. 网络各个类的关系

	* connector：负责连接
	* eventmgr：负责自定义事件管理
	* listener：负责监听
	* net：网络壳
	* reactor：负责网络事件调度
	* socker：
	* sockermgr：负责socker创建、延迟回收、数据发送

# 相关接口的错误回调什么时候调用
# 消息打印优化：详细，分类，不冗余
# 逐个类检查逻辑
# 注意一些函数的调用顺序，特别是回调函数


# 内部机制：
	错误机制
	日志机制

	系统相关尽量包裹在底层


# 工具

	日志
	定时器
	错误
	内存
	数据库
	对象池
	包裹容器： 便于内存泄露检查
	配置文件解析器： 配置文件设置（划分，经常修改以及不经常修改）



# 命名规则

//typedef struct t_PkgTreasureReq
//{
//	int  nNum;
//
//}ST_PKG_TREASURE_REQ, *PST_PKG_TRESURE_REQ;
//
//enum e_Color
//{
//	Color_Red   = 0,
//	Color_Green = 1,
//	Color_White = 2,
//};
//
//
//int g_nHelloTimeHello;
//static const int CONST_THE_NUM_OF_HELLO = 123;
//
//class HelloWorld
//{
//public:
//
//	static const int CONST_HELLO_COUNT = 100;
//
//public:
//	int GetCount() { return 0; }
//	void SetCount(int nCount) { m_nCount = nCount; }
//
//private:
//	int m_nCount;
//
//	
//};



/******************************************************************************
	反应器接口设计过程记录

	一、调用者

	1. 关心什么（需求）


	2. 使用用例例子

		ILPConnection : public ILPEventHandler
		ILPListener : public ILPEventHandler
		ILPConnector : public ILPEventHandler

	二、设计者

	1. 事件处理器，listener、Connector、Connection三者都继承自事件处理器，反应
	器只与事件处理器接口交互，无需知道前面三者的存在（listener/connector/connection）。
		
		连接器相关事件：

		    连接失败，新链接建立，停止，释放

		监听器相关事件：

		    监听失败，新链接建立，停止，释放

		链接相关事件：

		    链接建立，收到消息，错误，链接关闭

		释放/关闭的考虑：
			
			自动释放流程（调用者发起的释放）

			被动释放流程（反应器发起的释放，包括对端关闭）


		1.1. has-a关系
		1.2. is-a关系

	2. 注册事件处理器

	enum ILPEventHandler::e_EventMask
	{
		READ_MASK,
		ACCEPT_MASK,
		CONNECT_MASK,
	}
	ILPReactor::RegisterEventHandler(ILPEventHandler, ILPEventHandler::EVENT_MASK);

	3. 反应器功能（做什么？）

	    是否一定需要反应器不可？尽量不要分太多的层（不要超过三层）

	    3.1 监听外来消息
		
		    外来连接请求、收到外来消息（普通通讯消息、错误消息）、主动连上外部socket


	4. 注意释放问题

	    当某一层的对象释放掉，不应当影响其它层，即理想情况是自动将其它层的保存引用设置为null。


******************************************************************************/





/******************************************************************************
  网络组件接口设计过程记录
 
  一、组件调用者
  
    1. 关心什么（需求）

	  1.1.  尽量减少交互API的数量，降低交互机制的复杂性
	  1.2.  跨平台，所有平台操作一致，无需修改任何代码即可使用，并且不会与调用层代码冲突
	  1.3.  组件最好能直接使用，无需初始化
	  1.4.  只需提供IP和Port就可以开始连接和监听，相关选项默认最优化设置，并提供相关选项的设置接口
	  1.5.  开始connect后有反馈结果，提供撤销连接、提供设置失败重连及重连方式的机制
	  1.6.  开始listen后有反馈结果，提供撤销监听、提供设置重新监听的机制
	  1.7.  连接建立后输出一个带有标示的连接对象
	  1.8.  提供设置连接的选项的接口
	  1.9.  连接断开通知
	  1.10. 连接有消息通知
	  1.11. 连接异常通知（读、写消息都在组件内部处理，将适当的异常则抛出让调用者决定如何处理）
	  1.12. 不负责连接对象的维护工作，由组件负责释放

	2. 使用用例代码

	 




  二、组件提供者

    1. 对调用者需求处理

	  1.1. 为了保持所有组件调用的一致性，所有组件都提供Install/UnInstall接口

	2. 使用机制设计

	  2.1. 使用方式一

	      提供Listen、Connect接口，直接调用。

	  2.2. 使用方式二

	      提供Post机制，通过Post网络任务（NetTask）进行监听和连接。

	3. 消息分类（正常和异常）

	      连接建立之前的消息，连接建立之后的消息。
		  对于消息，也需要有分层的概念，将复杂多样的消息封装成简易明了的抽象消息。

	4. 反馈机制设计

	  4.1. 反馈方式一

	    通过注册回调函数实现。所有反馈都发完该回调接口。

	  4.2. 反馈方式二

	    通过注册回调对象实现。将相应的消息对应到回调对象相应的接口。


  三、key

	listener、connector都是用于创建连接对象的，最终表现有相似性

******************************************************************************/
