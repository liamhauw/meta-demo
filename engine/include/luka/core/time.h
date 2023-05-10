#pragma once

#include <chrono>
#include <ratio>

namespace luka {

class Time {
 public:
  void Tick() {
    {
      using namespace std::chrono;
      auto now{high_resolution_clock::now()};
      auto diff{now - last_};
      delta_time_ = duration<double, std::milli>{diff}.count();
      last_ = now;
      fps_ = static_cast<uint32_t>(1.0 / delta_time_);
    }
  }

 private:
  double delta_time_{0.0};
  uint32_t fps_{0};
  std::chrono::high_resolution_clock::time_point last_{
      std::chrono::high_resolution_clock::now()};
};

};  // namespace luka