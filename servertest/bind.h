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
#include "include/ctypes.h"

#define USED 0      // 在使用
#define UNUSED 1    // 空闲区

#define SERVICE 1   // 客服
#define USER 0      // 用户

#define NOEXIST -1
#define EXIST -2

/**
 * 将用户id和socket绑定,已实现转发通信
 */
struct UserPack {
    int socket;
    int classify; /** 用户0, 客服1 */
    string userid;
    unsigned char state; /** used-0, unused-1  */
};

/**
 * 客服-用户绑定
 */
struct BindPack {
    string serviceid;      /** 客服id*/
    string userid;         /** 用户id*/
    unsigned char state;    /** used-0, unused-1 */
};

typedef std::map<std::string, std::string> Smap;

#define LEN_USERLISTS 100
#define LEN_BINDENCE 100  // 客服-用户绑定关系
/** 支持100个用户(包括普通用户和客服)在线 */
extern struct UserPack UserLists[LEN_USERLISTS];

/** 支持100个绑定关系 */
extern struct BindPack BindLists[LEN_BINDENCE];

/**
 * 用户和socket绑定 (classify指定客服还是用户)
 */ 
extern void emptyUserLists();
extern int bindUserAndSocket (string userid, int socket, int classify);
extern int unbindUserAndSocket (string userid, int classify);
extern int unbindUserAndSocket (int socket);
extern int getSocketWithUserid (string userid, int classify);
extern string getUserWithSocket (int socket, int classify);
extern int getStateWithUserid (string userid, int classify);
extern void unBindAll();

/**
 * 客服和用户socket绑定
 */
extern int addBindence (string serviceid, string userid);
extern int rmBindence (string serviceid, string userid);

// 根据绑定情况获取用户socket(如果不存在则将此绑定解除)
extern int getUserSocketWithBindence (string serviceid, string userid);

// 根据用户id获取客服socket
extern int getServiceSocketWithBindence (string userid);

// 根据用户id获取客服userid
extern string getServeridWithBindence (string userid);

// 删除客服serviceSocket的所有绑定关系
extern int rmBindence (string serviceid);

#endif /** __BIND_H__ */

