#pragma once

#include <vector>

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
  SubsystemManager(size_t count);

  void cycle(unsigned char *buffer);
private:
  std::vector<Subsystem> subsystems_;
};