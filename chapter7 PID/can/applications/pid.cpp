#include "pid.hpp"

void pid_init(pid * pid, float dt, float kp, float ki, float kd, float max_out, float max_iout)
{
  pid->dt = dt;
  pid->kp = kp;
  pid->ki = ki;
  pid->kd = kd;
  pid->max_out = max_out;
  pid->max_iout = max_iout;
  pid->iout = 0;
  pid->err[0] = pid->err[1] = 0;
  pid->dbuf[0] = pid->dbuf[1] = 0;
}

float limit_max(float x, float max)
{
  if (x > max) return max;
  if (x < -max) return -max;
  return x;
}

float pid_calc(pid * pid, float set, float fdb)
{
  pid->err[1] = pid->err[0];
  pid->err[0] = set - fdb;

  pid->dbuf[1] = pid->dbuf[0];
  pid->dbuf[0] = pid->err[1] - pid->err[0];

  pid->pout = pid->kp * pid->err[0];

  pid->iout += pid->ki * pid->err[0] * pid->dt;
  pid->iout = limit_max(pid->iout, pid->max_iout);

  pid->dout = pid->kd * pid->dbuf[0] / pid->dt;

  pid->out = pid->pout + pid->iout + pid->dout;
  pid->out = limit_max(pid->out, pid->max_out);

  return pid->out;
}