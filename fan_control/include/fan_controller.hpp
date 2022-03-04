#pragma once

#include <stdint.h>

class FanController{
public:
  FanController() = delete;
  FanController(uint32_t pwm_count, uint32_t max_pwm_count, uint8_t id);

  void writePWMCount();

  void setPWMCount(float duty_cycle);

private:
  const uint32_t MAX_PWM_COUNT;
  uint32_t pwm_count_;
  uint8_t id_;
};