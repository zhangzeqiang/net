#ifndef __REGISTER_H__
#define __REGISTER_H__
#include "TCPController.h"

class CRegisterController: public TCPController {

    public:
        // 声明
        DECLARE_CLASS(CRegisterController);
        
        CRegisterController () {}
        static void* GetInstance (void) {return new CRegisterController();}

        string index ();
};

REGISTER_CLASS (CRegisterController);

string CRegisterController::index () {

    return "register"; 
}
#endif /** __REGISTER_H__ */

