#ifndef REMOTE_H
#define REMOTE_H


#include "struct_typedef.h"
#include "bsp_rc.h"


#ifdef __cplusplus
extern "C" {
#endif





class Remote
{
public:
    Remote();
    ~Remote();
    RC_ctrl_t* get_rc_ctrl_point();

private:
    RC_ctrl_t rc_ctrl;

};


void remote_task();



#ifdef __cplusplus   
}
#endif



#endif 

