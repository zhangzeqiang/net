#ifndef __TCPController_H__
#define __TCPController_H__

#include <string>
using namespace std;
#include "../include/ctypes.h"
#include "../bind.h"
#include "../communicate.h"
#include "../include/CKClassFactory.h"
#include "../pkg/cJSON.h"

class TCPController 
{
    public: 
        static void* GetInstance () {return NULL;}
        TCPController () {}

    public:
        virtual string index(int curSocket, string jsonStr) = 0;

    protected:
        /** 加入线程池 */
        
        /** 加入数据库连接池 */

    protected:
        /** 往socket写入数据 */
        int writeWithDefaultHeader (int socket, const char* buf) {
            char header[] = "^SHU^";    
            char *tmpBuf = new char[strlen (header)+strlen(buf)+1];
            sprintf (tmpBuf, "%s%s", header, buf); 
            int result = onReturn (socket, tmpBuf);
            delete tmpBuf;
            return result;
        } 

        /** 写JSON数据(包括默认报头) */
        int respond (int socket, char const* s_action, char const* s_errcode,
                char const* s_errmsg, cJSON* j_data) {
            cJSON *root;
            root = cJSON_CreateObject ();
            cJSON_AddItemToObject (root, "action", cJSON_CreateString (s_action));
            cJSON_AddItemToObject (root, "errcode", cJSON_CreateString (s_errcode));
            cJSON_AddItemToObject (root, "errmsg", cJSON_CreateString (s_errmsg));
            if (j_data != NULL) {
                // 如果j_data不为空
                cJSON_AddItemToObject (root, "data", j_data);
            }
            writeWithDefaultHeader (socket, cJSON_Print (root)); 
        } 

    private:
        int onReturn (int iCliFd, const char *sendBuf) {

            if (0 == write (iCliFd, sendBuf, strlen (sendBuf)) )
            {
                return FAIL;
            }
            return SUCCESS;
        }    
};

#endif  /** __TCPController_H__ */

