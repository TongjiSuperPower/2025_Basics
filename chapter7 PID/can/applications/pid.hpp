#ifndef PID_HPP
#define PID_HPP

#include "cstdint"

struct pid
{
  float dt;
  float kp, ki, kd;
  float out;                // PID的输出值
  float pout;               // P项的输出值
  float iout;               // I项的输出值
  float dout;               // D项的输出值
  float max_out, max_iout;  // 输出和积分项的最大值
  float set;                // 设定值
  float fdb;                // 反馈值
  float err[2];             // 误差缓冲区
  float dbuf[2];            // D项的滤波器缓冲区
};

void pid_init(pid * pid, float dt, float kp, float ki, float kd, float max_out, float max_iout);

float pid_calc(pid * pid, float set, float fdb);

#endif  // PID_HPP