#include "bind.h"

/** 支持100个用户在线 */
struct UserPack UserLists[LEN_USERLISTS];

/** 支持100个绑定关系 */
struct BindPack BindLists[LEN_BINDENCE];

void emptyUserLists() 
{
    int i = 0;

    for (i=0;i<LEN_USERLISTS;i++)
    {
        UserLists[i].socket = -1;
        UserLists[i].userid = "";
        UserLists[i].state = UNUSED;
    }

    for (i=0;i<LEN_BINDENCE;i++) {
        BindLists[i].state = UNUSED;
    }
}

int bindUserAndSocket (string userid, int socket, int classify) 
{
    int i = 0;

    /** 如果已经绑定过客服则更新userid */
    for (i=0;i<LEN_USERLISTS;i++) {
        if (UserLists[i].state == USED &&
            UserLists[i].socket == socket && 
            UserLists[i].classify == classify)
        {
            UserLists[i].state = USED;
            /** 更新userid */
            UserLists[i].userid = userid;
            return SUCCESS;
        }
    }

    /** 如果未绑定过客服则选择空闲位置绑定 */
    /** 在使用前要先保证userid和socket的合法性*/
    for (i=0;i<LEN_USERLISTS;i++) {
        if (UserLists[i].state == UNUSED)
        {
            /** 未使用,可使用 */
            UserLists[i].state = USED;
            UserLists[i].userid = userid;
            UserLists[i].socket = socket;
            UserLists[i].classify = classify;
            return SUCCESS;
        }
    }
    return FAIL;
}

int unbindUserAndSocket (string userid, int classify)
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

int getSocketWithUserid (string userid, int classify)
{
    int i = 0;

    for (i=0;i<LEN_USERLISTS;i++) {
        if (UserLists[i].state == USED &&
            UserLists[i].userid == userid && 
            UserLists[i].classify == classify) {
            return UserLists[i].socket;
        }
    } 
    return NOEXIST;
}

string getUserWithSocket (int socket, int classify) {
    int i = 0;

    for (i=0;i<LEN_USERLISTS;i++) {
        if (UserLists[i].state == USED &&
            UserLists[i].classify == classify &&
            UserLists[i].socket == socket) {
            return UserLists[i].userid;
        }
    }
    return "";
}

int getStateWithUserid (string userid, int classify) {
    int i = 0;

    for (i=0;i<LEN_USERLISTS;i++) {
        if (UserLists[i].userid == userid && 
                UserLists[i].state == USED &&
                UserLists[i].classify == classify) {
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

int addBindence (string serviceid, string userid) {
    
    int i=0;

    // 如果已经绑定过了
    for (i=0;i<LEN_BINDENCE;i++) {
        if (BindLists[i].state == USED && 
                BindLists[i].userid == userid) {
            return FAIL;
        }
    }

    // 加入新绑定
    for (i=0;i<LEN_BINDENCE;i++) {
        if (BindLists[i].state == UNUSED) {
            BindLists[i].userid = userid;
            BindLists[i].serviceid = serviceid;
            BindLists[i].state = USED;
            return SUCCESS;
        }
    } 
    return FAIL; 
}

int rmBindence (string serviceid, string userid) {

    int i=0;

    // 设置为空闲区
    for (i=0;i<LEN_BINDENCE;i++) {
        if (BindLists[i].state == USED && 
            BindLists[i].serviceid == serviceid &&
            BindLists[i].userid == userid)
        {
            BindLists[i].state = UNUSED;
            return SUCCESS;
        }
    } 
    return FAIL;
}

int rmBindence (string serviceid) {

    int i=0;

    // 设置为空闲区
    for (i=0;i<LEN_BINDENCE;i++) {
        if (BindLists[i].state == USED && 
                BindLists[i].serviceid == serviceid) {
            BindLists[i].state = UNUSED;
            return SUCCESS;
        }
    }
    return FAIL;
}

int getUserSocketWithBindence (string serviceid, string userid) {
 
    int i=0;

    for (i=0;i<LEN_BINDENCE;i++) {
        if (BindLists[i].state == USED && 
                BindLists[i].serviceid == serviceid &&
                BindLists[i].userid == userid) {
            // 存在此绑定,获取用户socket
            return getSocketWithUserid (userid, USER);
        }
    }   

    // 不存在此绑定,则将绑定解除
    rmBindence (serviceid, userid);
    return NOEXIST;
}

int getServiceSocketWithBindence (string userid) {
    
    int i=0;

    for (i=0;i<LEN_BINDENCE;i++) {
        if (BindLists[i].state == USED && 
                BindLists[i].userid == userid) {
            // 存在此绑定,获取客服socket
            return getSocketWithUserid (BindLists[i].serviceid, SERVICE);
        }
    }

    // 返回不存在绑定,此用户为新用户
    return NOEXIST;
}

