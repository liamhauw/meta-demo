#include "luka/engine.h"

#include <memory>

namespace luka {

Engine::Engine() {
  time_ = std::make_shared<Time>();
  function_ = std::make_shared<Function>();
  editor_ = std::make_shared<Editor>();
}

void Engine::Run() {
  while (!function_->ShouldClose()) {
    time_->Tick();
    function_->Tick();
  }
}

}  // namespace luka
