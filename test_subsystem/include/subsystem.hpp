#pragma once

#include <unordered_map>

struct Subsystem{
  float temperature;

  Subsystem() = delete;
  Subsystem(float t) : temperature(t) {}
};

/**
 * @brief Subsystem Manager manages configurable number of subsystems.
 *
 */
class SubsystemManager{
public:
  SubsystemManager(size_t count){

  }

  Subsystem() = delete;
private:
  std::unordered_map<size_t, Subsystem*> subsystems_;
};