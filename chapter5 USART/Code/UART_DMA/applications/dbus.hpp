#include "usart.h"

enum class DBusSwitchMode
{
  DOWN,
  MID,
  UP
};

struct Dbus
{
  float stick_rh;  // [-1, 1] right horizontal
  float stick_rv;  // [-1, 1] right vertical
  float stick_lh;  // [-1, 1] left horizontal
  float stick_lv;  // [-1, 1] left vertical

  DBusSwitchMode switch_r;
  DBusSwitchMode switch_l;
} remote;

void remote_restart();