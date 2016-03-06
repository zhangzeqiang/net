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
#include "server"

using namespace std;

int main () {

    // onTest ();
    char ip[] = "127.0.0.1";

    Server::config (ip, 8888);
    Server::run ();
    return 0;
}

