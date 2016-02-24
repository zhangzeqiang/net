#ifndef __PROGRESSDEMO__
#define __PROGRESSDEMO__
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <wait.h>
#include "hello.h"
using namespace std;

namespace PROGRESSDEMO {

    /**
     * 声明类
     */
    class CProgress;
    class CEvent;

    class CProgress {
    private:
        CEvent* p_cEvent;
    
    public:
        pid_t p_id;

    public: 
        CProgress ();

        void setCEvent (CEvent* cEvent);

        /** 新建进程 */
        void create ();
        
    };

    class CEvent {
      
    public:
        /**
         * 执行函数(进程实现函数)
         */  
        virtual void run () = 0;   
    };


    int Main (); 
}
#endif /** __PROGRESSDEMO__ */

