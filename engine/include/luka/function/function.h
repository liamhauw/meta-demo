#pragma once
#include <memory>

#include "luka/function/rendering.h"

namespace luka {
class Function {
 public:
  Function();

  void Tick();
  bool ShouldClose();

 private:
  std::shared_ptr<Rendering> rendering_;
};
}  // namespace luka
