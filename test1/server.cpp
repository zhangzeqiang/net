#include "server.h"

/**
 * CServer: CEvent
 */
void CServer::run () {
    /**
     * 客户端进程
     */
    CProgress p_client;
    p_client.setCEvent (new CClient ()); 
    p_client.create ();

    /**
     * 监听获取客户端发送过来的数据
     */
    
    cout << "I'm is CServer!" << endl;
    cout << "This is Server pid:" << getpid() << endl;
}

