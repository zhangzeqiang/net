#ifndef __ACKNOWLEDGE_CPP__
#define __ACKNOWLEDGE_CPP__
#include "TCPController.h"
/**
 * 新用户抢单提示 (通知所有在线客服)
 */
class CAcknowledgeController: public TCPController {

    public: 
        // 声明
        DECLARE_CLASS(CAcknowledgeController);

    public:
        CAcknowledgeController () {}
        static void* GetInstance (void) {return new CAcknowledgeController();}
        string index (int socket, string jsonStr);
    
};

string CAcknowledgeController::index (int socket, string jsonStr) {

    /** 获取所有在线客服socket,发送新用户抢单提醒 */
    int i=0;
    for (i=0;i<LEN_USERLISTS;i++) {
        if (UserLists[i].state == USED && 
            UserLists[i].classify == SERVICE) {
            // 给所有在线的客服发送提醒
            int tmp_socket = UserLists[i].socket;

            string s_fromid = getUserWithSocket (socket, USER); 
            const char* sfromid = s_fromid.c_str();

            cJSON *j_data = cJSON_CreateObject ();
            cJSON_AddItemToObject (j_data, "fromid", cJSON_CreateString (sfromid));

            respond (tmp_socket, "Acknowledge", "1", "new user.", j_data);
        } 
    }
    
    return "Acknowledge"; 
}

REGISTER_CLASS (CAcknowledgeController);

#endif

