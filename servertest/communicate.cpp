#include "communicate.h"

int onClose (int iCliFd) {
    /** 取消绑定 */
    unbindUserAndSocket (iCliFd);
}

/**
 * 解析客户端发送过来的数据,以^为分割符,#为段内分割
 * fromuserid#123^touserid#124^msg#Hello World
 * key#value^key1#value1^key2#value2
 */
void onResolve (int iCliFd, char *buf) 
{
    cout<< "接收到客户端数据:" << buf << endl;

    Smap myMap;

    char *token = strtok (buf, "^");
    
    while (token!=NULL)
    {
        /** 分割成(key#value) */    
        // cout << "token:" << token << endl; 
        
        char *pKey = (char *) malloc (strlen (token) * sizeof (char));
        char *pValue = (char *) malloc (strlen (token) * sizeof (char));

        int i = 0;
        int index = 0;
        for (i=0;token[i] != '\0';i++) {

            if (token[i] != '#') { 
                pKey[i] = token[i];
            } else {
                index = i;
                pKey[i] = 0;
                break;
            }     

        } 

        for (i=index+1;token[i] != '\0';i++) {
            pValue[i-index-1] = token[i];
        }
        pValue[i-index-1] = 0;

        // cout << "key:" << pKey << endl;
        // cout << "value:" << pValue << endl;

        std::string sKey = pKey;
        std::string sValue = pValue;
        myMap[sKey] = sValue;
        free (pKey);
        free (pValue);
        token = strtok (NULL, "^");
    }

    /** 处理网络字段 */
    onHandle (iCliFd, myMap);

}

/**
 * 处理客户端发送过来的请求
 * 注册:
 * action#register^from#111
 * 发送:
 * action#send^to#123
 *
 */
void onHandle (int iCliFd, Smap myMap) {
    
    // Smap::iterator pos;

    // for (pos=myMap.begin();pos!=myMap.end();++pos) {
    //     cout << pos->first  << ":" << pos->second << endl;
    // }
    
    Smap::iterator pos = myMap.find ("action"); 

    if (pos == myMap.end ()) {
        return ;
    }
    if (pos->second == "register") {
        /** 注册 */
        cout << "--- 注册 ---" << endl; 

        Smap::iterator fromMap = myMap.find ("from");

        if (fromMap == myMap.end ()) {
            return ;
        }
        cout << "id:" << fromMap->second << endl;
        cout << "socket:" << iCliFd << endl;
        
        /** 绑定用户id和socket */
        if (SUCCESS == bindUserAndSocket (fromMap->second, iCliFd) ) {
            cout << "成功绑定" << endl;
        } else {
            cout << "绑定失败" << endl;
        }
       
        char retBuf[] = "register success."; 
        onReturn (iCliFd, retBuf);
    } else if (pos->second == "send") {
        /** 发送 */
        cout << "--- 消息转发---" << endl; 

        Smap::iterator toMap = myMap.find ("to");
        if (toMap == myMap.end ()) {
            char retBuf[] = "lack of touserid";
            onReturn (iCliFd, retBuf);
            return ;
        }
        Smap::iterator msgMap = myMap.find ("msg");
        if (msgMap == myMap.end ()) {
            char retBuf[] = "lack of msg";
            onReturn (iCliFd, retBuf);
            return ;
        }

        int toSocket = getSocketWithUserid (toMap->second);
         
        char *sTmp = const_cast<char *> (msgMap->second.c_str());
        if (getStateWithUserid (toMap->second) == USED && onReturn (toSocket, sTmp) == SUCCESS) {
            // 转发成功
            char retBuf[] = "send success.";
            onReturn (iCliFd, retBuf);  
        } else {
            // 转发失败
            char retBuf[] = "send fail.";
            onReturn (iCliFd, retBuf);  
        }
         
        cout << "to:" << toMap->second << ",msg:" << msgMap->second << endl;
    } else if (pos->second == "list") {
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
                
        cout << tmp << endl;
        char retBuf[] = "list success."; 
        onReturn (iCliFd, tmp);
    }
}

int onReturn (int iCliFd, char *sendBuf) {

    if (0 == write (iCliFd, sendBuf, strlen (sendBuf)) )
    {
        return FAIL;
    }
    return SUCCESS;
}

