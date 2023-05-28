#pragma once

#include <memory>

#include "luka/core/time.h"
#include "luka/editor/editor.h"
#include "luka/function/function.h"

namespace luka {

class Engine {
 public:
  Engine();
  void Run();

 private:
  std::shared_ptr<Time> time_;
  std::shared_ptr<Function> function_;
  std::shared_ptr<Editor> editor_;
};
}  // namespace luka