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
    
};

string CSendController::index (int socket, string jsonStr) {
    /** 发送 */
    cout << "--- 消息转发---" << endl; 

    cout << jsonStr << endl;
    // 分配内存
    char* tBuff = new char[jsonStr.length ()];
    strcpy (tBuff, jsonStr.c_str());
    cJSON *root = cJSON_Parse (tBuff);

    // 释放内存
    delete tBuff;

    cJSON *jClass = cJSON_GetObjectItem (root, "class");
    if (jClass == NULL) {
        respond (socket, "Send", "0", "jClass is null", NULL);
        return "jClass is null";
    }
    int iClass = jClass->valueint;

    cJSON *jMsg = cJSON_GetObjectItem (root, "msg");
    if (jMsg == NULL) {
        respond (socket, "Send", "0", "jMsg is null", NULL);
        return "jMsg is null";
    }
    char* sMsg = jMsg->valuestring;
    if (sMsg == NULL) {
        respond (socket, "Send", "0", "sMsg is null", NULL);
        return "sMsg is null";
    }

    int distSocket;

    cJSON* j_data = cJSON_CreateObject ();
    cJSON_AddItemToObject (j_data, "msg", cJSON_CreateString (sMsg));

    if (iClass == SERVICE) {
        cout << "service." << endl;
        // 客服发送消息,需要指定接收用户id且已经绑定过此客服的用户socket
        cJSON *jTo = cJSON_GetObjectItem (root, "to");
        if (jTo == NULL) {
            respond (socket, "Send", "0", "jTo is null", NULL);
            return "jTo is null";
        }
        char* sTo = jTo->valuestring;
        if (sTo == NULL) {
            respond (socket, "Send", "0", "sTo is null", NULL);
            return "sTo is null";
        }
        
        string sToUser = sTo;
        string sFromService = getUserWithSocket (socket, SERVICE);
        
        cout << "service:" << sFromService << endl;
        if (0 == strcmp (sFromService.c_str(), "")) {
            // 不存在此客服
            respond (socket, "Send", "0", "Send sFromService is noexist.", NULL); 
            return "Send sFromService is noexist.";
        }

        // 需要加入目的用户id
        cJSON_AddItemToObject (j_data, "to", cJSON_CreateString (sTo));

        distSocket = getUserSocketWithBindence (sFromService, sTo); 
    } else {
        cout << "user." << endl;
        // 用户发送消息,不需要指定接受用户,目的socket在绑定表内查找

        string sFromUser = getUserWithSocket (socket, USER);

        if (sFromUser == "") {
            // 不存在此绑定
            respond (socket, "Send", "0", "Send sFromUser is noexist.", NULL); 
            return "Send sFromUser is noexist";
        }
        distSocket = getServiceSocketWithBindence (sFromUser);
    }
    
    respond (distSocket, "Send", "1", "Send success.", j_data); 
    // 删除root
    cJSON_Delete (root);

    return "send msg."; 
}

REGISTER_CLASS (CSendController);

#endif

