#ifndef SEND_TASK_H
#define SEND_TASK_H

#include <cstdint>

class Send
{
public:
  Send();
  ~Send();
  uint8_t send_data;
  uint8_t rece_data;
};

#endif