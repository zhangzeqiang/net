#ifndef __SERVER__H__
#define __SERVER__H__
#include "progress.h"
#include "client.h"

using namespace PROGRESSDEMO;

class CServer: public CEvent {
    
public:
    void run ();
};


#endif /** __SERVER__H__ */

