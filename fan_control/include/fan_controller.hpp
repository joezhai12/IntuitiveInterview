#pragma once

#include <stdint.h>
#include <string>

class FanController{
public:
  FanController() = delete;
  FanController(uint32_t pwm_count,
                uint32_t max_pwm_count,
                std::string write_address,
                uint8_t id);

  void writePWMCount();

  void setPWMCount(float duty_cycle);

private:
  const uint32_t MAX_PWM_COUNT;
  uint32_t pwm_count_;
  std::string write_address_;
  uint8_t id_;
};