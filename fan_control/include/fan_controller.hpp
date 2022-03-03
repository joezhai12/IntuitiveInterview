#pragma once

class FanController{
public:
  Fan() = delete;
  Fan(uint32_t pwm_count);

private:
  uint32_t pwm_count;
};