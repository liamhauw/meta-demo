#pragma once

#include <chrono>

namespace luka {

class Time {
 public:
  void Tick();

 private:
  double delta_time_{0.0};
  uint32_t fps_{0};
  std::chrono::high_resolution_clock::time_point last_{
      std::chrono::high_resolution_clock::now()};
};

}  // namespace luka