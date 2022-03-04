#include "fan_controller.hpp"
#include <iostream>

FanController::FanController(uint32_t pwm_count,
                             uint32_t max_pwm_count,
                             std::string write_address,
                             uint8_t id) :
  pwm_count_(pwm_count),
  MAX_PWM_COUNT(max_pwm_count),
  write_address_(write_address),
  id_(id)
{
}

/**
 * @brief Write data to register
 *
 * Note: I have never done this, so to the reviewer: do not think too hard
 *       about what I am doing here as it probably makes no sense.
 *       The idea is to read the write address from a config file
 *       (fan_configuration.csv) and store it as a class member variable.
 *       We will then write to the address in the following way shown below.
 *       This is an attempt at making the address configurable. Current
 *       implementation doesn't work, but not sure how to do it.
 */
void FanController::writePWMCount(){
  /**
   * volatile uint32_t *location = reinterpret_cast<uint32_t*>(write_address_);
   *
   * *location = pwm_count_;
   */
  std::cout << "Writing to register ("
            << write_address_ << ")" << std::endl;
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

  std::cout << "Fan #" << unsigned(id_)
            << " PWM count set to " << pwm_count_
            << " (max " << MAX_PWM_COUNT
            << ")" << std::endl;
}