#include "luka/core/core.h"

namespace luka {

Core::Core() { time_ = std::make_shared<Time>(); }

void Core::Tick() {
  time_->Tick();
}

};  // namespace luka