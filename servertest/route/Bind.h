#ifndef __BIND_CPP__
#define __BIND_CPP__
#include "TCPController.h"
/**
 * 客服抢单 (客服绑定用户(已经有客服绑定的用户不能有另一个客服绑定),客服取消用户绑定)
 */
class CBindController: public TCPController {

    public: 
        // 声明
        DECLARE_CLASS(CBindController);

    public:
        CBindController () {}
        static void* GetInstance (void) {return new CBindController();}
        string index (int socket, string jsonStr);
    
};

string CBindController::index (int socket, string jsonStr) {

    /** 绑定 */
    cout << "--- 绑定 ---" << endl;

    // 分配内存
    char* tBuff = new char[jsonStr.length()];
    strcpy(tBuff, jsonStr.c_str());
    cJSON *root = cJSON_Parse (tBuff);

    // 释放内存
    delete tBuff; 

    cJSON *j_to = cJSON_GetObjectItem (root, "to");
    if (j_to == NULL) {
        respond (socket, "Bind", "0", "jToUser is null", NULL);
        return "jToUser is null";
    }

    char* sToUser = j_to->valuestring;
    if (sToUser == NULL) {
        respond (socket, "Bind", "0", "sToUser is null", NULL);
        return "sToUser is null";
    }

    cJSON *j_action = cJSON_GetObjectItem (root, "action");
    if (j_action == NULL) {
        respond (socket, "Bind", "0", "jAction is null", NULL);
        return "jAction is null";
    }

    char* sAction = j_action->valuestring;
    if (sAction == NULL) {
        respond (socket, "Bind", "0", "sAction is null", NULL);
        return "sAction is null";
    }
    string s_action = sAction;
    string s_user = sToUser;
    string s_servicer = getUserWithSocket (socket, SERVICE);
    if (s_user == "" || s_servicer == "") {
        respond (socket, "Bind", "0", "user or service is null", NULL);
        return "user or service is null";
    } 

    if (0 == strcmp (s_action.c_str(), "bind")) {
        /** 客服绑定请求 */
        int ret = addBindence (s_servicer, s_user); 

        if (ret == FAIL) {
            respond (socket, "Bind", "0", "bind error", NULL);
            return "bind error";
        }
        respond (socket, "Bind", "0", "bind success", NULL);
        return "bind success";
    } else if (0 == strcmp (s_action.c_str(), "unbind")) {
        /** 客服取消绑定请求 */
        int ret = rmBindence (s_servicer, s_user); 

        if (ret == FAIL) {
            respond (socket, "Bind", "0", "unbind error", NULL);
            return "bind error";
        }
        // 通知客服已经成功取消绑定
        respond (socket, "Bind", "0", "unbind success", NULL);
        
        int userSocket = getSocketWithUserid (s_user, USER);

        // 通知用户, 客服已经取消绑定了
        if (userSocket != NOEXIST) {
            respond (userSocket, "Bind", "0", "unbind success", NULL);
        }
        return "bind success";
    } else {
        respond (socket, "Bind", "0", "unknown bind opt.", NULL);
        return "unknown bind opt.";
    }
}

REGISTER_CLASS (CBindController);

#endif
