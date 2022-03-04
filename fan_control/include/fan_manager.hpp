#pragma once

#include <vector>
#include <stdint.h>

#include "fan_controller.hpp"

constexpr float TEMP_20PCT_DUTY_CYCLE = 25.0f; // Celsius
constexpr float TEMP_100PCT_DUTY_CYCLE = 75.0f; // Celsius

class FanManager{
public:
  FanManager(std::vector<uint32_t> max_pwm_counts);

  float getDutyCycle();

  void cycle(unsigned char *buffer, int bytes_recvd);
private:
  float max_subsystem_temp_;
  std::vector<FanController> fans_;
};