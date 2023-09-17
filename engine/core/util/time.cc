/*
  SPDX license identifier: MIT

  Copyright (C) 2023-present Liam Hauw.

  Time source file.
*/

#include "core/util/time.h"

namespace luka {

const Time& Time::Tick() {
  using namespace std::chrono;
  auto now{high_resolution_clock::now()};
  auto diff{now - last_};
  delta_time_ = duration<double>{diff}.count();
  last_ = now;
  return *this;
}

double Time::GetDeltaTime() const { return delta_time_; }

}  // namespace luka