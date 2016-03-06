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
    const char* tBuff = jsonStr.c_str();

    cJSON *root = cJSON_Parse (tBuff);

    if (root == NULL) {
        respond (socket, "Register", "-6", "root is null", NULL);
        cJSON_Delete (root);
        return "root is null";
    }
    cJSON *jFrom = cJSON_GetObjectItem (root, "id");
    if (jFrom == NULL) {
        respond (socket, "Register", "-1", "jFrom is null", NULL);
        // 删除root
        cJSON_Delete (root);
        return "lack of id";
    }
    char* sFrom = jFrom->valuestring;
    if (sFrom == NULL) {
        respond (socket, "Register", "-2", "sFrom is null", NULL);
        // 删除root
        cJSON_Delete (root);
        return "id is empty";
    }
    
    cJSON *jClass = cJSON_GetObjectItem (root, "class");
    if (jClass == NULL) {
        respond (socket, "Register", "-3", "jClass is null", NULL);
        // 删除root
        cJSON_Delete (root);
        return "lack of class";
    }
    int iClass = jClass->valueint; // 用户-0,客服-1

    string fromUser = sFrom;
    cJSON *j_data = cJSON_CreateObject ();
    cJSON_AddItemToObject (j_data, "id", cJSON_CreateString (sFrom));
    cJSON_AddItemToObject (j_data, "class", cJSON_CreateNumber (iClass));

    /** 绑定用户id和socket */
    if (SUCCESS == bindUserAndSocket (fromUser, socket, iClass) ) {
        cout << "成功绑定" << endl;
    } else {
        cout << "绑定失败" << endl;
        respond (socket, "Register", "0", "bind fail", j_data);
        // 删除root
        cJSON_Delete (root);
        return "bind fail.";
    }
   
    respond (socket, "Register", "1", "register success", j_data);

    // 删除root
    cJSON_Delete (root);
    return "register"; 
}
#endif /** __REGISTER_H__ */

