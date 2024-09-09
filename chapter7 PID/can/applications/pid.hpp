#ifndef PID_HPP
#define PID_HPP

#include "cstdint"

struct pid
{
  float dt;
  float kp, ki, kd;
  float out;                // PID�����ֵ
  float pout;               // P������ֵ
  float iout;               // I������ֵ
  float dout;               // D������ֵ
  float max_out, max_iout;  // ����ͻ���������ֵ
  float set;                // �趨ֵ
  float fdb;                // ����ֵ
  float err[2];             // ������
  float dbuf[2];            // D����˲���������
};

void pid_init(pid * pid, float dt, float kp, float ki, float kd, float max_out, float max_iout);

float pid_calc(pid * pid, float set, float fdb);

#endif  // PID_HPP