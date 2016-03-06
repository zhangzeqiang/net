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
    const char* tBuff = jsonStr.c_str();
    cJSON *root = cJSON_Parse (tBuff);
    if (root == NULL) {
        respond (socket, "Bind", "-6", "root is null", NULL);
        cJSON_Delete (root);
        return "root is null";
    }

    cJSON *j_to = cJSON_GetObjectItem (root, "id");
    if (j_to == NULL) {
        respond (socket, "Bind", "-6", "jToUser is null", NULL);
        cJSON_Delete (root);
        return "jToUser is null";
    }

    char* sToUser = j_to->valuestring;
    if (sToUser == NULL) {
        respond (socket, "Bind", "-2", "sToUser is null", NULL);
        cJSON_Delete (root);

        return "sToUser is null";
    }
    
    cJSON *j_action = cJSON_GetObjectItem (root, "action");
    if (j_action == NULL) {
        respond (socket, "Bind", "-3", "jAction is null", NULL);
        cJSON_Delete (root);

        return "jAction is null";
    }

    char* sAction = j_action->valuestring;
    if (sAction == NULL) {
        respond (socket, "Bind", "-4", "sAction is null", NULL);
        cJSON_Delete (root);
        return "sAction is null";
    }
    
    string s_action = sAction;
    string s_user = sToUser;
    string s_servicer = getUserWithSocket (socket, SERVICE);
    if (s_user == "" || s_servicer == "") {
        respond (socket, "Bind", "-5", "user or service is null", NULL);
        cJSON_Delete (root);
        return "user or service is null";
    } 

    // 新的data json 节点
    cJSON* j_data = cJSON_CreateObject ();
    cJSON_AddItemToObject (j_data, "serverid", cJSON_CreateString (s_servicer.c_str()));
    cJSON_AddItemToObject (j_data, "", cJSON_CreateString (sAction));
    cJSON_AddItemToObject (j_data, "userid", cJSON_CreateString (sToUser));

    if (0 == strcmp (s_action.c_str(), "bind")) {


        /** 客服绑定请求 */
        cout << "bind"  << endl;
        int ret = addBindence (s_servicer, s_user); 
                
        if (ret == FAIL) {
            respond (socket, "Bind", "0", "bind error", j_data);
            cJSON_Delete (root);
            return "bind error";
        }

        cout << "1" << endl;
        // 通知用户,有客服来服务了
        int userSocket = getSocketWithUserid (s_user, USER);
        cout << "2" << endl;

        if (userSocket != NOEXIST) {
            respond (userSocket, "Bind", "1", "bind success", j_data);
        } else {
            respond (userSocket, "Bind", "-7", "bind fail", j_data);
        }
        cout << "3" << endl;

        cJSON* j_data = cJSON_CreateObject ();
        cJSON_AddItemToObject (j_data, "serverid", cJSON_CreateString (s_servicer.c_str()));
        cJSON_AddItemToObject (j_data, "action", cJSON_CreateString (sAction));
        cJSON_AddItemToObject (j_data, "userid", cJSON_CreateString (sToUser));

        respond (socket, "Bind", "1", "bind success", j_data);
        cout << "4" << endl;

        cJSON_Delete (root);
        return "bind success";
    } else if (0 == strcmp (s_action.c_str(), "unbind")) {
        /** 客服取消绑定请求 */
        cout << "unbind"  << endl;
        cJSON *jClass = cJSON_GetObjectItem (root, "class");
        if (jClass == NULL) {
            respond (socket, "class", "-3", "jClass is null", NULL);
            cJSON_Delete (root);
            return "jClass is null";
        }
        int iClass = jClass->valueint;

        
        // 加入class属性
        cJSON_AddItemToObject (j_data, "class", cJSON_CreateNumber (iClass));

        if (iClass == SERVICE) {
            // 取消本客服对应的服务用户绑定
            int j=0;
            for (j=0;j<LEN_BINDENCE;j++) {
                if (BindLists[j].state == USED && 
                        BindLists[j].serviceid == s_servicer) {
                    // 获取所有客服绑定下的用户id
                    int u_socket = getSocketWithUserid (BindLists[j].userid, USER);
                    // 取消绑定
                    BindLists[j].state = UNUSED;
                    if (u_socket != NOEXIST) {
                        // 用户在线,则给用户发送客服挂单通知
                        respond (u_socket, "Bind", "2", "unbind success", j_data);
                    }
                    cJSON* j_data = cJSON_CreateObject ();
                    cJSON_AddItemToObject (j_data, "serverid", cJSON_CreateString (s_servicer.c_str()));
                    cJSON_AddItemToObject (j_data, "action", cJSON_CreateString (sAction));
                    cJSON_AddItemToObject (j_data, "userid", cJSON_CreateString (sToUser));
                }
            }      
            respond (socket, "Bind", "2", "unbind success", j_data);
        } else {
            // 取消单个用户服务
            int ret = rmBindence (s_servicer, s_user); 

            if (ret == FAIL) {
                respond (socket, "Bind", "-1", "unbind error", j_data);
                cJSON_Delete (root);
                return "bind error";
            }
            // 通知客服已经成功取消绑定
            respond (socket, "Bind", "2", "unbind success", j_data);
            
            int userSocket = getSocketWithUserid (s_user, USER);

            // 通知用户, 客服已经取消绑定了
            if (userSocket != NOEXIST) {
                cJSON* j_data = cJSON_CreateObject ();
                cJSON_AddItemToObject (j_data, "serverid", cJSON_CreateString (s_servicer.c_str()));
                cJSON_AddItemToObject (j_data, "action", cJSON_CreateString (sAction));
                cJSON_AddItemToObject (j_data, "userid", cJSON_CreateString (sToUser));

                respond (userSocket, "Bind", "2", "unbind success", j_data);
            }
        }
        cJSON_Delete (root);
        return "bind success";
    } else {
        respond (socket, "Bind", "0", "unknown bind opt.", NULL);
        cJSON_Delete (root);
        return "unknown bind opt.";
    }
}

REGISTER_CLASS (CBindController);

#endif
