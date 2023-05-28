#pragma once
#include <memory>

#include "luka/core/file.h"
#include "luka/core/math.h"
#include "luka/core/time.h"

namespace luka {

class Core {
 public:
  Core();

  void Tick();

 private:
  std::shared_ptr<Time> time_;
};

};  // namespace luka