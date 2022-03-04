#include "fan_controller.hpp"
#include <iostream>

FanController::FanController(uint32_t pwm_count, uint32_t max_pwm_count, uint8_t id) :
  pwm_count_(pwm_count),
  MAX_PWM_COUNT(max_pwm_count),
  id_(id)
{
}

/**
 * @brief Write data to register
 *
 * Note: I have never done this, so to the reviewer: do not think too hard
 *       about what I am doing here as it probably makes no sense.
 */
void FanController::writePWMCount(){

}

/**
 * @brief Set PWM count for a fan and write to register
 *
 * @param duty_cycle (0.0 <= duty_cycle <= 1.0)
 */
void FanController::setPWMCount(float duty_cycle){
  if(duty_cycle > 1.0f){
    std::cout << "Duty cycle cannot be above 100%!!! Setting duty cycle to 100%" << std::endl;
    duty_cycle = 1.0f;
  }
  else if(duty_cycle < 0.0f){
    std::cout << "Duty cycle cannot be below 0%!!! Setting duty cycle to 0%" << std::endl;
    duty_cycle = 0.0f;
  }

  pwm_count_ = static_cast<uint32_t>(duty_cycle * MAX_PWM_COUNT);

  // write to register
  writePWMCount();

  std::cout << "Fan #" << unsigned(id_) << " PWM count set to " << pwm_count_ << std::endl;
}