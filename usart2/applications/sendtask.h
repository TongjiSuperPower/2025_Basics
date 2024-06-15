#ifndef SEND_TASK_H
#define SEND_TASK_H

extern "C" {

#include <cstdint>
class Send
{

public:
    Send();
    ~Send();
    uint8_t send_data;
    uint8_t rece_data;
};

void send_task();



}

#endif
