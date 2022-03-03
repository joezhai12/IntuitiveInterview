#pragma once

class FanManager{
public:
  FanManager(size_t count);

  void cycle(unsigned char *buffer);
private:
  float max_subsystem_temp_;
};