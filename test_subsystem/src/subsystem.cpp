#include "subsystem.hpp"

#include <iostream>
#include <cstring>

SubsystemManager::SubsystemManager(size_t count){
  for(int i = 0; i < count; ++i){
    subsystems_.push_back(Subsystem(0.0f));
  }
}

void SubsystemManager::cycle(unsigned char *buffer){
  // Get temperature as user input
  for(int i = 0; i < subsystems_.size(); ++i){
    std::cout << "Enter subsystem #" << i << " temperature: ";
    std::cin >> subsystems_[i].temperature;
    std::cout << std::endl;

    // Pack buffer with temperatures
    memcpy(&buffer[i*sizeof(float)], &subsystems_[i].temperature, sizeof(float));
  }
}