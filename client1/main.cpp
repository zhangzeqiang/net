#include <iostream>
#include <event.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>

using namespace std;

// 事件
struct event_base* base;

// 配置
#define PORT 8888
#define SERVERADDR "127.0.0.1"

// 客户端socket连接符
int _icliFd;

void onRead(int iCliFd, short iEvent, void *arg); 
void onWrite (int icliFd, short iEvent, void *arg);

void onRead(int iCliFd, short iEvent, void *arg) 
{ 
    int iLen; 
    char buf[1500]; 
   
    iLen = recv(iCliFd, buf, 1500, 0); 
   
    if (iLen <= 0) { 
        cout << "Client Close" << endl; 

        // 连接结束=0 | 连接错误<0, 将事件池删除并释放内存空间
        struct event *pEvRead = (struct event*)arg; 
        event_del(pEvRead); 
        delete pEvRead; 
                      
        close(iCliFd); 
        return;
    }

    buf[iLen] = 0;
    cout<< "服务器回复:" << buf << endl;
}

void onWrite (int icliFd, short iEvent, void *arg)
{
    int iLen;
    char buf[] = "My id's 123";
    cout << "客户端写入:" << buf << endl;

    iLen = write (icliFd, buf, strlen (buf));
   
    // 发完就关闭 
    struct event *pEvWrite = (struct event*)arg;
    event_del(pEvWrite);
    delete pEvWrite;

    close (icliFd);
    return ;
}

void onInput (int iinpFd, short iEvent, void *arg) 
{
    int iLen;
    char buf[1024];
    iLen = read (iinpFd, buf, sizeof (buf));

    if (iLen > 0) 
    {
        cout << "准备发送:" << buf << endl;

        /** 最后一个字符为回车,去除,发送过滤处理不合适的字符 */
        buf[iLen-1] = 0;
        // 发送数据给服务器
        write (_icliFd, buf, strlen (buf));
    }
    return ;
}

void onConnect (int iSvrFd) 
{ 
    cout << "客户机建立连接... " << endl;

    // 保存客户端socket连接符
    _icliFd = iSvrFd;
    
    /** 等待标准输入可读  */
    struct event *pEvInput = new event; 
    event_set(pEvInput, STDIN_FILENO, EV_READ|EV_PERSIST, onInput, pEvInput); 
    event_base_set(base, pEvInput); 
    event_add(pEvInput, NULL);

    /** 等待socket可读 */
    struct event *pEvRead = new event;
    event_set(pEvRead, iSvrFd, EV_READ|EV_PERSIST, onRead, pEvRead); 
    event_base_set(base, pEvRead); 
    event_add(pEvRead, NULL);
}

int main (int arg, char* argv[]) {

    int iCliFd;   
    struct sockaddr_in sSvrAddr; 
             
    memset(&sSvrAddr, 0, sizeof(sSvrAddr));   
    sSvrAddr.sin_family = AF_INET;   
    sSvrAddr.sin_addr.s_addr = inet_addr(SERVERADDR);     
    sSvrAddr.sin_port = htons(PORT);
    iCliFd = socket(AF_INET, SOCK_STREAM, 0);   
    
    base = event_base_new ();

    
    int iresult = connect(iCliFd, (struct sockaddr*)&sSvrAddr, sizeof (sSvrAddr));

    if (iresult == 0) {
       onConnect(iCliFd);
    } 

    // 事件循环 
    event_base_dispatch(base); 

    return 0;
}


