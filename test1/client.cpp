#include "client.h"

/**
 * CClient: CEvent
 */
void CClient::run () {

    /**
     * 创建管道
     */
    char fifoname[100];

    pid_t pid = getpid ();
    snprintf (fifoname, sizeof (fifoname), "fifo/%ld", (long) pid);

    if ((mkfifo (fifoname, FILE_MODE) <0 ) && (errno != EEXIST)) {
        err_sys ("cant't create %s", fifoname);
    }
    cout << "I'm is CClient!" << endl;
    cout << "This is client pid:" << pid << endl;

    /**
     * 客户端定时给服务器管道发送数据
     */

    /** 释放管道 */
}

