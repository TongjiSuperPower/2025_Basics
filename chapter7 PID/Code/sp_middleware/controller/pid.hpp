#ifndef _PID_HPP
#define _PID_HPP

#include <cmath>

#include "cstdint"

namespace controller
{
//����posģʽ��deltaģʽ
enum class PIDMode
{
  POSITION = 0,
  DELTA
};

class PID
{
private:
  struct pid_param_t
  {
    PIDMode mode;                 // PIDģʽ
    float kp, ki, kd;             // PID����������
    float out, pout, iout, dout;  // PID�������P�������I�������D�����
    float max_out, max_iout;      // PID��������ƣ�I���������
    float dbuf[3], err[3];        // D����˲�����������������
    float set, fdb;               // �趨ֵ������ֵ
    float alpha;                  // D���˲���ϵ��
  } pid_data_;

  // ��������ֵ�����ֵ����Сֵ֮��
  inline void limitMax(float & input, float max)
  {
    if (input > max)
      input = max;
    else if (input < -max)
      input = -max;
  }

public:
  PID(PIDMode mode, const float pid[3], float max_out, float max_iout, float alpha);
  ~PID() {}
  float pid_calc(float set, float fdb);
};

}  // namespace controller

#endif  // _PID_HPP