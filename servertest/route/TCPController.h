#ifndef __TCPController_H__
#define __TCPController_H__
/**
 * CKClassFactory,CKDynamic是C++反射注册类;TCPController则定义了tcp控制器的接口
 */
#include <iostream>
#include <string.h>
#include <map>
using namespace std;

typedef void* (*__createClass) (void);

class CKClassFactory {

    private:
        map<string, __createClass> m_classMap;

        CKClassFactory () {}

    public:
        virtual ~CKClassFactory () {
        }

    public:
        static CKClassFactory& getInstance ();
        void RegisterClass (string className, __createClass method); 
        void* UseClass (string className);
};

CKClassFactory& CKClassFactory::getInstance () {

    static CKClassFactory __factory;
    return __factory;
}

/**
 * 注册类
 */
void CKClassFactory::RegisterClass (string className, __createClass method) {
    m_classMap.insert (pair<string, __createClass> (className, method)); 
}

/**
 * 获取类实例
 */
void* CKClassFactory::UseClass (string className) {
    map<string, __createClass>::const_iterator iter;

    iter = m_classMap.find (className);
    if (iter == m_classMap.end ()) {
        return NULL;
    } else {
        return iter->second();
    }
}

class CKDynamic {
    public:
        CKDynamic (string name, __createClass method) {
            CKClassFactory::getInstance ().RegisterClass (name, method);
        }
};

#define GETCONTROLLER(className) \
    C##className##Controller

#define DECLARE_CLASS(className) \
    static CKDynamic* _m;

#define REGISTER_CLASS(className) \
    CKDynamic* className::_m = \
    new CKDynamic(#className, (__createClass)className::GetInstance);

class TCPController 
{
    public: 
        static void* GetInstance () {return NULL;}
        TCPController () {}

    public:
        virtual string index() = 0;

};

string TCPController::index () {}

#endif  /** __TCPController_H__ */

