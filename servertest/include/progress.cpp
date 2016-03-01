#include "progress.h"

namespace PROGRESSDEMO {

    /**
     * Progress
     */
    CProgress::CProgress () {
        p_id = 0;
    }
    
    void CProgress::setCEvent (CEvent* cEvent) {
        p_cEvent = cEvent;  
    }

    void CProgress::create () {
        
        if ( (p_id = fork ()) == 0) {
            /** childpid */ 
            p_cEvent->run ();

            exit (0); 
        }
    }

}

