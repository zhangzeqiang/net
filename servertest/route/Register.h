#ifndef __REGISTER_H__
#define __REGISTER_H__
#include "TCPController.h"
/**
 * 注册用户和客服
 */
class CRegisterController: public TCPController {

    public:
        // 声明
        DECLARE_CLASS(CRegisterController);
        
        CRegisterController () {}
        static void* GetInstance (void) {return new CRegisterController();}

        string index (int socket, string jsonStr);
};

REGISTER_CLASS (CRegisterController);

string CRegisterController::index (int socket, string jsonStr) {

    /** 注册 */
    cout << "--- 注册 ---" << endl; 

    // 分配内存
    char* tBuff = new char[jsonStr.length ()];
    strcpy (tBuff, jsonStr.c_str());
    cJSON *root = cJSON_Parse (tBuff);

    // 释放内存
    delete tBuff;
     
    cJSON *jFrom = cJSON_GetObjectItem (root, "from");
    if (jFrom == NULL) {
        respond (socket, "Register", "1", "jFrom is null", NULL);
        return "jFrom is null";
    }
    char* sFrom = jFrom->valuestring;
    if (sFrom == NULL) {
        respond (socket, "Register", "1", "sFrom is null", NULL);
        return "sFrom is null";
    }
    
    cJSON *jClass = cJSON_GetObjectItem (root, "class");
    if (jClass == NULL) {
        respond (socket, "Register", "0", "jClass is null", NULL);
        return "jClass is null";
    }
    int iClass = jClass->valueint; // 用户-0,客服-1

    string fromUser = sFrom;
    /** 绑定用户id和socket */
    if (SUCCESS == bindUserAndSocket (fromUser, socket, iClass) ) {
        cout << "成功绑定" << endl;
    } else {
        cout << "绑定失败" << endl;
        respond (socket, "Register", "1", "bind fail", NULL);
        return "bind fail.";
    }
   
    respond (socket, "Register", "1", "register success", NULL);
    // 删除root
    cJSON_Delete (root);

    return "register"; 
}
#endif /** __REGISTER_H__ */

