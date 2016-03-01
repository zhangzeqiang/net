#ifndef __SEND_CPP__
#define __SEND_CPP__
#include "TCPController.h"

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
     
    cJSON *jTo = cJSON_GetObjectItem (root, "to");
    if (jTo == NULL) {
        cout << "jTo is null" << endl;
        return "jTo is null";
    }
    cJSON *jMsg = cJSON_GetObjectItem (root, "msg");
    if (jMsg == NULL) {
        cout << "jMsg is null" << endl;
        return "jMsg is null";
    }
    char* sTo = jTo->valuestring;
    char* sMsg = jMsg->valuestring;

    string sToUser = sTo;
    int toSocket = getSocketWithUserid (sToUser);
     
    if (getStateWithUserid (sToUser) == USED && writeWithDefaultHeader (toSocket, sMsg) == SUCCESS) {
        // 转发成功
        char retBuf[] = "send success.";
        writeWithDefaultHeader(socket, retBuf);  
    } else {
        // 转发失败
        char retBuf[] = "send fail.";
        writeWithDefaultHeader(socket, retBuf);  
    }
     
    cout << "to:" << sTo << ",msg:" << sMsg << endl;

    // 删除root
    cJSON_Delete (root);

    return "send msg."; 
}

REGISTER_CLASS (CSendController);

#endif

