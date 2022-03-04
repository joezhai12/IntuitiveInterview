#pragma once

#include <string>
#include <vector>
#include <stdint.h>

/**
 * @brief Read the fan configuration file CSV into a vector
 *
 * @param filename The file name to be read.
 * @param config The configuration for fans (pwm counts and address to be written to)
 */
void read_fan_config(std::string filename,
                     std::vector<std::pair<uint32_t, std::string>>& config);