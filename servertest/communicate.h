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

#include "bind.h"
using namespace std;

extern void onResolve (int icliFd, char *buf); 
extern void onHandle (int icliFd, Smap myMap);
extern int onReturn (int iCliFd, char *buf);
extern int onClose (int iCliFd);

#endif /** __COMMUNICATE__H__  */

