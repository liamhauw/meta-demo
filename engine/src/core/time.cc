#include "luka/core/time.h"

#include <ratio>

namespace luka {

void Time::Tick() {
  using namespace std::chrono;
  auto now{high_resolution_clock::now()};
  auto diff{now - last_};
  delta_time_ = duration<double, std::milli>{diff}.count();
  last_ = now;
  fps_ = static_cast<uint32_t>(1.0 / delta_time_);
}
}  // namespace luka