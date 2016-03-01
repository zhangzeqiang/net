#ifndef __CKCLASSFACTORY_H__
#define __CKCLASSFACTORY_H__

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

#endif /** __CKCLASSFACTORY_H__ */

