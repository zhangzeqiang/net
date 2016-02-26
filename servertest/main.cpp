#include <iostream>
#include <event.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>

#include "communicate.h"
#include "incset.route"
#include "testinc/fileopt.h"

using namespace std;

// 事件
struct event_base* base;

// 配置
#define PORT 8888
#define SERVERADDR "127.0.0.1"

void onRead(int iCliFd, short iEvent, void *arg); 
void onAccept(int iSvrFd, short iEvent, void *arg); 
void run ();
void onTest ();

void onRead(int iCliFd, short iEvent, void *arg) 
{ 
    int iLen; 
    char buf[1500]; 
   
    // cout<< "等待客户端数据... " << endl;

    iLen = read (iCliFd, buf, sizeof (buf)); 
   
    if (iLen <= 0) { 
        cout << "Client Close" << endl; 

        // 连接结束=0 | 连接错误<0, 将事件池删除并释放内存空间
        struct event *pEvRead = (struct event*)arg; 
        event_del(pEvRead); 
        delete pEvRead; 
                      
        close(iCliFd); 

        onClose (iCliFd);
        return;
    }

    buf[iLen] = 0;
    onResolve (iCliFd, buf);
}

void onAccept(int iSvrFd, short iEvent, void *arg) 
{ 
    int iCliFd; 
    struct sockaddr_in sCliAddr; 
    cout<< "客户端新连接... " << endl;
   
    socklen_t iSinSize = sizeof(sCliAddr); 
    iCliFd = accept(iSvrFd, (struct sockaddr*)&sCliAddr, &iSinSize); 
    
    struct event *pEvRead = new event; 
    event_set(pEvRead, iCliFd, EV_READ|EV_PERSIST, onRead, pEvRead); 
    event_base_set(base, pEvRead); 
    event_add(pEvRead, NULL);
}

void run () {
    int iSvrFd;   
    struct sockaddr_in sSvrAddr; 
             
    memset(&sSvrAddr, 0, sizeof(sSvrAddr));   
    sSvrAddr.sin_family = AF_INET;   
    sSvrAddr.sin_addr.s_addr = inet_addr(SERVERADDR);     
    sSvrAddr.sin_port = htons(PORT);
    iSvrFd = socket(AF_INET, SOCK_STREAM, 0);   
    bind(iSvrFd, (struct sockaddr*)&sSvrAddr, sizeof(sSvrAddr));   
    listen(iSvrFd, 10);
    base = event_base_new ();

    /** 初始化UserLists列表 */
    emptyUserLists();

    cout<< "服务器成功启动" << endl;
    struct event evListen;

    // 设置事件 
    event_set(&evListen, iSvrFd, EV_READ|EV_PERSIST, onAccept, NULL); 
    // 设置为base事件 
    event_base_set(base, &evListen); 
    // 添加事件 
    event_add(&evListen, NULL); 

    // 事件循环 
    event_base_dispatch(base); 

}

void onTest () {
    // TCPController*controller = (TCPController*)CKClassFactory::getInstance().UseClass("CRegisterController");
    // cout << controller->index() << endl;
    // delete controller;
    Utils::CFileOpt fileOpt;
    char filename[] = "tmp/client.json";
    // open file.
    char* buff = fileOpt.readFileBuf (filename);
    // cout << buff << endl;
    
    string str = buff;
    list<string> arr_list = Utils::StringOpt::split (str, "^SHD^");
    list<string>::iterator pos = arr_list.begin ();
    cJSON *root;
        
    while (pos != arr_list.end ()) {
        // cout << *pos << endl;

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
        char *route = cJSON_GetObjectItem (root, "route")->valuestring;
        if (route == NULL) {
            cout << "route is null" << endl;
            return ;
        }
        // cout << route << endl;
        char tmp[] = "CController";
        char *tRoute = new char[strlen (route)+strlen (tmp)+1];
        sprintf (tRoute, "C%sController", route);         

        cout << tRoute << endl;
        TCPController*controller = (TCPController*)CKClassFactory::getInstance().UseClass(tRoute);
        if (controller == NULL) {
            controller = (TCPController *)CKClassFactory::getInstance ().UseClass ("CNofoundController");
        }
        if (controller != NULL) {
            cout << controller->index() << endl;
        }
        delete controller;
   
        *pos++;
    }
    
    cJSON_Delete (root);
    // close file.
    fileOpt.releaseBuf ();
}

int main () {

    onTest ();
    // run ();
    return 0;
}

