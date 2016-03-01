#include "CKClassFactory.h"

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

