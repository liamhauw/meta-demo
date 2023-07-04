#pragma once

#include <chrono>

namespace luka {

class Time {
 public:
  const Time& Tick();
  [[nodiscard]] double GetDeltaTime() const;

 private:
  double delta_time_{0.0};
  std::chrono::high_resolution_clock::time_point last_{std::chrono::high_resolution_clock::now()};
};

}  // namespace luka