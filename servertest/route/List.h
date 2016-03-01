#ifndef __LIST_H__
#define __LIST_H__
#include "TCPController.h"

class CListController: public TCPController {

    public: 
        // 声明
        DECLARE_CLASS(CListController);

    public:
        CListController () {}
        static void* GetInstance (void) {return new CListController();}
        string index (int socket, string jsonStr);
    
};

string CListController::index (int socket, string jsonStr) {

    /** 列举当前连接的客户机信息 */
    cout << "--- 列举客户机信息 ---" << endl;

    int i = 0;
    char tmp[1024];
    sprintf (tmp, "%s", ""); 
    for (i=0;i<LEN_USERLISTS;i++) {
        if (UserLists[i].state == USED) {
            
            // 将string转为char *
            char *sTmp = const_cast<char *> (UserLists[i].userid.c_str());

            if (0 == strcmp (tmp, "")) {
                sprintf (tmp, "(%s,%d)", sTmp, UserLists[i].socket);
            } else {
                sprintf (tmp, "%s,(%s,%d)", tmp, sTmp, UserLists[i].socket);
            } 

        }
    } 
            
    cout << "待发送:" << tmp << endl;
    char retBuf[] = "list success."; 

    respond (socket, "List", "1", tmp, NULL);
    return "List"; 
}

REGISTER_CLASS (CListController);

#endif /** __LIST_H__ */

