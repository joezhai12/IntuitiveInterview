#include "fan_config_reader.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

void read_fan_config(std::string filename,
                     std::vector<std::pair<uint32_t, std::string>>& config){
  std::fstream file;

  file.open(filename, std::ios::in);

  if(!file.is_open()){
    std::cout << "Could not open file!" << std::endl;
    return;
  }

  std::uint32_t cur_pwm_count;
  std::string cur_address;
  std::string line, tmp;

  while(getline(file, line)){
    std::stringstream str(line);

    getline(str, tmp, ',');
    cur_pwm_count = static_cast<uint32_t>(std::stoul(tmp));
    getline(str, cur_address, ',');
    config.push_back(std::make_pair(cur_pwm_count, cur_address));
  }
}