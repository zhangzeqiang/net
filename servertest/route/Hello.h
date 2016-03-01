#ifndef __HELLO_CPP__
#define __HELLO_CPP__
#include "TCPController.h"

class CHelloController: public TCPController {

    public: 
        // 声明
        DECLARE_CLASS(CHelloController);

    public:
        CHelloController () {}
        static void* GetInstance (void) {return new CHelloController();}
        string index (int socket, string jsonStr);
    
};

string CHelloController::index (int socket, string jsonStr) {

    writeWithDefaultHeader (socket, "hello");
    return "hello"; 
}

REGISTER_CLASS (CHelloController);

#endif

