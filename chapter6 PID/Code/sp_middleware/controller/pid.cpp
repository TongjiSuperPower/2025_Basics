#include "pid.hpp"

namespace controller
{
PID::PID(PIDMode mode, const float pid[3], float max_out, float max_iout, float alpha)
{
  pid_data_.mode = mode;

  pid_data_.kp = pid[0];
  pid_data_.ki = pid[1];
  pid_data_.kd = pid[2];
  pid_data_.max_out = max_out;
  pid_data_.max_iout = max_iout;
  pid_data_.alpha = alpha;

  pid_data_.out = pid_data_.pout = pid_data_.iout = pid_data_.dout = 0.0f;
  pid_data_.dbuf[0] = pid_data_.dbuf[1] = pid_data_.dbuf[2] = 0.0f;
  pid_data_.err[0] = pid_data_.err[1] = pid_data_.err[2] = 0.0f;
  pid_data_.set = pid_data_.fdb = 0.0f;
}

float PID::pid_calc(float set, float fdb)
{
  if (set != pid_data_.set) pid_data_.iout /= 2.0f;

  // ΢������
  pid_data_.dbuf[2] = pid_data_.dbuf[1];
  pid_data_.dbuf[1] = pid_data_.dbuf[0];
  pid_data_.dbuf[0] = pid_data_.fdb - fdb;
  // �˲�
  pid_data_.dbuf[0] =
    pid_data_.alpha * pid_data_.dbuf[0] + (1.0f - pid_data_.alpha) * pid_data_.dbuf[1];

  pid_data_.err[2] = pid_data_.err[1];
  pid_data_.err[1] = pid_data_.err[0];
  pid_data_.err[0] = set - fdb;

  pid_data_.set = set;
  pid_data_.fdb = fdb;

  if (pid_data_.mode == PIDMode::POSITION) {
    // Kp
    pid_data_.pout = pid_data_.kp * pid_data_.err[0];
    // Ki,���λ���
    pid_data_.iout += pid_data_.ki * (pid_data_.err[0] + pid_data_.err[1]) / 2.0f;
    limitMax(pid_data_.iout, pid_data_.max_iout);
    // Kd
    pid_data_.dout = pid_data_.kd * pid_data_.dbuf[0];
  }

  else if (pid_data_.mode == PIDMode::DELTA) {
    // Kp
    pid_data_.pout = pid_data_.kp * (pid_data_.err[0] - pid_data_.err[1]);
    // Ki,���λ���
    pid_data_.iout += pid_data_.ki * (pid_data_.err[0] + pid_data_.err[1]) / 2.0f;
    limitMax(pid_data_.iout, pid_data_.max_iout);
    //Kd
    pid_data_.dout = pid_data_.kd * pid_data_.dbuf[0];
  }

  pid_data_.out = pid_data_.pout + pid_data_.iout + pid_data_.dout;
  limitMax(pid_data_.out, pid_data_.max_out);

  return pid_data_.out;
}
}  // namespace controller