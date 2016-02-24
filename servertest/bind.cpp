#include "bind.h"

/** 支持100个用户在线 */
struct UserPack UserLists[LEN_USERLISTS];

void emptyUserLists() 
{
    int i = 0;

    for (i=0;i<LEN_USERLISTS;i++)
    {
        UserLists[i].socket = -1;
        UserLists[i].userid = "";
        UserLists[i].state = UNUSED;
    }
}

int bindUserAndSocket (string userid, int socket) 
{
    int i = 0;

    /** 如果已经绑定过则更新userid */
    for (i=0;i<LEN_USERLISTS;i++) {
        if (UserLists[i].state == USED &&
            UserLists[i].socket == socket)
        {
            UserLists[i].state = USED;
            /** 更新userid */
            UserLists[i].userid = userid;
            return SUCCESS;
        }
    }

    /** 如果未绑定过则选择空闲位置绑定 */
    /** 在使用前要先保证userid和socket的合法性*/
    for (i=0;i<LEN_USERLISTS;i++) {
        if (UserLists[i].state == UNUSED)
        {
            /** 未使用,可使用 */
            UserLists[i].state = USED;
            UserLists[i].userid = userid;
            UserLists[i].socket = socket;
            return SUCCESS;
        }
    }
    return FAIL;
}

int unbindUserAndSocket (string userid)
{
    int i = 0;
    
    /** 在使用前要先保证userid和socket的合法性*/
    for (i=0;i<LEN_USERLISTS;i++) {
        if (UserLists[i].state == USED && 
            UserLists[i].userid == userid) {
            UserLists[i].state == UNUSED;    
            return SUCCESS;
        }    
    }
    return FAIL;
}

int unbindUserAndSocket (int socket)
{
    int i = 0;
    
    /** 在使用前要先保证userid和socket的合法性*/
    for (i=0;i<LEN_USERLISTS;i++) {
        if (UserLists[i].state == USED && 
            UserLists[i].socket == socket) {
            UserLists[i].state == UNUSED;    
            return SUCCESS;
        }    
    }
    return FAIL;
}

int getSocketWithUserid (string userid)
{
    int i = 0;

    for (i=0;i<LEN_USERLISTS;i++) {
        if (UserLists[i].state == USED &&
            UserLists[i].userid == userid) {
            return UserLists[i].socket;
        }
    } 
    return -1;
}

int getStateWithUserid (string userid) {
    int i = 0;

    for (i=0;i<LEN_USERLISTS;i++) {
        if (UserLists[i].userid == userid && 
                UserLists[i].state == USED) {
            return USED;
        }
    }
    return UNUSED;
}

void unBindAll ()
{
    int i = 0;

    for (i=0;i<LEN_USERLISTS;i++) {
        if (UserLists[i].state == USED) {
            UserLists[i].state = UNUSED;
        }
    }
}

