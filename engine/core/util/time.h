/*
  Copyright (C) 2023-present Liam Hauw.

  SPDX license identifier: MIT.   

  Time header file.
*/

#pragma once

#include <chrono>

namespace luka {

class Time {
 public:
  const Time& Tick();
  double GetDeltaTime() const;

 private:
  double delta_time_{0.0};
  std::chrono::high_resolution_clock::time_point last_{
      std::chrono::high_resolution_clock::now()};
};

}  // namespace luka