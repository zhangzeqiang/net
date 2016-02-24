#include "client.h"
#include "server.h"

int main () {

    /**
     * 服务器进程
     */
    CProgress p_server;
    p_server.setCEvent (new CServer ());
    p_server.create (); 

    /** 新进程id */
    cout << "server pid is " << p_server.p_id << endl;

    /** 等待子进程结束 */
    waitpid (p_server.p_id, NULL, 0);
    exit (0);

    return 0;
}
