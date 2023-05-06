#pragma once

#include <memory>

#include "luka/core/core.h"
#include "luka/editor/editor.h"
#include "luka/function/function.h"
#include "luka/platform/platform.h"
#include "luka/resource/resource.h"

namespace luka {

class Engine {
 public:
  Engine();
  void Run();

 private:
  std::shared_ptr<Platform> platform_;
  std::shared_ptr<Core> core_;
  std::shared_ptr<Resource> resource_;
  std::shared_ptr<Function> function_;
  std::shared_ptr<Editor> editor_;
};
}  // namespace luka