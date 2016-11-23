//****************************************************************************/
//    author: caihy
//    date: September 23, 2014
//    description: 
//
//****************************************************************************/
#ifndef _LP_SIGNLETON_H_
#define _LP_SIGNLETON_H_

#include "lp_def.h"



//begin�������������ֿռ�
NS_LZPL_BEGIN



//****************************************************************************/
//	��һ�ֵ���������ʽ
//  ʹ�÷���
//  1. ����Ĺ������������������˽�����ͣ���ֹ�����ط����Դ��������ٶ���
//  2. ���������м���SINGLETON_DECLARE(cls)������cls���������
//	3. ����ʵ�ֵ�CPP�ļ��м���SINGLETON_IMPLEMENT(cls)
//
//  ���磺
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
//	�ڶ��ֵ���������ʽ
//	1. ��������������public��ʽ�̳е���ģ���࣬��������Ĺ��캯������������
//		���붨��Ϊprivate���͡�
//	2. �̳е���ģ�����֮�����Ҫ����������У�����
//		SINGLETON_FRIEND_CLASS_REGISTER(T)��ע��Ϊ��Ԫ��
//	3. ����ģ����Ϊ���̰߳�ȫ������double-checked Locking��ʽ���ɵ�������
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
//		//�˴����Զ���������ֹ�ͷ�֮����������´���
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





//end�������������ֿռ�
NS_LZPL_END

#endif