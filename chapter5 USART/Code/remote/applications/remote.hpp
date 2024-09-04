#ifndef REMOTE_H
#define REMOTE_H

#include "bsp_rc.h"
#include "struct_typedef.h"

class Remote
{
public:
  Remote();
  ~Remote();
  RC_ctrl_t * get_rc_ctrl_point();

private:
  RC_ctrl_t rc_ctrl;
};

#endif
