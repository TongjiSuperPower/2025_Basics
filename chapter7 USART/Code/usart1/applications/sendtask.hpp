#ifndef SEND_TASK_H
#define SEND_TASK_H

#include <cstdint>

class Send
{
public:
  Send() { Send::send_data = 0; }
  ~Send() {}
  uint8_t send_data;
};

#endif
