#include "communicate.h"

int onClose (int iCliFd) {
    /** 取消绑定 */
    unbindUserAndSocket (iCliFd);
}

void onResolve (int iCliFd, char *buf) 
{
    cout<< "------ 完整客户数据包 ------" << endl << buf << endl;

    /** 路由处理 */
    string str = buf;
    char header[] = "^SHD^";
    list<string> arr_list = Utils::StringOpt::split (str, header);
    list<string>::iterator pos = arr_list.begin ();
    cJSON *root;
       
    while (pos != arr_list.end ()) {
        cout << endl << "分段:" << *pos << endl;

        // 分配内存
        char* tBuff = new char[(*pos).length ()];
        strcpy (tBuff, ((*pos).c_str()));
        root = cJSON_Parse(tBuff);

        // 释放内存
        delete tBuff;

        if (root == NULL) {
            cout << "root is null" << endl;
            return ;
        }
        cJSON *jRoute = cJSON_GetObjectItem (root, "route");
        if (jRoute == NULL) {
            cout << "route tree is not exist." << endl;
            return ;
        }
        char *route = jRoute->valuestring;
        if (route == NULL) {
            cout << "route is null" << endl;
            return ;
        }
        // cout << route << endl;
        char tmp[] = "CController";
        char *tRoute = new char[strlen (route)+strlen (tmp)+1];
        sprintf (tRoute, "C%sController", route);         

        cout << "--- 路由处理器: " << tRoute << " ------ " << endl;
        TCPController*controller = (TCPController*)CKClassFactory::getInstance().UseClass(tRoute);
        if (controller == NULL) {
            // 找不到路由地址
            controller = (TCPController *)CKClassFactory::getInstance ().UseClass ("CNofoundController");
        }

        if (controller != NULL) {
            cout << controller->index(iCliFd, *pos) << endl;
        }

        delete controller;
        
        cout << "------ 路由处理器完成处理 ------" << endl << endl;        
        *pos++;
    }

    // 释放JSON对象
    cJSON_Delete (root);

}


