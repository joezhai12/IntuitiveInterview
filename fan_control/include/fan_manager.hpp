#pragma once

#include <vector>
#include <stdint.h>
#include <string>

#include "fan_controller.hpp"

constexpr float TEMP_20PCT_DUTY_CYCLE = 25.0f; // Celsius
constexpr float TEMP_100PCT_DUTY_CYCLE = 75.0f; // Celsius

class FanManager{
public:
  FanManager(std::vector<std::pair<uint32_t, std::string>> fan_config);

  float getDutyCycle();

  void cycle(unsigned char *buffer, int bytes_recvd);
private:
  float max_subsystem_temp_;
  std::vector<FanController> fans_;
};