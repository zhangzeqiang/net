#ifndef __COMMUNICATE__H__
#define __COMMUNICATE__H__
#include <iostream>
#include <event.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <string>
#include <list>
#include "bind.h"
#include "include/fileopt.h"
#include "pkg/cJSON.h"
#include "route/TCPController.h"
using namespace std;

/**
 * 解析客户端发送过来的数据,以^为分割符,#为段内分割
 * fromuserid#123^touserid#124^msg#Hello World
 * key#value^key1#value1^key2#value2
 */
extern void onResolve (int icliFd, char *buf); 

/**
 * 连接断开时执行
 */ 
extern int onClose (int iCliFd);

#endif /** __COMMUNICATE__H__  */

