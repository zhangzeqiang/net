#ifndef __NOFOUND_CPP__
#define __NOFOUND_CPP__
#include "TCPController.h"

class CNofoundController: public TCPController {

    public: 
        // 声明
        DECLARE_CLASS(CNofoundController);

    public:
        CNofoundController () {}
        static void* GetInstance (void) {return new CNofoundController();}
        string index ();
    
};

string CNofoundController::index () {

    return "404"; 
}

REGISTER_CLASS (CNofoundController);

#endif

