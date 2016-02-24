#ifndef __BIND_H__
#define __BIND_H__
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
using namespace std;

#define USED 0
#define UNUSED 1
#define SUCCESS 0
#define FAIL 1

/**
 * 将用户id和socket绑定,已实现转发通信
 */
struct UserPack {
    int socket;
    string userid;
    unsigned char state; /** used-0, unused-1  */
};

typedef std::map<std::string, std::string> Smap;

#define LEN_USERLISTS 100

/** 支持100个用户在线 */
extern struct UserPack UserLists[LEN_USERLISTS];

extern void emptyUserLists();
extern int bindUserAndSocket (string userid, int socket);
extern int unbindUserAndSocket (string userid);
extern int unbindUserAndSocket (int socket);
extern int getSocketWithUserid (string userid);
extern int getStateWithUserid (string userid);
extern void unBindAll();

#endif /** __BIND_H__ */

