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
        string index ();
    
};

string CHelloController::index () {

    return "hello"; 
}

REGISTER_CLASS (CHelloController);

#endif

