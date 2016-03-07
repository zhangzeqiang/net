#ifndef __SEND_CPP__
#define __SEND_CPP__
#include "TCPController.h"
/**
 * 发送数据(事先绑定用户和客服,客服和用户之间才能相互直接沟通)
 */
class CSendController: public TCPController {

    public: 
        // 声明
        DECLARE_CLASS(CSendController);

    public:
        CSendController () {}
        static void* GetInstance (void) {return new CSendController();}
        string index (int socket, string jsonStr);
        cJSON* newJData (const char* sMsg, const char* sTo, const char* serverid);   
};

cJSON* CSendController::newJData (const char* sMsg, const char* sTo, const char* serverid) {
    cJSON* j_data = cJSON_CreateObject ();
    cJSON_AddItemToObject (j_data, "msg", cJSON_CreateString (sMsg));
    cJSON_AddItemToObject (j_data, "userid", cJSON_CreateString (sTo));
    cJSON_AddItemToObject (j_data, "serverid", cJSON_CreateString (serverid));
    return j_data;
}

string CSendController::index (int socket, string jsonStr) {
    /** 发送 */
    cout << "--- 消息转发---" << endl; 

    cout << jsonStr << endl;
    // 分配内存
    const char* tBuff = jsonStr.c_str();
    cJSON *root = cJSON_Parse (tBuff);
    if (root == NULL) {
        respond (socket, "Send", "-6", "root is null", NULL);
        cJSON_Delete (root);
        return "root is null";
    }
    cJSON *jClass = cJSON_GetObjectItem (root, "class");
    if (jClass == NULL) {
        respond (socket, "Send", "-3", "jClass is null", NULL);

        cJSON_Delete (root);
        return "jClass is null";
    }
    int iClass = jClass->valueint;

    cJSON *jMsg = cJSON_GetObjectItem (root, "msg");
    if (jMsg == NULL) {
        respond (socket, "Send", "-1", "jMsg is null", NULL);
        cJSON_Delete (root);

        return "jMsg is null";
    }

    char* sMsg = jMsg->valuestring;
    if (sMsg == NULL) {
        respond (socket, "Send", "-2", "sMsg is null", NULL);
        cJSON_Delete (root);

        return "sMsg is null";
    }

    int distSocket;

    if (iClass == SERVICE) {
        // 客服发送消息,需要指定接收用户id且已经绑定过此客服的用户socket
        cout << "service." << endl;
        cJSON *jTo = cJSON_GetObjectItem (root, "toid");
        if (jTo == NULL) {
            respond (socket, "Send", "-4", "jTo is null", NULL);
            cJSON_Delete (root);
            return "jTo is null";
        }
        char* sTo = jTo->valuestring;
        if (sTo == NULL) {
            respond (socket, "Send", "-5", "sTo is null", NULL);
            cJSON_Delete (root);
            return "sTo is null";
        }

        string sToUser = sTo;
        string sFromService = getUserWithSocket (socket, SERVICE);
        
        cout << "service:" << sFromService << endl;
        if (0 == strcmp (sFromService.c_str(), "")) {
            // 不存在此客服
            respond (socket, "Send", "0", "Send sFromService is noexist.", NULL); 
            cJSON_Delete (root);
            return "Send sFromService is noexist.";
        }

        distSocket = getUserSocketWithBindence (sFromService, sTo); 

        if (distSocket == NOEXIST) {
            cJSON* j_data = newJData (sMsg, sTo, sFromService.c_str());
            respond (socket, "Send", "0", "no bind user.", j_data); 
        } else {
            cJSON* j_data = newJData (sMsg, sTo, sFromService.c_str());
            respond (socket, "Send", "1", "Send success.", j_data); 

            j_data = newJData (sMsg, sTo, sFromService.c_str());
            respond (distSocket, "Send", "2", "Send success.", j_data); 
        }
    } else {
        cout << "user." << endl;
        
        // 用户发送消息,不需要指定接受用户,目的socket在绑定表内查找
        string sFromUser = getUserWithSocket (socket, USER);

        if (sFromUser == "") {
            // 不存在此绑定
            respond (socket, "Send", "0", "Send sFromUser is noexist.", NULL); 
            cJSON_Delete (root);
            return "Send sFromUser is noexist";
        }
        cout << sFromUser << endl;
 
        distSocket = getServiceSocketWithBindence (sFromUser);

        if (distSocket == NOEXIST) {
            // 还未有客服服务,通知客服抢单
            /** 获取所有在线客服socket,发送新用户抢单提醒 */
            int i=0;
            string s_tmp_server;
            for (i=0;i<LEN_USERLISTS;i++) {
                if (UserLists[i].state == USED && 
                    UserLists[i].classify == SERVICE) {
                    // 给所有在线的客服发送提醒
                    int tmp_socket = UserLists[i].socket;
                   
                    s_tmp_server = getUserWithSocket (tmp_socket, SERVICE);
                    
                    if (0 != strcmp (s_tmp_server.c_str(), "")) {
                        cJSON* j_data = newJData (sMsg, sFromUser.c_str(), s_tmp_server.c_str());
                        respond (tmp_socket, "Acknowledge", "1", "new user.", j_data);
                    }

                } 
            }

            if (0 != strcmp (s_tmp_server.c_str(), "")) {
                cJSON* j_data = newJData (sMsg, sFromUser.c_str(), s_tmp_server.c_str());
                respond (socket, "Acknowledge", "1", "already call for server.", j_data);
            } else {
                cJSON* j_data = newJData (sMsg, sFromUser.c_str(), s_tmp_server.c_str());
                respond (socket, "Acknowledge", "2", "no server online.", j_data);
            }
            cJSON_Delete (root);
            return "already call for server.";
        } else {
            cJSON* j_data = newJData (sMsg, sFromUser.c_str(), "null");
            respond (socket, "Send", "1", "Send success.", j_data); 
        }
        
        string serverid_tmp = getServeridWithBindence (sFromUser);
        cJSON* j_data = newJData (sMsg, sFromUser.c_str(), serverid_tmp.c_str());

        respond (distSocket, "Send", "2", "Send success.", j_data); 
    }
    cJSON_Delete (root);

    return "send msg."; 
}

REGISTER_CLASS (CSendController);

#endif

