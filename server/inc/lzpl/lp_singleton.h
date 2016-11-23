//****************************************************************************/
//    author: caihy
//    date: September 23, 2014
//    description: 
//
//****************************************************************************/
#ifndef _LP_SIGNLETON_H_
#define _LP_SIGNLETON_H_

#include "lp_def.h"



//begin声明所处的名字空间
NS_LZPL_BEGIN



//****************************************************************************/
//	第一种单例申明方式
//  使用方法
//  1. 将类的构造和析构函数申明成私有类型，防止其它地方可以创建和销毁对象
//  2. 在申明类中加入SINGLETON_DECLARE(cls)，其中cls是类的名字
//	3. 在类实现的CPP文件中加入SINGLETON_IMPLEMENT(cls)
//
//  例如：
//
//  //Sample.h
//  class Sample
//  {
//  Sample();
//  ~Sample();
//
//  SINGLETON_DECLARE(Sample)
//  };
//
//  //Sample.cpp
//  SINGLETON_IMPLEMENT(Sample)
//
//****************************************************************************/
#define SINGLETON_DECLARE(cls) \
	public:\
		static cls& LPAPI Instance(){ return m_oInstance; }\
	private:\
		static cls m_oInstance;

#define SINGLETON_IMPLEMENT(cls)\
	cls cls::m_oInstance;



//****************************************************************************/
//	第二种单例申明方式
//	1. 单例类对象必须以public方式继承单例模板类，并且自身的构造函数和析构函数
//		必须定义为private类型。
//	2. 继承单例模板类的之类必须要在类的申明中，调用
//		SINGLETON_FRIEND_CLASS_REGISTER(T)，注册为友元。
//	3. 单例模板类为多线程安全。采用double-checked Locking方式生成单例对象。
//		Singleton* Singleton::getInstance() {
//		Singleton* tmp = m_instance;
//		...
//			// insert memory barrier
//			if (tmp == NULL) {
//				Lock lock;
//				tmp = m_instance;
//				if (tmp == NULL) {
//					tmp = new Singleton;
//					...
//						// insert memory barrier
//						m_instance = tmp;
//				}
//			}
//			return tmp;
//		}
//****************************************************************************/
//#define SINGLETON_FRIEND_CLASS_REGISTER(T)    friend class Singleton<T>
//#define NS_SINGLETON_FRIEND_CLASS_REGISTER(NS,T)    friend class NS::Singleton<T>
//
//template<class T>
//class Singleton
//{
//public:
//
//	static T* LPAPI CreateInstance()
//	{
//		if (NULL == m_poInstance)
//		{
//			m_poInstance = new T;
//		}
//
//		return m_poInstance;
//	}
//
//	static T* LPAPI Instance()
//	{
//		//此处不自动创建，防止释放之后调用又重新创建
//		//if (NULL == m_poInstance)
//		//{
//		//	m_poInstance = new T;
//		//}
//
//		return m_poInstance;
//	}
//
//	static void LPAPI DestoryInstance()
//	{
//		if (NULL != m_poInstance)
//		{
//			delete m_poInstance;
//			m_poInstance = NULL;
//		}
//	}
//
//protected:
//
//	Singleton(){}
//	virtual ~Singleton(){}
//
//private:
//
//	static T*  m_poInstance;
//};
//
//template<class T>
//T* Singleton<T>::m_poInstance = NULL;





//end声明所处的名字空间
NS_LZPL_END

#endif