#include "communicate.h"

int onClose (int iCliFd) {
    /** 取消绑定 */
    int i = 0;
    
    /** 在使用前要先保证userid和socket的合法性*/
    for (i=0;i<LEN_USERLISTS;i++) {
        if (UserLists[i].state == USED && 
            UserLists[i].socket == iCliFd) {
            UserLists[i].state = UNUSED;    
            
            // 关系表中有此用户名的删除
            int j = 0; 
            for (j=0;j<LEN_BINDENCE;j++) {
                if (BindLists[j].state == USED && 
                    (UserLists[i].userid == BindLists[j].userid || 
                        UserLists[i].userid == BindLists[j].serviceid)) {
                    BindLists[j].state = UNUSED;
                }
            }
        }    
    }
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
        char* tBuff = (char *)malloc(sizeof(char)*((*pos).length ()+1));

        strcpy (tBuff, ((*pos).c_str()));
        
        cout << tBuff << endl;
        root = cJSON_Parse(tBuff);
        
        // 释放内存
        free(tBuff);

        if (root == NULL) {
            cout << "root is null" << endl;
            cJSON_Delete (root);
            return ;
        }
        cJSON *jRoute = cJSON_GetObjectItem (root, "route");
        if (jRoute == NULL) {
            cout << "route tree is not exist." << endl;
            cJSON_Delete (root);
            return ;
        }
        char *route = jRoute->valuestring;
        if (route == NULL) {
            cout << "route is null" << endl;
            cJSON_Delete (root);
            return ;
        }
        cout << route << endl;
        char tmp[] = "CController";
        char *tRoute = (char *)malloc(sizeof(char)*(strlen (route)+strlen (tmp)+1));
        sprintf (tRoute, "C%sController", route);         

        cout << "--- 路由处理器: " << tRoute << " ------ " << endl;
        TCPController*controller = (TCPController*)CKClassFactory::getInstance().UseClass(tRoute);

        // 释放内存
        free (tRoute);
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


