#include "fan_manager.hpp"
#include <cstring>
#include <iostream>

FanManager::FanManager(std::vector<std::pair<uint32_t, std::string>> fan_config){
  for(int i = 0; i < fan_config.size(); ++i){
    uint32_t cur_pwm = fan_config[i].first;
    std::string cur_address = fan_config[i].second;
    uint32_t default_pwm = static_cast<uint32_t>(cur_pwm * 0.2);
    fans_.push_back(FanController(default_pwm,
                                  cur_pwm,
                                  cur_address,
                                  static_cast<uint8_t>(i)));

    // write to register at 20% duty cycle just in case we are waiting on
    // subsystem data
  }
}

float FanManager::getDutyCycle(){
  if(max_subsystem_temp_ <= TEMP_20PCT_DUTY_CYCLE){
    return 0.2f;
  }
  else if(max_subsystem_temp_ >= TEMP_100PCT_DUTY_CYCLE){
    return 1.0f;
  }

  // linearly interpolate duty cycle between 20% and 100% duty cycle temperatures
  // i.e. (1.0-0.2)/(temp(100%)-temp(20%)) = (dc-0.2)/(max_temp-temp(20%))
  // dc = 0.8/(temp(100%)-temp(20%)) * (max_temp-temp(20%)) + 0.2
  return ((0.8f)/(TEMP_100PCT_DUTY_CYCLE-TEMP_20PCT_DUTY_CYCLE) * (max_subsystem_temp_ - TEMP_20PCT_DUTY_CYCLE) + 0.2);
}

void FanManager::cycle(unsigned char *buffer, int bytes_recvd){
  float cur_temperature;
  float cur_max_temperature = -273.0f; // surely not measuring below -273 Celsius

  for(int i = 0; i < bytes_recvd/sizeof(float); ++i){
    // Unpack buffer to get temperature
    memcpy(&cur_temperature, &buffer[i*sizeof(float)], sizeof(float));

    std::cout << "Subsystem " << i
              << " temperature: " << cur_temperature
              << std::endl;

    cur_max_temperature = std::max(cur_max_temperature, cur_temperature);
  }

  max_subsystem_temp_ = cur_max_temperature;

  std::cout << "Current max subsystem temperature: "
            << cur_max_temperature << std::endl;

  // determine duty cycle %
  float duty_cycle = getDutyCycle();

  std::cout << "Setting duty cycle to "
            << duty_cycle * 100.0f << "%"
            << std::endl;

  // set all fan pwm counts
  for(int i = 0; i < fans_.size(); ++i){
    fans_[i].setPWMCount(duty_cycle);
  }
}